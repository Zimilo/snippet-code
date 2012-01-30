#ifndef __ZIACK_CONFIG_H__
#define __ZIACK_CONFIG_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>

typedef uint32_t ziack_size_t;
typedef uint32_t ziack_flag_t;
typedef int32_t  ziack_rc_t;


typedef enum __ZIACK_RC {
  ZIACK_RC_OK = 0,
  ZIACK_RC_EQUAL = 1,
  ZIACK_RC_NOT_FOUND;
} ziack_rc_t;

#endif // !_ZIACK_CONFIG_H__

