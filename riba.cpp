#include <cmath>
#include <string>
#include <cctype>
#include <vector>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include <leveldb/db.h>
#include <leveldb/write_batch.h>

#include "riba.h"

namespace {
/* A single leveldb instance */
leveldb::DB* ldb = NULL;

/* A batch of updates */
bool should_batch = false;
leveldb::WriteBatch lbatch;

/* A DB snapshot */
bool exists_snapshot = false;
leveldb::ReadOptions loptions;

/* Is this slice a string? */
bool is_string(const char* str, size_t len)
{
  if(!str) return false;
  for(size_t i=0; i<len; i++)
  {
    if( !isalpha( str[i]) )
      return false;
  }
  return true;
}

char from_hex(char hbyte)
{
  char outbyte = -1;
  
  if(isalpha(hbyte))
  {
    hbyte = tolower(hbyte);
    outbyte = 10 + (hbyte - 'a');
  }
  else if(isdigit(hbyte))
  {
    outbyte = hbyte - '0';
  }

  return outbyte;
}

// Note: The caller needs to free the underlying memory after
// using the returned slice
leveldb::Slice make_slice(const char* sdata)
{
  int i=0, len = strlen(sdata);
  
  // If the data is a string, create a string slice out of it
  for(i=0; i<len && isalpha(sdata[i]); i++);
  if(i == len)
  {
    return leveldb::Slice( strdup(sdata) );
  }

  // If the data is a hex string, create the appropriate bytes
  if(len > 2 && sdata[0] == '0' && (sdata[1] == 'x' || sdata[1] == 'X'))
  {
    int hlen = 0, j = 0;
    char* hdata;
    
    sdata += 2, len -=2; // Skip past 0x
    hlen = (int)ceil(len/2.0);

    hdata = (char*)malloc(hlen);
    memset(hdata, 0, hlen);
    for(int i=len-1; i>=0; i-=2)
    {
      char nibble_low = from_hex(sdata[i]);
      char nibble_high = (i>0) ? from_hex(sdata[i-1]) : 0;
      // If either character is not a valid hex character, bail out
      if(nibble_low == -1 || nibble_high == -1)
      {
        free(hdata);
        throw(std::runtime_error("Please enter a valid hex string"));
      }
      hdata[j++] = nibble_high*16 + nibble_low;
    }
    return leveldb::Slice(hdata, hlen);
  }

  // Else the data is just a number
  std::stringstream ss;
  int *number = (int*)malloc(sizeof(int));
  ss << sdata;
  ss >> (*number);
  return leveldb::Slice((const char*)number, sizeof(int));
}

std::string to_print(const char* stream, size_t len)
{
  static char hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7',
                           '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

  if(is_string(stream, len))
    return std::string(stream, len);

  // Treat it as a byte stream
  std::string outs("0x");
  for(int i=len-1; i>=0; i--)
  {
    char hb = stream[i];
    outs += hexmap[(hb & 0xf0) >> 4];
    outs += hexmap[hb & 0x0f];
  }
  return outs;
}

std::ostream& operator <<(std::ostream& os, leveldb::Slice slice)
{
  os << to_print(slice.data(), slice.size());
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
  assert(ldb);

  std::string value;
  leveldb::ReadOptions options = leveldb::ReadOptions();
  leveldb::Slice key_slice = make_slice(key);

  if(exists_snapshot)
  {
    options.snapshot = loptions.snapshot;
  }

  leveldb::Status s = ldb->Get(options, key_slice, &value);
  if(s.ok())
    std::cout << to_print(value.c_str(), value.size()) << std::endl;
  else
    std::cerr << s.ToString() << std::endl;

  free(const_cast<char*>(key_slice.data()));
}

void leveldb_put(const char* key, const char* value)
{
  assert(ldb);

  leveldb::Slice key_slice = make_slice(key);
  leveldb::Slice value_slice = make_slice(value);

  if(should_batch)
  {
    lbatch.Put(key_slice, value_slice);
  }
  else
  {
    leveldb::Status s = ldb->Put(leveldb::WriteOptions(), key_slice, value_slice);
    if(!s.ok())
      std::cerr << s.ToString() << std::endl;
  }

  free(const_cast<char*>(key_slice.data()));
  free(const_cast<char*>(value_slice.data()));
}

void leveldb_delete(const char* key)
{
  
}

void leveldb_start_batch()
{
  assert(ldb);
  if(should_batch) return;

  should_batch = true;
  lbatch.Clear();
}

void leveldb_commit_batch()
{
  assert(ldb);
  if(!should_batch) return;
  
  leveldb::Status s = ldb->Write(leveldb::WriteOptions(), &lbatch);
  if(!s.ok())
  {
    std::cerr << s.ToString() << ". Batch cleared" << std::endl;
  }

  should_batch = false;
  lbatch.Clear();
}

void leveldb_snap()
{
  assert(ldb);
  if(exists_snapshot) return;

  exists_snapshot = true;
  loptions.snapshot = ldb->GetSnapshot();
}

void leveldb_unsnap()
{
  assert(ldb);

  if(exists_snapshot)
  {
    ldb->ReleaseSnapshot(loptions.snapshot);
    exists_snapshot = false;
  }
}

void leveldb_print(const char* what)
{

}

void leveldb_count()
{

}
