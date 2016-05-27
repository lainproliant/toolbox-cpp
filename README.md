Lain's Personal C++ Toolbox
===========================

A header-only meta library with tools to make C++ life more fun.

All of the tools in this library require C++11, some require C++14.

Included among these are the following sets of tools:

+ Custom Tools
  + `<lain/algorithms.h>`: Convenient wrappers around STL algorithms for functional transformation of containers.
  + `<lain/exception.h>`: A sensible Exception base class.
  + `<lain/getopt.h>`: A really basic simplified command line option parser.
    + Requires C++14
  + `<lain/maps.h>`: Convenience functions for STL map types.
  + `<lain/mmap.h>`: Syntactic static initialization of multimaps.
  + `<lain/settings.h>`: A wrapper around picojson providing an easy to use JSON config file interface.
  + `<lain/string.h>`: Some useful functions built around strings and standard library containers.
  + `<lain/testing.h>`: A minimalistic C++11 functional unit testing framework used by this library.

+ Submodules
  + **apathy**: C++ path manipulation.
    + https://github.com/dlecocq/apathy.git
  + **Optional**: Optional/nullable objects for C++14
	 + https://github.com/akrzemi1/Optional
  + **picojson**: a C++ JSON parser / serializer by Kazuho Oku
    + https://travis-ci.org/kazuho/picojson
  + **spdlog**: Super fast C++ logging library with multithreading support.
	 + https://github.com/gabime/spdlog.git
  + **tinyformat**: A minimal type safe printf() replacement by Chris Foster
    + https://github.com/c42f/tinyformat

Submodule Notes
---------------

Some of the directories under `include` are Git submodules.  When you clone this project, you'll notice that these directories are empty.  To pull all of these down, run the **`submodules.sh`** script at the root of the project.

For more information on Git submodules, see
http://git-scm.com/book/en/Git-Tools-Submodules

