#include "ziack_vector.h"
#include "ziack_memory.h"
#include "ziack_tests.h"

ziack_vector_t *
ziack_vector_create(ziack_size_t count)
{
  ziack_vector_t *vec = (ziack_vector_t *)ziack_calloc(1, sizeof(ziack_vector_t));
  if (NULL == vec) return NULL;
  vec->count = count;
  if (count == 0) {
    vec->data = NULL;
  } else {
    if (NULL == (vec->data = (void **)ziack_calloc(1, sizeof(char *) * count))) {
      ziack_free(vec);
      return NULL;
    }
  }
  vec->capacity = count;
  return vec;
}

ziack_rc_t
ziack_vector_push(ziack_vector_t *vec,
		  void           *data)
{
  if (vec->count >= vec->capacity) {
    ziack_vector_expand(vec, (vec->capacity == 0) ? 1 : vec->capacity << 1);
  }
  vec->data[vec->count] = data;
  ++vec->count;
  return ZIACK_RC_OK;
}

ziack_bool_t
ziack_vector_empty(ziack_vector_t *vec)
{
  return (vec->count == 0) ? ZIACK_TRUE : ZIACK_FALSE;
}

ziack_size_t
ziack_vector_count(ziack_vector_t *vec)
{
  return vec->count;
}

ziack_size_t
ziack_vector_capacity(ziack_vector_t *vec)
{
  return vec->capacity;
}

ziack_rc_t
ziack_vector_reserve(ziack_vector_t *vec,
		     ziack_size_t    size)
{
  return ziack_vector_expand(vec, size);
}

ziack_rc_t
ziack_vector_expand(ziack_vector_t *vec,
		    ziack_size_t    size)
{
  if (size <= vec->capacity) return ZIACK_RC_OK;
  vec->data = (void **)ziack_realloc(vec->data, size * sizeof(char *));
  memset(&(vec->data[vec->capacity]), 0, (size - vec->capacity) * sizeof(char *));
  vec->capacity = size;
  return ZIACK_RC_OK;
}

ziack_rc_t
ziack_vector_destroy(ziack_vector_t *vec,
		     void (*vector_free_func)(void *))
{
  if (vec->data != NULL) {
    ziack_size_t i = 0;
    for (; i < vec->count; ++i) {
      if (vector_free_func != NULL) {
	vector_free_func(vec->data[i]);
      }
    }
    ziack_free(vec->data);
  }
  ziack_free(vec);
  return ZIACK_RC_OK;
}

void *
ziack_vector_index(ziack_vector_t *vec,
		   ziack_size_t    vidx)
{
  if (vidx >= vec->count) return NULL;
  return vec->data[vidx];
}

void
ziack_vector_print(ziack_vector_t *vec,
		   void (*ziack_vector_value_print_func)(void *v))
{
  ziack_size_t i;
  for (i = 0; i < vec->count; ++i) {
    if (ziack_vector_value_print_func != NULL) {
      printf("##########################\n");
      ziack_vector_value_print_func(vec->data[i]);
      printf("##########################\n");
    }
  }
}

#if 0
int 
main(int argc, char **argv)
{
  char *v1 = "V1", *v2 = "V2", *v3 = "V3", *v4 = "V4";
  ziack_vector_t *vec = ziack_vector_create(0);
  ziack_vector_push(vec, v1);
  ziack_vector_push(vec, v2);
  ziack_vector_push(vec, v3);
  ziack_vector_push(vec, v4);
  ziack_assert(ziack_vector_count(vec) == 4);
  ziack_assert(ziack_vector_capacity(vec) == 4);
  char *p1 = ziack_vector_index(vec, 0);
  char *p2 = ziack_vector_index(vec, 1);
  char *p3 = ziack_vector_index(vec, 2);
  char *p4 = ziack_vector_index(vec, 3);
  ziack_assert(memcmp(p1, v1, strlen(p1)) == 0);
  ziack_assert(memcmp(p2, v2, strlen(p2)) == 0);
  ziack_assert(memcmp(p3, v3, strlen(p3)) == 0);
  ziack_assert(memcmp(p4, v4, strlen(p4)) == 0);
  ziack_vector_destroy(vec, NULL);
}
#endif 
