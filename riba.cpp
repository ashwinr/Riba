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

void clear_db_state()
{
  if(!ldb) return;

  // Clear any pending batches
  if(should_batch)
  {
    should_batch = false;
    lbatch.Clear();
  }
  
  // Clear any snapshots
  if(exists_snapshot)
  {
    exists_snapshot = false;
    ldb->ReleaseSnapshot(loptions.snapshot);
  }
}

bool get_confirmation(const std::string& question,
                      const std::string& yes,
                      const std::string& no)
{
  std::string answer;
  std::cout << question;
  std::cin >> answer;
  return (answer == yes);
}

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
      // If neither character is a valid hex character, bail out
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
    clear_db_state();
    delete ldb;
    ldb = NULL;
  }
}

void leveldb_get(const char* key)
{
  if(!ldb) return;

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
  if(!ldb) return;

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
  if(!ldb) return;
  
  leveldb::Slice key_slice = make_slice(key);
  
  if(should_batch)
  {
    lbatch.Delete(key_slice);
    return;
  }
  else
  {
    leveldb::Status s = ldb->Delete(leveldb::WriteOptions(), key_slice);
    if(!s.ok())
      std::cerr << s.ToString() << std::endl;
  }
  
  free(const_cast<char*>(key_slice.data()));
}

void leveldb_start_batch()
{
  if(!ldb || should_batch) return;

  should_batch = true;
  lbatch.Clear();
}

void leveldb_commit_batch()
{
  if(!ldb || !should_batch) return;
  
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
  if(!ldb || exists_snapshot) return;

  exists_snapshot = true;
  loptions.snapshot = ldb->GetSnapshot();
}

void leveldb_unsnap()
{
  if(!ldb) return;

  if(exists_snapshot)
  {
    ldb->ReleaseSnapshot(loptions.snapshot);
    exists_snapshot = false;
  }
}

void leveldb_print()
{
  if(!ldb) return;

  leveldb::Iterator* it = ldb->NewIterator(leveldb::ReadOptions());
  for(it->SeekToFirst(); it->Valid(); it->Next())
  {
    std::cout << it->key() << " : " <<  it->value() << std::endl;
  }
  delete it;
}

// Currently an inefficient way of printing out the total number of elements
// in the database, until a more efficient method can be found
void leveldb_count()
{
  if(!ldb) return;

  size_t count = 0;
  leveldb::Iterator* it = ldb->NewIterator(leveldb::ReadOptions());

  for(it->SeekToFirst(); it->Valid(); it->Next(), count++);
  std::cout << count << std::endl;
  delete it;
}

void leveldb_exit(void)
{
  std::string exitString("A batch with uncommitted updates might exist. Really exit? [y/n] ");
  if(should_batch && get_confirmation(exitString, "y", "n")==false)
  {
    return;
  }

  leveldb_close();
  exit(0);
}

void leveldb_help(void)
{
  std::cout << "The following list of commands are supported in riba:" << std::endl;
  std::cout << "open 'dir'        - Open/Create a database at the specified directory" << std::endl;
  std::cout << "close             - Close the database" << std::endl;
  std::cout << "get <key>         - Get value for <key>" << std::endl;
  std::cout << "put <key> <value> - Insert key-value pair" << std::endl;
  std::cout << "delete <key>      - Delete value at <key>" << std::endl;
  std::cout << "batch             - Start a new batch" << std::endl;
  std::cout << "commit            - Commit the batch of operations" << std::endl;
  std::cout << "snap              - Take a snapshot of the database" << std::endl;
  std::cout << "unsnap            - Delete the snapshot" << std::endl;
  std::cout << "print             - Print the contents of the database" << std::endl;
  std::cout << "count             - Print the number of elements in the database" << std::endl;
  std::cout << "help              - Print this help" << std::endl;
  std::cout << "about             - About riba" << std::endl;
  std::cout << "exit              - Exit" << std::endl;
}

void leveldb_about(void)
{
  std::cout <<
"\n                                  riba - A REPL for leveldb"
"\n"
"\n"
"             ...                        M                                      \n"
"           .M.M                     M MM M          MMMMMMMMM. .               \n"
"          MMMM.                   .  M M. M     MMM..       ...MMMM            \n"
"         M MMM                   . M M  M..M MM                  ..MMM         \n"
"        MMM..M                 M.M  M.M. M.M.         MMMM..         MMM       \n"
"MMM.M  M .MMMM                ..M.M M. MMM         MMMMMMMMMM.         MM.     \n"
"M M M MM   M.M              .M  M.M  .MM         . MMMMMMMM   M         .MM    \n"
"  .MM M..MM..M                M.M .M M .         M   MMMMM     .          MM   \n"
"    .MM  M.  M          . .M MM . .MM            M             .          .M   \n"
"      M      M          M  M  M MMN               M.          M            MM  \n"
"       M     .M      .  M..M. MMM    M.             M.      MM             .MM \n"
"              .M      . M..M.M .     M     M         ...M.                  MM.\n"
"         .M     M.  M   M. M          M.   M                            . MM   \n"
"           M       MMMMM       M   .         .         MMM .   . . MMMMM       \n"
"           .M                 M    M      M              MMMMMMM               \n"
"             M           M.    M.   .    M                ..MMMM               \n"
"             .M.         M                M                    MM              \n"
"               MM         .  M                                   . MMM.        \n"
"                 M           M.          .M..MMMMM                       MMMM  \n"
"              MMMM M.         .    M      M MMM . M                        M   \n"
"              M  .M MM                    M....   .                     .M     \n"
"                .M MM .MM.                  MMMM..                     M       \n"
"                M       .MMM                                      .MMM.        \n"
"                          .MMMM                                MMMM            \n"
"                               MMMMMM                 ..MMMMMM                 \n"
"                            MMM..M. MMMMMMMMMMMMMMMMMMMMM                      \n"
"                             MMM..M M M. ..  ..                                \n"
"                                MM.MM.                                         \n"
"                               MM                                              \n";
}
