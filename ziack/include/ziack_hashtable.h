/**
 * zimilo@code-trick.com
 */

#ifndef __ZIACK_HASHTABLE_H__
#define __ZIACK_HASHTABLE_H__

#include "ziack_config.h"
#include "ziack_types.h"

uint32_t
ziack_hashtable_hashfunc(void        *key,
			 ziack_size_t key_size);

ziack_rc_t
ziack_hashtable_equalfunc(void         *key,
			  ziack_size_t  key_size,
			  void         *key2,
			  ziack_size_t  key2_size);

ziack_hashtable_key_t *
ziack_hashtable_key_create(void        *key,
			   ziack_size_t key_size);

ziack_rc_t
ziack_hashtable_key_destroy(ziack_hashtable_key_t *key);


ziack_hashtable_t *
ziack_hashtable_create(ziack_size_t min_size,
		       uint32_t (*hashfunc)(void *key, ziack_size_t key_size),
		       ziack_rc_t (*key_equal_func)(void *key, ziack_size_t key_size, void *key2, ziack_size_t key2_size));

ziack_rc_t
ziack_hashtable_reserve(ziack_hashtable_t *h,
			ziack_size_t       bucket_count);

ziack_rc_t
ziack_hashtable_add(ziack_hashtable_t       *h,
		    ziack_hashtable_key_t   *key,
		    void                    *value);

void *
ziack_hashtable_delete(ziack_hashtable_t     *h,
		       ziack_hashtable_key_t *key);

ziack_rc_t
ziack_hashtable_update(ziack_hashtable_t       *h,
		       ziack_hashtable_key_t   *key,
		       void                    *value);

void *
ziack_hashtable_lookup(ziack_hashtable_t     *h,
		       ziack_hashtable_key_t *key);

ziack_size_t
ziack_hashtable_count(ziack_hashtable_t *h);

ziack_size_t
ziack_hashtable_bucket_count(ziack_hashtable_t *h);

void
ziack_hashtable_destroy(ziack_hashtable_t *h,
			void (*ziack_entry_value_free_func)(void *));

ziack_rc_t
ziack_hashtable_expand(ziack_hashtable_t *h,
		       ziack_size_t       new_size);

ziack_rc_t
ziack_hashtable_compact(ziack_hashtable_t *h, ziack_flag_t flags);

void
ziack_hashtable_print(ziack_hashtable_t *h,
		      void (*ziack_hashtable_value_print_func)(void *));
#endif // !__ZIACK_HASHTABLE_H__
