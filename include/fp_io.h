/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#ifndef _FP_IO_H_
#define _FP_IO_H_

#include "fp_defines.h"
#include "fp_common.h"

#include <string>
#include <stdio.h>
#include <fstream>

#define USE_PLATFORM_SPECIFIC_CODE 0

#if USE_PLATFORM_SPECIFIC_CODE
#if defined(FP_WINDOWS)
#include <Windows.h>
#else
#include <fcntl.h>
#include <stdlib.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif
#endif /* USE_PLATFORM_SPECIFIC_CODE */

namespace fp {

typedef std::string FilePath;

std::ifstream& readFile( const FilePath& filePath, std::ifstream& ifs ) {
  ifs.open( filePath );
  return ifs;
}

bool removeFile( const FilePath& filePath ) {
  return remove( fromString(filePath) ) == 0;
}

bool renameFile( const FilePath& srcPath, const std::string& dstFileName ) {
  return ::rename( fromString(srcPath), fromString(dstFileName) ) == 0;
}

bool copyFile( const FilePath& srcPath, const FilePath& dstPath ) {
#if USE_PLATFORM_SPECIFIC_CODE
#if defined(PU_WINDOWS)
  return CopyFile( fromString(srcPath), fromString(dstPath), false ) != 0;
#else
  bool success = false;
  int read_fd, write_fd;
  struct stat stat_buf;
  off_t offset = 0;

  read_fd = open( fromString(srcPath), O_RDONLY );
  if( read_fd != -1 ) {
    if ( fstat(read_fd, &stat_buf) == 0 ) {
      write_fd = open( fromString(dstPath), O_WRONLY | O_CREAT, stat_buf.st_mode );
      if ( write_fd != -1 ) {
        success = sendfile( write_fd, read_fd, &offset, stat_buf.st_size ) != -1;
        close( write_fd );
      }
    }
    close(read_fd);
  }
  return success;
}
#endif /* defined(FP_WINDOWS) */
#else  /* USE_PLATFORM_SPECIFIC_CODE */
  std::ifstream ifs( srcPath, std::ios::in  | std::ios::binary );
  if ( ifs.is_open() ) {
    std::ofstream ofs( dstPath, std::ios::out | std::ios::binary );
    if ( ofs.is_open() ) {
      ofs << ifs.rdbuf();
      return !ofs.bad() /* && ifs.eof() */ ;
    }
  }
  return false;
#endif
}

bool moveFile( const FilePath& srcPath, const FilePath& dstPath ) {
  return copyFile( srcPath, dstPath ) && removeFile( srcPath );
}

bool doesFileExist( const FilePath& filePath ) {
#if USE_PLATFORM_SPECIFIC_CODE
#if defined(FP_WINDOWS)
  DWORD attributes = GetFileAttributes( fromString(filePath) );
  if (attributes != INVALID_FILE_ATTRIBUTES &&
      attributes & FILE_ATTRIBUTE_DIRECTORY)
    return true;
  else
    return false;
#else
  struct stat sb;
  return (stat( fromString(filePath), &sb ) == 0) && S_ISDIR(sb.st_mode);
#endif /* defined(FP_WINDOWS) */
#else /* USE_PLATFORM_SPECIFIC_CODE */
  std::ifstream ifs( filePath );
  return !(!ifs);
#endif
}

size_t fileSize( const FilePath& filePath ) {
  std::streampos fsize = 0;
  std::ifstream file(filePath, std::ios::binary);
  if (file.is_open()) {
    fsize = file.tellg();
    file.seekg( 0, std::ios::end );
    fsize = file.tellg() - fsize;
  }
  return (size_t)fsize;
}

}

#endif /* _FP_IO_H */
