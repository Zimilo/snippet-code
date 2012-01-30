#ifndef __ZIACK_HINT_H__
#define __ZIACK_HINT_H__

/**
 * zimilo@code-trick.com
 *
 * The ziack hint file format
 */

#include "ziack_config.h"
#include "ziack_types.h"

ziack_hint_t *
ziack_hint_create(ziack_flag_t flags);

ziack_hint_t *
ziack_hint_create_from_file(const char   *file_name,
			    ziack_flag_t flags);

ziack_rc_t 
ziack_hint_dump2file(ziack_hint_t *hint, 
		     const char   *file_name);

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



#endif // !__ZIACK_HINT_H__
