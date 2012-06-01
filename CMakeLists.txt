cmake_minimum_required (VERSION 2.6)
project(fpcpp)

set(CMAKE_DEBUG_POSTFIX "d")

# Treat warnings as errors cmake option
set(TREAT_WARNINGS_AS_ERRORS TRUE CACHE BOOL "Treat warnings as errors")
SET(BUILD_FPCPP_SAMPLES      TRUE  CACHE BOOL "Build FPCPP samples")
set(BUILD_FPCPP_TESTS        FALSE CACHE BOOL "Build FPCPP unit tests")

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

if(UNIX OR MINGW)
  set(STOP_ON_FIRST_ERROR TRUE CACHE BOOL "Stop compilation on first error")
  if(TREAT_WARNINGS_AS_ERRORS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
  endif()
  if (STOP_ON_FIRST_ERROR)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfatal-errors")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wfatal-errors")
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -std=c++0x -Wall -Wno-missing-braces")
  if(MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -U__STRICT_ANSI__")
  endif()
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wno-deprecated")
  else()
    add_definitions( "-DDEBUG" )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG -fomit-frame-pointer -Wno-deprecated -fno-omit-frame-pointer")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -DNDEBUG -fomit-frame-pointer -Wno-deprecated -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")
  endif()
endif()

if(MSVC)
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
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /Ox /GL /GS- /fp:fast")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox /GL /GS- /fp:fast")
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

include_directories(include)
include_directories(externals)

if(BUILD_FPCPP_SAMPLES OR BUILD_FPCPP_TESTS)
  include_directories(common)
  include_directories(external)
  include_directories(samples)
endif()

if(BUILD_FPCPP_SAMPLES)
  add_subdirectory(samples)
endif()

if(BUILD_FPCPP_TESTS)
  include_directories(${FPCPP_SOURCE_DIR}/tests)
  add_subdirectory(externals/googletest)
  add_subdirectory(tests)
endif()
