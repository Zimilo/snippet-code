/**
 * zimilo@code-trick.com
 */

#ifndef __ZIACK_HINT_H__
#define __ZIACK_HINT_H__

#include "ziack_config.h"
#include "ziack_types.h"

ziack_hint_key_t *
ziack_hint_key_create(void        *key,
		      ziack_size_t key_size);

ziack_rc_t
ziack_hint_key_destory(ziack_hint_key_t *key);

ziack_hint_value_t *
ziack_hint_value_create();

ziack_rc_t
ziack_hint_value_destroy(ziack_hint_value_t *hint);

ziack_hint_version_t *
ziack_hint_version_create();

void
ziack_hint_value_free_func(void *v);

ziack_hint_t *
ziack_hint_create(ziack_flag_t flags);

ziack_hint_t *
ziack_hint_create_from_file(const char   *file_name,
			    ziack_flag_t  flags);

ziack_rc_t
ziack_hint_destroy(ziack_hint_t *hint);

ziack_rc_t 
ziack_hint_dump_to_file(ziack_hint_t *hint, 
			const char   *file_name);

ziack_rc_t 
ziack_hint_add_version(ziack_hint_t         *hint, 
		       ziack_hint_key_t     *key, 
		       ziack_hint_version_t *value);

ziack_hint_version_t *
ziack_hint_lookup_version(ziack_hint_t      *hint,
			  ziack_hint_key_t  *key,
			  ziack_size_t       vidx);

ziack_rc_t
ziack_hint_add(ziack_hint_t       *hint,
	       ziack_hint_key_t   *key,
	       ziack_hint_value_t *value);

ziack_rc_t
ziack_hint_delete(ziack_hint_t     *hint,
		  ziack_hint_key_t *key);

ziack_hint_value_t *
ziack_hint_lookup(ziack_hint_t     *hint,
		  ziack_hint_key_t *key);

ziack_rc_t
ziack_hint_update(ziack_hint_t       *hint,
		  ziack_hint_key_t   *key,
		  ziack_hint_value_t *value);

void
ziack_hint_version_print_func(void *v);

void
ziack_hint_value_print_func(void *v);

void
ziack_hint_print(ziack_hint_t *hint);

#endif // !__ZIACK_HINT_H__
