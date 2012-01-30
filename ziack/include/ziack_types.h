#ifndef __ZIACK_HINT_TYPES_H__
#define __ZIACK_HINT_TYPES_H__

#include "ziack_config.h"

typedef struct __ZIACK_RECORD_KEY {
  uint32_t key_size;
  char     key[0];
} ziack_record_key_t;

typedef struct __ZIACK_RECORD_DATA {
  uint32_t data_size;
  char     data[0];
} ziack_record_data_t;

typedef struct __ZIACK_RECORD {
  uint64_t            magic_number;        // for recovery 
  char                cookie[64];          // brute force lookups
  ziack_record_key_t  key;                 
  uint32_t            flags;               // extra flag like delete flag and etc
  ziack_record_data_t data;                
  uint64_t            footer_magic_number; // footer magic number for recovery
  uint64_t            checksum;            
  char                padding[0];          // bytes aligned to 8 bytes
} ziack_record_t;


/* BEGIN HASHTABLE TYPES DEFINITIONS */
typedef struct __ZIACK_HASHTABLE_KEY {
  ziack_size_t  key_size;
  void         *key;
} ziack_hashtable_key_t;

typedef struct __ZIACK_HASHTABLE_VALUE {
  ziack_size_t  value_size;
  void         *value;
} ziack_hashtable_value_t;

typedef struct __ZIACK_HASHTABLE_ENTRY {
  ziack_hashtable_key_t *key;
  ziack_hashtable_value_t *value;
  uint32_t      hash;
  struct __ZIACK_HASHTABLE_ENTRY *next;
} ziack_hashtable_entry_t;

typedef struct __ZIACK_HASHTABLE {
  ziack_hashtable_entry_t **buckets;
  ziack_size_t bucket_count;
  ziack_size_t entry_count;
  uint32_t (*hashfunc)(void *key, ziack_size_t key_size);
  int (*key_equal_func)(void *key, ziack_size_t key_size, void *value, ziack_size_t value_size);
} ziack_hashtable_t;

/* END HASHTABLE TYPES DEFINITIONS */


/* BEGIN HINT TYPES DEFINITIONS */
typedef struct __ZIACK_HINT_VALUE {
  uint32_t file_id; // actual file index
  uint64_t offset; 
  uint32_t size;
} ziack_hint_value_t;

typedef struct __ZIACK_HINT {
  ziack_hashtable_t *data;
} ziack_hint_t;

typedef struct __ZIACK_HINT_KEY {
  ziack_size_t key_size;
  void         *key;
} ziack_hint_key_t;
/* END HINT TYPES DEFINITIONS */

#endif // __ZIACK_HINT_TYPES_H__
