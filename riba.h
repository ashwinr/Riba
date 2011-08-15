#ifndef __RIBA_H__
#define __RIBA_H__

void leveldb_open(const char* name);

void leveldb_close();

void leveldb_get(const char* key);

void leveldb_put(const char* key, const char* value);

void leveldb_delete(const char* key);

void leveldb_start_batch();

void leveldb_commit_batch();

void leveldb_snap();

void leveldb_unsnap();

void leveldb_print();

void leveldb_count();

void leveldb_help();

void leveldb_about();

void leveldb_exit();

#endif
