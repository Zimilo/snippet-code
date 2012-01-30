/**
 * zimilo@code-trick.com
 */

#include "ziack_hashtable.h"

ziack_hashtable_t *
ziack_hashtable_create(ziack_size_t min_size,
		       uint32_t (*hashfunc)(void *key, ziack_size_t key_size),
		       ziack_rc_t (*key_equal_func)(void *key, ziack_size_t key_size, void *key2, ziack_size_t key2_size)) 
{
  ziack_hashtable_t *ht = (ziack_hashtable_t *)ziack_calloc(1, sizeof(ziack_hashtable_t));
  ht->hashfunc = hashfunc;
  ht->key_equal_func = key_equal_func;
  ht->buckets = (ziack_hashtable_entry_t *)ziack_calloc(1, sizeof(ziack_hashtable_entry_t *) * min_size);
  ht->bucket_count = min_size;
  ht->entry_count = 0;
  return ht;
}

ziack_rc_t
ziack_hashtable_add(ziack_hashtable_t       *h,
		    ziack_hashtable_key_t   *key,
		    ziack_hashtable_value_t *value)
{
  if (h->entry_count > 100) {
    ziack_hashtable_expand(h, h->entry_count << 1); // to expand to 2 * size
  }
  struct ziack_hashtable_entry_t *entry = (ziack_hashtable_entry_t *)ziack_calloc(1, sizeof(ziack_hashtable_entry_t));
  entry->hash = h->hashfunc(key->key, key->key_size);
  uint32_t index = entry->hash % h->bucket_count;
  entry->next = h->buckets[index];
  entry->key = key;
  entry->value = value;
  h->buckets[index] = entry;
  return ZIACK_RC_OK;
}

ziack_hashtable_data_t *
ziack_hashtable_delete(ziack_hashtable_t     *h,
		       ziack_hashtable_key_t *key)
{
  uint32_t hash = h->hashfunc(key->key, key->key_size);
  ziack_hashtable_entry_t *entry = h->buckets[hash % h->bucket_count];
  ziack_hashtable_value_t *value = NULL;
  while (entry != NULL) {
    if ((hash == entry->hash) && (h->key_equal_func(entry->key->key, entry->key->key_size, key->key, key->key_size) == ZIACK_RC_EQUAL)) {
      --(h->entry_count);
      h->buckets[index] = entry->next;
      value = entry->value;
      ziack_free(entry->key);
      ziack_free(entry);
      return value;
    }
    e = e->next;
  }
  return NULL;
}

ziack_rc_t
ziack_hashtable_update(ziack_hashtable_t *h,
		       ziack_hashtable_t *key,
		       ziack_hashtable_t *value)
{
  uint32_t hash = h->hashfunc(key->key, key->key_size);
  ziack_hashtable_entry_t *entry = h->buckets[hash % h->bucket_count];
  while (entry != NULL) {
    if ((hash == entry->hash) && (h->key_equal_func(entry->key->key, entry->key->key_size, key->key, key->key_size) == ZIACK_RC_EQUAL)) {
      ziack_free(entry->value);
      entry->value = value;
      return ZIACK_RC_OK;
    }
    e = e->next;
  }
  return ZIACK_RC_NOT_FOUND;
}

ziack_hashtable_data_t *
ziack_hashtable_lookup(ziack_hashtable_t     *h,
		       ziack_hashtable_key_t *key)
{
  uint32_t hash = h->hashfunc(key->key, key->key_size);
  ziack_hashtable_entry_t *entry = h->buckets[hash % h->bucket_count];
  while (entry != NULL) {
    if ((hash == entry->hash) && (h->key_equal_func(entry->key->key, entry->key->key_size, key->key, key->key_size) == ZIACK_RC_EQUAL)) {
      return entry->value;
    }
    e = e->next;
  }
  return NULL;
}

ziack_size_t
ziack_hashtable_count(ziack_hashtable_t *h)
{
  return h->entry_count;
}

void
ziack_hashtable_destroy(ziack_hashtable_t *h,
			ziack_flag_t      flag)
{
  if (flag & ZIACK_FLAG_HASHTABLE_FREE_ENTRY) {
    // free the resource owned by the entry
  }
  ziack_free(h);
}
