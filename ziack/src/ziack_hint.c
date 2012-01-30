#include "ziack_hint.h"
#include "ziack_hashtable.h"
#include "ziack_memory.h"

ziack_hint_t *
ziack_hint_create(ziack_flag_t flags)
{
  ziack_hint_t *hint = (ziack_hint_t *)ziack_calloc(1, sizeof(ziack_hint_t));
  hint->data = ziack_hashtable_create(16, ziack_hashtable_hashfunc, ziack_hashtable_key_equal_func);
  return hint;
}

ziack_hint_t *
ziack_hint_create_from_file(const char   *file_name,
			    ziack_flag_t flags)
{
  return NULL;
}

ziack_rc_t 
ziack_hint_dump2file(ziack_hint_t *hint, 
		     const char   *file_name) {
  return ZIACK_RC_OK;
}

ziack_rc_t 
ziack_hint_add(ziack_hint_t       *hint, 
	       ziack_hint_key_t   *key, 
	       ziack_hint_value_t *value) {

  ziack_hashtable_key_t *hk = (ziack_hashtable_key_t *)ziack_calloc(1, sizeof(ziack_hashtable_key_t));
  hk->key_size = key->key_size;
  hk->key = key->key;

  ziack_hashtable_value_t *hvalue = (ziack_hashtable_value_t *)ziack_calloc(1, sizeof(ziack_hashtable_value_t));
  hvalue->data_size = sizeof(*value);
  hvalue->data = (void *)value;

  ziack_hashtable_add(hint->data, hk, hvalue);
  return ZIACK_RC_OK;
}

ziack_rc_t
ziack_hint_delete(ziack_hint_t     *hint,
		  ziack_hint_key_t *key) {
  return ZIACK_RC_OK;
}


ziack_hint_value_t *
ziack_hint_lookup(ziack_hint_t     *hint,
		  ziack_hint_key_t *key) {
  return NULL;
}

ziack_rc_t
ziack_hint_update(ziack_hint_t       *hint,
		  ziack_hint_key_t   *key,
		  ziack_hint_value_t *value) {
  return ZIACK_RC_OK;
}

