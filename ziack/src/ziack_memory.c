/**
 * zimilo@code-trick.com
 */

#include "ziack_memory.h"

void *
ziack_malloc(size_t size)
{
  return malloc(size);
}

void *
ziack_calloc(size_t count,
	     size_t size)
{
  return calloc(count, size);
}

void 
ziack_free(void *ptr)
{
  free(ptr);
}

void *
ziack_realloc(void *ptr,
	      size_t size)
{
  return realloc(ptr, size);
}
