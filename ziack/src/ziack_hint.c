#include "ziack_hint.h"
#include "ziack_hashtable.h"
#include "ziack_memory.h"
#include "ziack_tests.h"
#include "ziack_vector.h"
#include <time.h>
#include <fcntl.h>

ziack_hint_key_t *
ziack_hint_key_create(void        *key,
		      ziack_size_t key_size)
{
  ziack_hint_key_t *hk = (ziack_hint_key_t *)ziack_calloc(1, sizeof(ziack_hint_key_t) + key_size);
  if (NULL == hk) return NULL;
  hk->key_size = key_size;
  memcpy(hk->key, key, key_size);
  return hk;
}

ziack_rc_t
ziack_hint_key_destroy(ziack_hint_key_t *key)
{
  ziack_free(key);
  return ZIACK_RC_OK;
}

ziack_hint_value_t *
ziack_hint_value_create()
{
  ziack_hint_value_t *value = (ziack_hint_value_t *)ziack_calloc(1, sizeof(ziack_hint_value_t));
  if (NULL == value) return NULL;
  value->base = 0;
  value->versions = ziack_vector_create(0);
  if (NULL == value->versions) return NULL;
  return value;
}

ziack_rc_t
ziack_hint_value_destroy(ziack_hint_value_t *value)
{
  ziack_vector_destroy(value->versions, ziack_free);
  ziack_free(value);
  return ZIACK_RC_OK;
}

void
ziack_hint_value_free_func(void *v)
{
  ziack_hint_value_destroy((ziack_hint_value_t *)v);
}

ziack_hint_version_t *
ziack_hint_version_create()
{
  ziack_hint_version_t *version = (ziack_hint_version_t *)ziack_calloc(1, sizeof(ziack_hint_version_t));
  return (version != NULL) ? version : NULL;
}

ziack_hint_t *
ziack_hint_create(ziack_flag_t flags)
{
  ziack_hint_t *hint = (ziack_hint_t *)ziack_calloc(1, sizeof(ziack_hint_t));
  if (NULL == hint) return NULL;
  hint->hints = ziack_hashtable_create(16, ziack_hashtable_hashfunc, ziack_hashtable_equalfunc);
  if (NULL == hint->hints) return NULL;
  return hint;
}

ziack_hint_t *
ziack_hint_create_from_file(const char   *file_name,
			    ziack_flag_t flags)
{
  //@todo
  return NULL;
}

ziack_rc_t
ziack_hint_destroy(ziack_hint_t *hint)
{
  ziack_hashtable_destroy(hint->hints, ziack_hint_value_free_func);
  ziack_free(hint);
}

ziack_rc_t 
ziack_hint_dump2file(ziack_hint_t *hint, 
		     const char   *file_name) 
{
  int fd = open(file_name, O_APPEND | O_WRONLY | O_CREAT, 0666);
  if (-1 == fd) return ZIACK_RC_FILE_ERROR;
  uint64_t magic_number = ZIACK_HINT_FILE_MAGIC_NUMBER;
  write(fd, &magic_number, sizeof(magic_number));
  ziack_size_t count = ziack_hashtable_count(hint->hints);
  write(fd, &count, sizeof(count));
  //@todo iterator the hashtable
  write(fd, &magic_number, sizeof(magic_number));
  close(fd);
  return ZIACK_RC_OK;
}

ziack_rc_t 
ziack_hint_add_version(ziack_hint_t         *hint, 
		       ziack_hint_key_t     *hint_key, 
		       ziack_hint_version_t *version)
{
  ziack_hashtable_key_t *hk = ziack_hashtable_key_create(hint_key->key, hint_key->key_size);
  void *value = ziack_hashtable_lookup(hint->hints, hk);
  ziack_hint_value_t *hint_value;
  if (NULL == value) {
    hint_value = ziack_hint_value_create();
    ziack_hashtable_add(hint->hints, hk, hint_value);
  } else {
    hint_value = (ziack_hint_value_t *)value;    
  }
  ziack_vector_push(hint_value->versions, version);
  return ZIACK_RC_OK;
}

ziack_hint_version_t *
ziack_hint_lookup_version(ziack_hint_t     *hint,
			  ziack_hint_key_t *key,
			  ziack_size_t      vidx)
{
  ziack_hint_value_t *v = ziack_hint_lookup(hint, key);
  if (v == NULL) return NULL;
  ziack_size_t count = ziack_vector_count(v->versions);
  if (count == 0 || (vidx >= v->base + count) || vidx < v->base) return NULL;
  ziack_vector_t *versions = v->versions;
  return ziack_vector_index(versions, vidx - v->base);
}

ziack_rc_t
ziack_hint_add(ziack_hint_t       *hint,
	       ziack_hint_key_t   *key,
	       ziack_hint_value_t *value)
{
  ziack_hashtable_key_t *hk = ziack_hashtable_key_create(key->key, key->key_size);
  ziack_rc_t rc = ZIACK_RC_OK;
  ziack_hint_value_t *v = (ziack_hint_value_t *)ziack_hashtable_lookup(hint->hints, hk);
  if (NULL == v) {
    ziack_hashtable_add(hint->hints, hk, value);
  } else {
    rc = ZIACK_RC_KEY_EXISTS;
  }
  ziack_hashtable_key_destroy(hk);
  return rc;
}

ziack_rc_t
ziack_hint_delete(ziack_hint_t     *hint,
		  ziack_hint_key_t *key) 
{
  ziack_hashtable_key_t *hk = ziack_hashtable_key_create(key->key, key->key_size);
  ziack_rc_t rc = ZIACK_RC_OK;
  ziack_hint_value_t *v = (ziack_hint_value_t *)ziack_hashtable_delete(hint->hints, hk);
  if (NULL == v)
    rc = ZIACK_RC_NOT_FOUND;
  else
    ziack_free(v);
  ziack_hashtable_key_destroy(hk);
  return rc;
}

ziack_hint_value_t *
ziack_hint_lookup(ziack_hint_t     *hint,
		  ziack_hint_key_t *key) 
{
  ziack_hashtable_key_t *hk = ziack_hashtable_key_create(key->key, key->key_size);
  ziack_rc_t rc = ZIACK_RC_OK;
  ziack_hint_value_t *value = (ziack_hint_value_t *)ziack_hashtable_lookup(hint->hints, hk);
  ziack_hashtable_key_destroy(hk);
  return value;
}

ziack_rc_t
ziack_hint_update(ziack_hint_t       *hint,
		  ziack_hint_key_t   *key,
		  ziack_hint_value_t *value) 
{
  ziack_hashtable_key_t *hk = ziack_hashtable_key_create(key->key, key->key_size);
  ziack_rc_t rc = ziack_hashtable_update(hint->hints, hk, value);
  ziack_hashtable_key_destroy(hk);
  return rc;
}


#if 0
int
main(int argc, char **argv) 
{
  ziack_hint_t *hint = ziack_hint_create(0);
  char *hint_key = "HintDemoKey";
  ziack_hint_key_t *key = ziack_hint_key_create(hint_key, strlen(hint_key));
  ziack_hint_value_t *value = ziack_hint_value_create();
  ziack_hint_add(hint, key, value);
  ziack_hint_version_t *version = ziack_hint_version_create();
  time_t t1 = time(NULL);
  version->ts = t1;
  version->fidx = 0;
  version->offset = 0;
  version->size = 1000;
  
  ziack_hint_version_t *version2 = ziack_hint_version_create();
  time_t t2 = time(NULL);
  version2->ts = t2;
  version2->fidx = 0;
  version2->offset = 1000;
  version2->size = 2000;
  
  ziack_hint_add_version(hint, key, version);
  ziack_hint_add_version(hint, key, version2);

  ziack_hint_version_t *v = ziack_hint_lookup_version(hint, key, 0);
  ziack_assert(v->ts == t1);
  ziack_assert(v->fidx == 0);
  ziack_assert(v->offset == 0);
  ziack_assert(v->size == 1000);

  v = ziack_hint_lookup_version(hint, key, 1);
  ziack_assert(v->ts == t2);
  ziack_assert(v->fidx == 0);
  ziack_assert(v->offset == 1000);
  ziack_assert(v->size == 2000);

  v = ziack_hint_lookup_version(hint, key, 10);
  ziack_assert(v == NULL);

  ziack_hint_key_destroy(key);
  ziack_hint_dump2file(hint, "./hints");

  ziack_hint_destroy(hint);
  hint = ziack_hint_create_from_file("./hints", 0);
  if (hint != NULL) {
    ziack_hint_destroy(hint);
  }
}
#endif 
