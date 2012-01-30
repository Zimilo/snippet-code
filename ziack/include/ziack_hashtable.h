/**
 * zimilo@code-trick.com
 */

#ifndef __ZIACK_HASHTABLE_H__
#define __ZIACK_HASHTABLE_H__

#include "ziack_config.h"
#include "ziack_types.h"

ziack_hashtable_t *
ziack_hashtable_create(ziack_size_t min_size,
		       uint32_t (*hashfunc)(void *key, ziack_size_t key_size),
		       int (*key_equal_func)(void *key, ziack_size_t key_size, void *key2, ziack_size_t key2_size));

ziack_rc_t
ziack_hashtable_add(ziack_hashtable_t       *h,
		    ziack_hashtable_key_t   *key,
		    ziack_hashtable_value_t *value);

ziack_hashtable_data_t *
ziack_hashtable_delete(ziack_hashtable_t     *h,
		       ziack_hashtable_key_t *key);

ziack_rc_t
ziack_hashtable_update(ziack_hashtable_t *h,
		       ziack_hashtable_t *key,
		       ziack_hashtable_t *value);

ziack_hashtable_data_t *
ziack_hashtable_lookup(ziack_hashtable_t     *h,
		       ziack_hashtable_key_t *key);

ziack_size_t
ziack_hashtable_count(ziack_hashtable_t *h);

void
ziack_hashtable_destroy(ziack_hashtable_t *h,
			ziack_flag_t      flag);

ziack_rc_t
ziack_hashtable_expand(ziack_hashtable_t *h,
		       ziack_size_t       new_size);

ziack_rc_t
ziack_hashtable_compact(ziack_hashtable_t *h, ziack_flag_t flags);

#endif // !__ZIACK_HASHTABLE_H__
