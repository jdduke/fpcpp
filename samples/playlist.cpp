/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#include <fpcpp.h>

#include <fstream>
#include <string>
#include <iostream>

using std::string;

template<typename MapOp, typename FilterOp, typename Source>
bool filteredMap( MapOp mapOp, FilterOp filterOp, Source source, std::function<bool(result_type_of(MapOp))> successOp = &fp::istrue ) {
  using namespace fp;
  return all(successOp,
             map(mapOp,
                 filter(filterOp,
                        source)));
}

bool m3uLineFilter( const string& line ) {
  return (!line.empty())  &&
         ( line[0] != '#') &&
         ( line.find_first_of(".mp3") != std::string::npos );
}

bool deletePlaylist( const fp::FilePath& playlistPath ) {
  std::ifstream ifs( playlistPath );
  return filteredMap( &fp::removeFile, &m3uLineFilter, fp::lines(ifs) );
}

int main(int argc, char **argv) {

  //TODO: parse args
  //TODO: dispatch commands

  return 0;
}
