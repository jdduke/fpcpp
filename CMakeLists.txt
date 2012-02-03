cmake_minimum_required (VERSION 2.6)
project(FPCPP)

set(CMAKE_DEBUG_POSTFIX "d")

# Treat warnings as errors cmake option
set(TREAT_WARNINGS_AS_ERRORS TRUE CACHE BOOL "Treat warnings as errors")
set(ENABLE_EXPERIMENTAL_FC TRUE  CACHE BOOL "Enable experimental function composition routines.")
set(ENABLE_VARIADIC_FC     FALSE CACHE BOOL "Enable variadic template use.")
SET(BUILD_SAMPLES          TRUE  CACHE BOOL "Build samples")
set(BUILD_TESTS            FALSE CACHE BOOL "Build unit tests")

if(ENABLE_EXPERIMENTAL_FC)
  add_definitions( "-DFC_EXPERIMENTAL" )
endif()
if(ENABLE_VARIADIC_FC)
  add_definitions( "-DFC_VARIADIC" )
endif()

if(MSVC10)
  set(FPCPP_SUFFIX _vc10)
elseif(MSVC11)
  set(FPCPP_SUFFIX _vc11)
else()
  set(FPCPP_SUFFIX)
endif()

if(CMAKE_SIZEOF_VOID_P MATCHES 4)
  set(FPCPP_SIZE_TYPE x86)
else()
  set(FPCPP_SIZE_TYPE x64)
endif()

set(FPCPP_BINARY_PATH ${CMAKE_HOME_DIRECTORY}/bin/${FPCPP_SIZE_TYPE}${FPCPP_SUFFIX})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${FPCPP_BINARY_PATH}
    CACHE PATH
    "Single Directory for all Executables.")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${FPCPP_BINARY_PATH}
    CACHE PATH
    "Single Directory for all Libraries")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${FPCPP_LIBRARY_PATH}
    CACHE PATH
    "Single Directory for all static libraries.")

if(UNIX)
  if(TREAT_WARNINGS_AS_ERRORS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
  endif()
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++0x -Wno-deprecated -fpermissive")
    add_definitions( "-DDEBUG" )
  elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 -Wall -std=c++0x -fno-omit-frame-pointer -fpermissive")
  elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithProfile")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -g -O3 -Wall -std=c++0x -fno-omit-frame-pointer -fpermissive")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg -fPIC")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -pg")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -std=c++0x -fpermissive -Wno-unused-variable -Wno-write-strings -fno-strict-aliasing  -Wno-parentheses -Wno-deprecated -fno-omit-frame-pointer")
  endif()
endif()

if(WIN32)
  SET(LINK_STATIC_RUNTIME OFF CACHE BOOL "Link statically against C++ runtime")
  if(TREAT_WARNINGS_AS_ERRORS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /WX")
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DNOMINMAX /MP /W3")
  if(MSVC10 OR MSVC11)
    SET(ITERATOR_DEBUG TRUE CACHE BOOL "Use iterator debug level 2")
    #add_definitions("-D_ITERATOR_DEBUG_LEVEL=0")
    if (ITERATOR_DEBUG)
      set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D_ITERATOR_DEBUG_LEVEL=2")
    else()
      set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D_ITERATOR_DEBUG_LEVEL=0")
    endif()
    #add_definitions("-D_ITERATOR_DEBUG_LEVEL=0")
  endif()
  # Extra speed optimisation options
  set(MSVC_EXTRA_OPTIMIZE ON CACHE BOOL "Use extra optimization flags in release builds")
  if(MSVC_EXTRA_OPTIMIZE)
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Oi /Ot /Oy /GL /GS- /fp:fast")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Oi /Ot /Oy /GL /GS- /fp:fast")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /OPT:REF /OPT:ICF /LTCG")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS_RELEASE} /OPT:REF /OPT:ICF /LTCG")
  endif()
  # Even more speed optimisations
  set(MSVC_OPTIMIZE_SSE2 ON CACHE BOOL "Use SSE2 instruction set in release builds")
  if(MSVC_OPTIMIZE_SSE2)
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /arch:SSE2")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /arch:SSE2")
  endif()
  #ADD_DEFINITIONS("-D_SCL_SECURE_NO_WARNINGS")
  #ADD_DEFINITIONS("-D_CRT_SECURE_NO_WARNINGS")
  if(LINK_STATIC_RUNTIME AND NOT BUILD_SHARED_LIBS)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /NODEFAULTLIB:msvcrt /NODEFAULTLIB:msvcrtd")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /NODEFAULTLIB:msvcrt /NODEFAULTLIB:msvcrtd")
  endif()
endif()

include_directories(${FPCPP_SOURCE_DIR}/include)
include_directories(${FPCPP_SOURCE_DIR}/externals/fc/include)
include_directories(${FPCPP_SOURCE_DIR}/externals)

set(TTHREAD_LIB tthread)

if(BUILD_SAMPLES OR BUILD_TESTS)
  add_subdirectory(externals/tthread)
  include_directories(common)
  include_directories(external)
endif()

if(BUILD_SAMPLES)
  add_subdirectory(samples)
endif()

if(BUILD_TESTS)
  include_directories(${FPCPP_SOURCE_DIR}/tests)
  add_subdirectory(tests)
endif()