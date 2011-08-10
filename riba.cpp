#include <iostream>
#include <string>
#include <cctype>
#include <leveldb/db.h>
#include "riba.h"

namespace {
/* A single leveldb instance */
leveldb::DB* ldb = NULL;

/* Is this slice a string? */
bool isString(leveldb::Slice* slice)
{
  if(!slice || slice->empty()) return false;
  for(size_t i=0; i<slice->size(); i++)
  {
    if( !isascii( (*slice)[i]) )
      return false;
  }
  return true;
}

std::ostream& operator <<(std::ostream& os, leveldb::Slice slice)
{
  return os;
}

}

void leveldb_open(const char* name)
{
  leveldb::Options options;
  options.create_if_missing = true;

  // Close the previous running instance
  if(ldb)
  {
    leveldb_close();
  }

  leveldb::Status s = leveldb::DB::Open(options, name, &ldb);

  if(!s.ok())
  {
    std::cerr << "open: " << s.ToString() << std::endl;
    ldb = NULL;
  }
}

void leveldb_close()
{
  if(ldb)
  {
    delete ldb;
    ldb = NULL;
  }
}

void leveldb_get(const char* key)
{
  leveldb::Slice skey;
  std::string value;
  leveldb::Status s = ldb->Get(leveldb::ReadOptions(), skey, &value);
  if(s.ok())
  {
    
  }
  else
  {
    std::cerr << "read: " << s.ToString() << " : " << skey << std::endl;
  }
}

void leveldb_put(const char* key, const char* value)
{
  std::cout << "leveldb_put: " << key << " " << value << std::endl;
}

void leveldb_start_batch()
{
  std::cout << "leveldb_batch" << std::endl;
}

void leveldb_commit_batch()
{
  std::cout << "leveldb_commit" << std::endl;
}

void leveldb_snap()
{
  std::cout << "leveldb_snap" << std::endl;
}

void leveldb_unsnap()
{
  std::cout << "leveldb_unsnap" << std::endl;
}

void leveldb_print(const char* what)
{
  std::cout << "leveldb_print: " << what << std::endl;
}
