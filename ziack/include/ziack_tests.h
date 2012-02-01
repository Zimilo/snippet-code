#ifndef __ZIACK_TESTS_H__
#define __ZIACK_TESTS_H__

#include "ziack_config.h"
#include <assert.h>

#ifdef ZIACK_DEBUG
#define ziack_assert(e) ((void)0)
#else
#define ziack_assert(exp) (assert((exp)))
#endif // !ZIACK_DEBUG

#endif // !__ZIACK_TESTS_H__
