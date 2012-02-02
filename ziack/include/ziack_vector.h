/**
 * zimilo@code-trick.com
 */

#ifndef __ZIACK_VECTOR_H__
#define __ZIACK_VECTOR_H__

#include "ziack_config.h"
#include "ziack_types.h"

ziack_vector_t *
ziack_vector_create(ziack_size_t count);

ziack_bool_t
ziack_vector_empty(ziack_vector_t *vec);

ziack_size_t
ziack_vector_count(ziack_vector_t *vec);

ziack_rc_t
ziack_vector_reserve(ziack_vector_t *vec,
		     ziack_size_t    size);

ziack_rc_t
ziack_vector_expand(ziack_vector_t *vec,
		    ziack_size_t    size);

ziack_rc_t
ziack_vector_push(ziack_vector_t *vec,
		 void            *data);

ziack_rc_t
ziack_vector_destroy(ziack_vector_t *vec,
		     void (*vector_free_func)(void *));

void *
ziack_vector_index(ziack_vector_t *vec,
		   ziack_size_t    vidx);

void
ziack_vector_print(ziack_vector_t *vec,
		   void (*ziack_vector_value_print_func)(void *v));

#endif // !__ZIACK_VECTOR_H__
