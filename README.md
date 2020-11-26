## **OSS Buils Status :computer:**

[![Build Status](https://travis-ci.com/AEduardo-dev/Sqlite3Utils.svg?token=24Az7WhqXAJTBGASJKeQ&branch=main)](https://travis-ci.com/AEduardo-dev/Sqlite3Utils)

[![Build status](https://ci.appveyor.com/api/projects/status/9u3gysvrsuhc8c46?svg=true)](https://ci.appveyor.com/project/AEduardo-png/sqlite3utils)


# **Welcome to the Sqlite3 Handler Repository! :notebook:**

## Announcements :clipboard:

#### :pushpin: Actual release version

This is the first release of this project (1.0.X)

#### :pushpin: Future Additions :construction:

As only the so called "basic functionality" of sqlite3 is implemented at the moment, all the advanced methods will be included in the future.

The terms "basic" and "advanced" have been extracted from the [Tutorials Point page](https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm) referenced to the C++ interface of sqlite3.

Also, the inclusion of [sqlite3_bind()](https://www.sqlite.org/c3ref/bind_blob.html) for internal parameters, and [sqlite3_reset()](https://www.sqlite.org/c3ref/reset.html) for the usage of commands without needing to prepare them again, is planned.

#### :pushpin: Integration :warning:

Windows integration is yet to be tested and is not guaranteed to work. Check [Appveyor build status](https://ci.appveyor.com/project/AEduardo-png/sqlite3utils.

## Description :scroll:

This project is based, and depends on, the sqlite3 official libraries. Check the [**SQLite3 website**](https://www.sqlite.org/index.html) for more information about them, and how they can be installed on your system.

The objective of this project is to provide an straight-forward, parameterized approach on sqlite3 database management, using the **C++** interface provided from sqlite3. The idea is to apply the main functionality of the database management without the need of writing a query in plain text.

With this project you receive a set of two .hpp libraries, consisting of:

1.  _query.hpp_: An aggregate of sqlite3 query commands and clauses, for you to use on your queries definition. It uses different name spaces to differentiate between command, clauses and conditions.

2.  _handler.hpp_: Contains a handler class which can execute it's different methods on the database selected.

The doxygen generated documentation of the software is included in the [docs/refman.pdf](https://github.com/AEduardo-png/Sqlite3Utils/tree/cmake_installer/docs/refman.pdf), containing the list of variables available in query.hpp, and the description of all the methods in handler.hpp. An html version of the documentation is also available in the [docs](https://github.com/AEduardo-png/Sqlite3Utils/tree/cmake_installer/docs) folder.

Each of the methods available at the moment have a pseudo code example that can be seen in the [examples folder](https://github.com/AEduardo-png/Sqlite3Utils/tree/cmake_installer/examples).

#### Custom method executeQuery() :information_source:

In case the use of any query not currently supported by the methods implemented, executeQuery() can operate with any (correctly built) sqlite3 query. Check the [docs](https://github.com/AEduardo-png/Sqlite3Utils/tree/cmake_installer/docs) for more information about how to use this method, the arguments it can take, and the information it can provide.


## Installation :floppy_disk:

**IMPORTANT NOTE :memo: This process has not been completely tested on windows yet.**

The installation is managed with cmake, which you should already have installed on your system. The minimum supported version is 3.11, but some functionalities need at least the 3.18 one. If you try to execute the installer without having the proper version the process will abort, so nothing will break on your machine.

For installation it is recommended for you to create a _"build/"_ directory inside of the project source folder, then from there run "_cmake &lt;options> .._". After that is done, execute "_cmake --build ._", and finally "_cmake --install ._", where you may want to add an installation prefix. Please take a look at [this tutorial](https://cmake.org/cmake/help/v3.19/manual/cmake.1.html) for more information about the native options you can add to each instruction.

Now lets take a look at what does the installer do in the default mode and the different options that can be added to apply more changes.

#### Generation flags :wrench:

When calling cmake .. for generating the build files, there are some option flags that you can set for customizing your installation.

-   **-DINSTALL_DEPENDENCIES=ON** - This will ask the installer to install sqlite3 in your system if you do not have it already. If sqlite3 is found this option will not change the generating process. **NOTE: this process requires some cmake functionalities only available from the 3.18 version. If you do not have this or a later version, the generation process will fail. On windows this will also require nmake.**

-   **-DUNIT_TESTS=ON** - This option downloads the [google test framework](https://github.com/google/googletest) locally, and uses it's source code to build a test suite, that could be executed using the ctest executable generated inside of the build directory. This tests ensure that the methods included in the handler library work as expected, but they are not necessary for you to build.If you do so, the [tests.cpp](https://github.com/AEduardo-png/Sqlite3Utils/blob/cmake_installer/tests/tests.cpp) file will also be installed on your system, so it may be interesting if you would like to develop this project further on.

-   **-DINSTALL_EXAMPLES=ON** - If this is set to ON, the examples folder will be installed to the installation prefix. This examples are used as documentation, as well as a simple introduction to the methods available inside of the handler library.

-   **-DINSTALL_DOCS=ON** - The documentation is generated both in pdf and in html format. Both of them will be installed if this option is set to ON. This README will also be installed with them.

#### Installation Options :cd:

-   **System installation**: This installs the libraries to the system, using the location in which all C++ includes are stored. This way you will be able to use this libraries as #include &lt;library_name.hpp> instead of using the relative or absolute path of it using #include "&lt;path to library>/library_name.hpp".

-   **Prefix Installation**: The libraries and their source files will be installed at the given path, at an **include** and **lib** folders respectively. This installation could be interesting if you would like to use them as a part of other project without having them on your system.

For both of them, all the other options you may have activated will be installed at the provided path.




## :checkered_flag: Final Notes :checkered_flag:

The installation folder is include/sqliteutils-\<version_number\>/library files. So your includes should be #include <sqliteutils-x.x.x/library_name>

The provided libraries can operate in parallel or separately from the sqlite3 C++ official interface without any issue.
