# A REPL for leveldb
_riba_ is a command line utility to access leveldb databases. It supports most of leveldb's  operations (including batched updates and snapshots).

Currently, none of the options to open/read/write are exposed via the command line, and there is no support for custom comparator functions when opening a new database connection.

## Working with databases
There can only be a single database open at any time within the command line. Whenever a new database is created/opened, the current one is first closed, and the new database is opened. This is important to note, since if the new connection fails, the previous connection will cease to exist and must be reopened.

Since all leveldb databases are just directories, opening a database is as simple as opening a directory. If a leveldb database already exists at that directory, it is opened. Otherwise a new database (and the corresponding directory if it does not already exist) is created at that location.

## Commands
_riba_ supports all leveldb actions (type **help** for brief descriptions of each command) such as **open**, **close**, **get**, **put** and **delete**, in addition to supporting batched updates and snapshots. The **print** command prints out the whole database, while **count** lists the total number of key-value pairs within the database.

## Data types
leveldb's keys and values are just a sequence of bytes, and hence there is no implicit data type for these keys or values. The database is quite simply a mapping between these two byte sequences. However, in order to facilitate easier operations on the database, _riba_ provides both the character string and integer data types in addition to a hexadecimal byte sequence. Commands such as **get**, **put** or **delete** that require specifying either a key, a value or both can be entered as follows:

* get 'my string' - Anything within single quotes gets interpreted as a character string.
* put 'world' 96 - Any number. It gets converted into an int (whose size is dependent on the machine).
* delete 0xabcd - A hexadecimal sequence of bytes. The byte order is most-significant byte to least-significant byte. So in this example, the bytes are laid out as 0xcd in byte 0 and 0xab in byte 1.

## Batching updates
leveldb supports atomic or batch updates, where all edits in the batch are applied in-order to the database, and a failure somewhere fails the whole batch. _riba_ supports this with the **batch** and **commit** commands. The **batch** command starts a new batch and has no effect when a previous batch has already been started, but not committed yet. The **commit** command applies all edits in the batch to the database. Note that in case the commit fails, a new batch must be created once again in order to reapply all the lost edits.

## Taking snapshots
Taking a database snapshot preserves the contents of the database for all subsequent reads. A new snapshot is created with the **snap** command and automatically applies to all commands executed right after, until the snapshot is explicitly deleted via the **unsnap** command. Similar to **batch**, **snap** does nothing if a snapshot has already been created and not yet deleted within this session.

---

### Compilation
Type **make** in the download directory.

Google's [leveldb](http://code.google.com/p/leveldb/) is a dependency and needs to be downloaded and installed. leveldb headers and the library archive needs to be accessible within the compilation environment.

### Bugs and enhancements
Any bug reports, fixes, ideas, or general comments are most welcome. Please report them to the author(s).

### Authors
Ashwin Ramaswamy.

### Copyright
This software is provided "as is". The author(s) take no responsibility for any actions caused by this program on your system.

Copyright (c) 2011 ashwinr. Please see license.txt for further details.
