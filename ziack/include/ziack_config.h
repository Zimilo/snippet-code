#ifndef __ZIACK_CONFIG_H__
#define __ZIACK_CONFIG_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>

typedef uint32_t ziack_size_t;
typedef uint32_t ziack_flag_t;

typedef enum __ZIACK_RC {
  ZIACK_RC_OK = 0,
  ZIACK_RC_EQUAL = 1,
  ZIACK_RC_NOT_EQUAL,
  ZIACK_RC_NOT_FOUND,
  ZIACK_RC_KEY_EXISTS,
  ZIACK_RC_OOM,
} ziack_rc_t;

typedef enum __ZIACK_BOOL {
  ZIACK_TRUE = 1,
  ZIACK_FALSE = 0
} ziack_bool_t;

#define ZIACK_FLAG_FREE_ENTRY_VALUE 0x01

#endif // !_ZIACK_CONFIG_H__

