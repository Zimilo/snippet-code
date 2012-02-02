/**
 * zimilo@code-trick.com
 */

#include "ziack_hashtable.h"
#include "ziack_hashtable_iter.h"
#include "ziack_memory.h"
#include "ziack_tests.h"


ziack_hashtable_iter_t *
ziack_hashtable_iter_create(ziack_hashtable_t *h)
{
  ziack_hashtable_iter_t *it = (ziack_hashtable_iter_t *)ziack_calloc(1, sizeof(ziack_hashtable_iter_t));
  if (NULL == it) return NULL;
  it->h = h;
  it->entry = NULL;
  it->entry_parent = NULL;
  it->idx = h->bucket_count;
  if (0 == h->entry_count) return it;
  ziack_size_t i = 0;
  for (; i < it->idx; ++i) {
    if (h->buckets[i] != NULL) {
      it->idx = i;
      it->entry = h->buckets[i];
      break;
    }
  }
  return it;
}

ziack_bool_t
ziack_hashtable_iter_valid(ziack_hashtable_iter_t *it)
{
  return (it->entry == NULL && it->idx == it->h->bucket_count) ? ZIACK_FALSE : ZIACK_TRUE;
}

ziack_hashtable_key_t *
ziack_hashtable_iter_get_key(ziack_hashtable_iter_t *it)
{
  return it->entry->key;
}

void *
ziack_hashtable_iter_get_value(ziack_hashtable_iter_t *it)
{
  return it->entry->value;
}

ziack_hashtable_entry_t *
ziack_hashtable_iter_get_entry(ziack_hashtable_iter_t *it)
{
  return it->entry;
}

ziack_rc_t
ziack_hashtable_iter_next(ziack_hashtable_iter_t *it)
{
  if (NULL == it->entry && it->h->bucket_count == it->idx) return ZIACK_RC_ITER_GUARDER;
  if (it->entry->next != NULL) {
    it->entry_parent = it->entry;
    it->entry = it->entry->next;
    return ZIACK_RC_OK;
  }
  it->entry_parent = NULL;
  it->entry = NULL;
  if (it->h->bucket_count <= ++it->idx) {
    return ZIACK_RC_ITER_GUARDER;
  }
  ziack_size_t i = it->idx;
  for (; i < it->h->bucket_count; ++i) {
    if (it->h->buckets[i] != NULL) {
      it->idx = i;
      it->entry = it->h->buckets[i];
      return ZIACK_RC_OK;
    }
  }
  it->idx = it->h->bucket_count;
  return ZIACK_RC_ITER_GUARDER;
}

ziack_rc_t
ziack_hashtable_iter_destroy(ziack_hashtable_iter_t *it)
{
  ziack_free(it);
  return ZIACK_RC_OK;
}
