## **OSS Buils Status :computer:**

[![Build Status](https://travis-ci.com/AEduardo-png/dbHandler-sqlite3.svg?token=24Az7WhqXAJTBGASJKeQ&branch=main)](https://travis-ci.com/AEduardo-png/dbHandler-sqlite3) :heavy_check_mark:

# **Welcome to the Sqlite3 Handler Repository! :notebook:**

## Announcements :clipboard:

#### :pushpin: Actual release version

This is the first release of this project (1.0.X)

#### :pushpin: Future Additions

As only the so called "basic functionality" of sqlite3 is implemented at the moment, all the advanced methods will be included in the future.


## Description :scroll:

This project is based, and depends on, the sqlite3 official libraries. Check the [**SQLite3 website**](https://www.sqlite.org/index.html) for more information about them, and how they can be installed on your system.

The objective of this project is to provide an straight-forward, parameterized approach on sqlite3 database management, using the **C++** interface provided from sqlite3. The idea is to apply the main functionality of the database management without the need of writing a query in plain text.

With this project you receive a set of two .hpp libraries, consisting of:
1\.  _query.hpp_: An aggregate of sqlite3 query commands and clauses, for you to use on your queries definition.
2\.  _handler.hpp_: Contains a handler class which can execute it's different methods on the database selected.

The doxygen generated documentation of the software is included in the [docs/refman.pdf](https://github.com/AEduardo-png/dbHandler-sqlite3/tree/cmake_installer/docs/refman.pdf) and an example for each of the methods available at the moment can be seen in the [examples folder](https://github.com/AEduardo-png/dbHandler-sqlite3/tree/cmake_installer/examples).

## How to install :floppy_disk:

## Getting Started :book:


## :checkered_flag: Final Notes :checkered_flag:
The provided libraries can operate in parallel or separately from the sqlite3 C++ official interface without any issue.
