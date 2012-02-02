/**
 * zimilo@code-trick.com
 */

#ifndef __ZIACK_HASHTABLE_ITER_H__
#define __ZIACK_HASHTABLE_ITER_H__

#include "ziack_config.h"
#include "ziack_types.h"

ziack_hashtable_iter_t *
ziack_hashtable_iter_create(ziack_hashtable_t *h);

ziack_bool_t
ziack_hashtable_iter_valid(ziack_hashtable_iter_t *it);

ziack_hashtable_key_t *
ziack_hashtable_iter_get_key(ziack_hashtable_iter_t *it);

void *
ziack_hashtable_iter_get_value(ziack_hashtable_iter_t *it);

ziack_hashtable_entry_t *
ziack_hashtable_iter_get_entry(ziack_hashtable_iter_t *it);

ziack_rc_t
ziack_hashtable_iter_next(ziack_hashtable_iter_t *it);

ziack_rc_t
ziack_hashtable_iter_destroy(ziack_hashtable_iter_t *it);

#endif // !__ZIACK_HASHTABLE_ITER_H__
