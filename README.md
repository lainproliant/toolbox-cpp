Lain's Personal C++ Toolbox
===========================

A header-only meta library containing many custom tools and other
header-only libraries which make C++ life easier.  Included among these
are the following sets of tools:

+ Custom Tools
  + `<lain/exception.h>`: A sensible Exception base class.
  + `<lain/getopt.h>`: A really basic simplified command line option parser.
  + `<lain/string.h>`: Some useful functions built around strings and standard library containers.  Some highlights:
    + `str::join`: Join the elements of an iterable into a string with the given delimiter.
    + `str::split`: Split the elements of a string into a list of strings using the given delimiter.
    + `str::repr`: Print structural representations of maps and lists, useful for debugging.

+ Submodules
  + **picojson**: a C++ JSON parser / serializer by Kazuho Oku
    + https://travis-ci.org/kazuho/picojson

Submodule Notes
---------------

Some of the directories under `include` are Git submodules.  When you clone this project, you'll notice that these directories are empty.  To pull all of these down, run the **`submodules.sh`** script at the root of the project.

For more information on Git submodules, see 
http://git-scm.com/book/en/Git-Tools-Submodules
