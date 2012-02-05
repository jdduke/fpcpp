FunctionalProgrammingWithCPP
============================

Usage
-----

fpcpp is a header-only library; just include and forget.  All relevant functions and structures live
within the fp namespace.

Compilation
--------
fpcpp itself requires no compilation: it is header-only.  All that is required is support for a certain
subset of C++11 functionality.  It was developed with VC10 support at the minimum; variadic
templates are used where supported, as well as other C++11 features that significantly reduce
the code necessary to achieve the basic functionality.


Samples
-------

To come.

Tests
-----

Tests are also built using CMake, and are dependent on the google unit testing framework (http://code.google.com/p/googletest/) 
Several options for building are documented within CMake itself, make sure BUILD_TESTS is enabled.
These were not meant as exhaustive unit tests; they are primarily to 
  1) test compilation 
  2) verify basic functionality
  3) provide some simple examples of usage


    fpcppTest        - Contains all tests