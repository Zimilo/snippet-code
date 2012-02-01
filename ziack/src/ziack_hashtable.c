/**
 * zimilo@code-trick.com
 */

#include "ziack_hashtable.h"
#include "ziack_hashtable_iter.h"
#include "ziack_memory.h"
#include "ziack_tests.h"

uint32_t
ziack_hashtable_hashfunc(void        *key,
			 ziack_size_t key_size)
{
  char *ptr = (char *)key;
  uint32_t hash = 0;
  uint32_t x	= 0;
  ziack_size_t i = 0;
  for (; i < key_size; ++i) {
    hash = (hash << 4) + (*ptr++);
    if ((x = hash & 0xF0000000L) != 0)
      {
	hash ^= (x >> 24);
	hash &= ~x;
      }
  }
  return (hash & 0x7FFFFFFF);
}

ziack_rc_t
ziack_hashtable_equalfunc(void         *key,
			  ziack_size_t  key_size,
			  void         *key2,
			  ziack_size_t  key2_size)
{
  return ((key_size == key2_size && memcmp(key, key2, key_size) == 0)) ? ZIACK_RC_EQUAL : ZIACK_RC_NOT_EQUAL;
}

ziack_hashtable_key_t *
ziack_hashtable_key_create(void        *key,
			   ziack_size_t key_size)
{
  ziack_hashtable_key_t *hk = (ziack_hashtable_key_t *)ziack_calloc(1, sizeof(ziack_hashtable_key_t));
  if (NULL == hk) return NULL;
  hk->key_size = key_size;
  memcpy(hk->key, key, key_size);
  return hk;
}

ziack_rc_t
ziack_hashtable_key_destroy(ziack_hashtable_key_t *key)
{
  ziack_free(key);
  return ZIACK_RC_OK;
}

ziack_hashtable_t *
ziack_hashtable_create(ziack_size_t min_size,
		       uint32_t (*hashfunc)(void *key, ziack_size_t key_size),
		       ziack_rc_t (*key_equal_func)(void *key, ziack_size_t key_size, void *key2, ziack_size_t key2_size)) 
{
  ziack_hashtable_t *ht = (ziack_hashtable_t *)ziack_calloc(1, sizeof(ziack_hashtable_t));
  if (NULL == ht) return NULL;
  ht->hashfunc = hashfunc;
  ht->key_equal_func = key_equal_func;
  ht->buckets = (ziack_hashtable_entry_t **)ziack_calloc(1, sizeof(ziack_hashtable_entry_t *) * min_size);
  ht->bucket_count = min_size;
  ht->entry_count = 0;
  return ht;
}

ziack_rc_t
ziack_hashtable_reserve(ziack_hashtable_t *h,
			ziack_size_t       bucket_count)
{
  if (bucket_count <= h->bucket_count) return ZIACK_RC_OK;
  h->buckets = (ziack_hashtable_entry_t **)ziack_realloc(h->buckets, bucket_count * sizeof(ziack_hashtable_entry_t *));
  memset(&(h->buckets[h->bucket_count]), 0, (bucket_count - h->bucket_count) * sizeof(ziack_hashtable_entry_t *));
  h->bucket_count = bucket_count;
  return ZIACK_RC_OK;
}

ziack_rc_t
ziack_hashtable_add(ziack_hashtable_t       *h,
		    ziack_hashtable_key_t   *key,
		    void                    *value)
{
  if ((double)++h->entry_count / (double)h->bucket_count >= 1.20)
    ziack_hashtable_expand(h, h->entry_count << 1); // to expand to 2 * size
  ziack_hashtable_entry_t *entry = (ziack_hashtable_entry_t *)ziack_calloc(1, sizeof(ziack_hashtable_entry_t));
  if (entry == NULL) return ZIACK_RC_OOM;
  entry->hash = h->hashfunc(key->key, key->key_size);
  uint32_t index = entry->hash % h->bucket_count;
  entry->next = h->buckets[index];
  entry->key = key;
  entry->value = value;
  h->buckets[index] = entry;
  return ZIACK_RC_OK;
}

void *
ziack_hashtable_delete(ziack_hashtable_t     *h,
		       ziack_hashtable_key_t *key)
{
  uint32_t hash = h->hashfunc(key->key, key->key_size);
  ziack_hashtable_entry_t *entry = h->buckets[hash % h->bucket_count];
  void *value = NULL;
  while (entry != NULL) {
    if ((hash == entry->hash) && (h->key_equal_func(entry->key->key, entry->key->key_size, key->key, key->key_size) == ZIACK_RC_EQUAL)) {
      --h->entry_count;
      h->buckets[hash % h->bucket_count] = entry->next;
      value = entry->value;
      ziack_hashtable_key_destroy(entry->key);
      ziack_free(entry);
      return value;
    }
    entry = entry->next;
  }
  return NULL;
}

ziack_rc_t
ziack_hashtable_update(ziack_hashtable_t       *h,
		       ziack_hashtable_key_t   *key,
		       void                    *value)
{
  uint32_t hash = h->hashfunc(key->key, key->key_size);
  ziack_hashtable_entry_t *entry = h->buckets[hash % h->bucket_count];
  while (entry != NULL) {
    if ((hash == entry->hash) && (h->key_equal_func(entry->key->key, entry->key->key_size, key->key, key->key_size) == ZIACK_RC_EQUAL)) {
      ziack_free(entry->value);
      entry->value = value;
      return ZIACK_RC_OK;
    }
    entry = entry->next;
  }
  return ZIACK_RC_NOT_FOUND;
}

void *
ziack_hashtable_lookup(ziack_hashtable_t     *h,
		       ziack_hashtable_key_t *key)
{
  uint32_t hash = h->hashfunc(key->key, key->key_size);
  ziack_hashtable_entry_t *entry = h->buckets[hash % h->bucket_count];
  while (entry != NULL) {
    if ((hash == entry->hash) && (h->key_equal_func(entry->key->key, entry->key->key_size, key->key, key->key_size) == ZIACK_RC_EQUAL)) {
      return entry->value;
    }
    entry = entry->next;
  }
  return NULL;
}

ziack_size_t
ziack_hashtable_count(ziack_hashtable_t *h)
{
  return h->entry_count;
}

ziack_size_t
ziack_hashtable_bucket_count(ziack_hashtable_t *h)
{
  return h->bucket_count;
}

void
ziack_hashtable_destroy(ziack_hashtable_t *h,
			void (*ziack_entry_value_free_func)(void *))
{
  ziack_hashtable_entry_t *entry = NULL;
  ziack_size_t i = 0;
  for (; i < h->bucket_count; ++i) {
    entry = h->buckets[i];
    while (entry != NULL) {
      h->buckets[i] = entry->next;
      ziack_hashtable_key_destroy(entry->key);
      if (ziack_entry_value_free_func != NULL) {
	ziack_entry_value_free_func(entry->value);
      }
      ziack_free(entry);
      --(h->entry_count);
      entry = h->buckets[i];
    }
  }
  ziack_free(h->buckets);
  ziack_free(h);
}

ziack_rc_t 
ziack_hashtable_expand(ziack_hashtable_t *h,
		       ziack_size_t       new_size)
{
  if (new_size <= h->bucket_count) return ZIACK_RC_OK;
  ziack_hashtable_entry_t **new_buckets = (ziack_hashtable_entry_t **)ziack_calloc(1, sizeof(ziack_hashtable_entry_t *) * new_size);
  if (new_buckets == NULL) return ZIACK_RC_OOM;
  if (NULL == new_buckets) return ZIACK_RC_OOM;
  ziack_hashtable_entry_t *entry = NULL;
  ziack_size_t i = 0;
  for (; i < h->bucket_count; ++i) {
    while (NULL != (entry = h->buckets[i])) {
      h->buckets[i] = entry->next;
      uint32_t index = entry->hash % new_size;
      entry->next = new_buckets[index];
      new_buckets[index] = entry;
    }
  }
  ziack_free(h->buckets);
  h->buckets = new_buckets;
  h->bucket_count = new_size;
  return ZIACK_RC_OK;
}

ziack_rc_t
ziack_hashtable_compact(ziack_hashtable_t *h,
			ziack_flag_t       flags)
{
  // @todo
  return ZIACK_RC_OK;
}

#if 1
int 
main(int argc, char **argv)
{
  ziack_hashtable_t *h = ziack_hashtable_create(16, ziack_hashtable_hashfunc, ziack_hashtable_equalfunc);

  // T1
  ziack_assert(ziack_hashtable_count(h) == 0);
  ziack_assert(ziack_hashtable_bucket_count(h) == 16);

  // T2
  ziack_hashtable_reserve(h, 100);
  ziack_assert(ziack_hashtable_count(h) == 0);
  ziack_assert(ziack_hashtable_bucket_count(h) == 100);

  // T3
  char *k = "zimilo", *v = "SampleData", *k2 = "ziack", *v2 = "ZiackNoSql"; 
  ziack_hashtable_key_t *key = ziack_hashtable_key_create(k, strlen(k));
  ziack_hashtable_add(h, key, v);
  ziack_assert(ziack_hashtable_count(h) == 1);
  
  ziack_hashtable_key_t *key2 = ziack_hashtable_key_create(k2, strlen(k2));
  ziack_hashtable_add(h, key2, v2);
  ziack_assert(ziack_hashtable_count(h) == 2);
  
  ziack_hashtable_iter_t *it = ziack_hashtable_iter_create(h);
  while (it->entry != NULL) {
    void *value = ziack_hashtable_iter_get_value(it);
    ziack_assert((memcmp(value, v, strlen(v)) == 0) || (memcmp(value, v2, strlen(v2)) == 0));
    if (ziack_hashtable_iter_next(it) == ZIACK_RC_ITER_GUARDER) 
      break;
  }
  ziack_hashtable_iter_destroy(it);
  
  // T4
  void *value = ziack_hashtable_delete(h, key);
  ziack_assert(ziack_hashtable_count(h) == 1);
  
  ziack_hashtable_destroy(h, NULL);
}
#endif

