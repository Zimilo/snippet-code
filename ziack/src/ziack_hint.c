#include "ziack_hint.h"
#include "ziack_hashtable.h"
#include "ziack_hashtable_iter.h"
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
			    ziack_flag_t  flags)
{
  int fd = open(file_name, O_RDONLY);
  if (-1 == fd) return NULL;
  ziack_hint_t *hint = (ziack_hint_t *)ziack_hint_create(flags);
  do {
    off_t file_size = lseek(fd, 0, SEEK_END);
    if (-1 == file_size || file_size < sizeof(uint64_t) << 1) break;
    lseek(fd, 0, SEEK_SET);
    uint64_t magic_number;
    if (pread(fd, &magic_number, sizeof(magic_number), 0) == -1) break;
    if (magic_number != ZIACK_HINT_FILE_MAGIC_NUMBER) break;
    if (pread(fd, &magic_number, sizeof(magic_number), file_size - sizeof(magic_number)) == -1) break;
    if (magic_number != ZIACK_HINT_FILE_MAGIC_NUMBER) break;
    ziack_size_t count;
    off_t pos = sizeof(uint64_t);
    pread(fd, &count, sizeof(count), pos);
    pos += sizeof(count);
    ziack_size_t i;
    ziack_size_t key_size, version_count;
    char *k = (char *)ziack_calloc(1, 256);
    for (i = 0; i < count; ++i) {
      pread(fd, &key_size, sizeof(key_size), pos);
      pos += sizeof(key_size);
      if (key_size > 256) k = (char *)ziack_realloc(k, key_size);
      pread(fd, k, key_size, pos);
      pos += key_size;
      ziack_hint_key_t *key = ziack_hint_key_create(k, key_size);
      ziack_hint_value_t *v = ziack_hint_value_create();
      pread(fd, &v->base, sizeof(v->base), pos);
      pos += sizeof(v->base);
      pread(fd, &version_count, sizeof(version_count), pos);
      pos += sizeof(version_count);
      ziack_size_t j;
      for (j = 0; j < version_count; ++j) {
	ziack_hint_version_t *version = (ziack_hint_version_t *)ziack_calloc(1, sizeof(ziack_hint_version_t));
	pread(fd, (char *)version, sizeof(ziack_hint_version_t), pos);
	pos += sizeof(ziack_hint_version_t);
	ziack_vector_push(v->versions, version);
      }
      ziack_hint_add(hint, key, v);
      ziack_hint_key_destroy(key);
    }
    ziack_free(k);
  } while (0);
  close(fd);
  return hint;
}

ziack_rc_t
ziack_hint_destroy(ziack_hint_t *hint)
{
  ziack_hashtable_destroy(hint->hints, ziack_hint_value_free_func);
  ziack_free(hint);
}

ziack_rc_t 
ziack_hint_dump_to_file(ziack_hint_t *hint, 
		     const char   *file_name) 
{
  int fd = open(file_name, O_WRONLY | O_CREAT, 0666);
  if (-1 == fd) return ZIACK_RC_FILE_ERROR;
  uint64_t magic_number = ZIACK_HINT_FILE_MAGIC_NUMBER;
  write(fd, &magic_number, sizeof(magic_number));
  ziack_size_t count = ziack_hashtable_count(hint->hints);
  write(fd, &count, sizeof(count));
  ziack_hashtable_iter_t *it = ziack_hashtable_iter_create(hint->hints);
  ziack_hint_value_t *v = NULL;
  ziack_hashtable_key_t *k = NULL;
  ziack_hint_version_t *version = NULL;
  while (ziack_hashtable_iter_valid(it)) {
    k = ziack_hashtable_iter_get_key(it);
    v = (ziack_hint_value_t *)ziack_hashtable_iter_get_value(it);
    write(fd, &k->key_size, sizeof(k->key_size));
    write(fd, k->key, k->key_size);
    write(fd, &v->base, sizeof(v->base));
    ziack_size_t version_count = ziack_vector_count(v->versions);
    write(fd, &version_count, sizeof(version_count));
    ziack_size_t i = 0;
    for (; i < version_count; ++i) {
      version = ziack_vector_index(v->versions, i);
      write(fd, version, sizeof(ziack_hint_version_t));
    }
    if (ziack_hashtable_iter_next(it) == ZIACK_RC_ITER_GUARDER) break;
  }
  ziack_hashtable_iter_destroy(it);
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
    ziack_hashtable_key_destroy(hk);
  }
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


#if 1
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

  ziack_hint_dump_to_file(hint, "./hints");

  ziack_hint_destroy(hint);
  hint = ziack_hint_create_from_file("./hints", 0);

  v = ziack_hint_lookup_version(hint, key, 0);
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
  ziack_hint_destroy(hint);
}
#endif 
