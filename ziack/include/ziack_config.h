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
  ZIACK_RC_FILE_ERROR,
  ZIACK_RC_ITER_GUARDER,
} ziack_rc_t;

typedef enum __ZIACK_BOOL {
  ZIACK_TRUE = 1,
  ZIACK_FALSE = 0
} ziack_bool_t;

#define ZIACK_FLAG_FREE_ENTRY_VALUE 0x01

#define ZIACK_HINT_FILE_MAGIC_NUMBER 0xA890AA

#endif // !_ZIACK_CONFIG_H__

