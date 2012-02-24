
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#include <fpcpp.h>

#include <fstream>
#include <string>
#include <iostream>
#include <regex>

#include <rapidxml/rapidxml_utils.hpp>

using std::string;
using std::regex;
using std::regex_match;

regex extensionRegex(const string& extension) {
  string result("^.*.\\.(");
  result.append(extension).append(")$");
  return regex(result, regex::icase);
}

template<typename C>
regex extensionsRegex(const C& extensions) {
  let extensionsString = fp::foldl1( [](const string& a, const string& b) -> string {
    let result = a;
    return result.append("|").append(b);
  }, extensions );
  return extensionRegex(extensionsString);
}

///////////////////////////////////////////////////////////////////////////

enum OpType {
  REMOVE = 0,
  COPY,
  MOVE,
  RENAME,
  NUM_OP_TYPES,
  INVALID_OP_TYPE,
};

std::array<string, NUM_OP_TYPES> OpTypeNames = {
  "remove",
  "copy",
  "move"
};
std::array<size_t, NUM_OP_TYPES> OpTypeArgs = {
  1,
  2,
  2,
};
let invalidOp = [](const fp::FilePath&) { return false; };

/////////////////////////////////////////////////////////////////////////////

enum SongType {
  MP3 = 0,
  WAV,
  OGG,
  WMA,
  FLAC,
  NUM_SONG_TYPES
};

std::array<string, NUM_SONG_TYPES> SongTypeExtensions = {
  "mp3",
  "wav",
  "ogg",
  "wma",
  "flac"
};
bool songFilter( const fp::FilePath& filePath ) {
  static const let songRegex = extensionsRegex( SongTypeExtensions );
  return regex_match( filePath, songRegex );
};

/////////////////////////////////////////////////////////////////////////////

enum PlaylistType {
  M3U = 0,
  WPL,
  NUM_PLAYLIST_TYPES
};

std::array<string, NUM_PLAYLIST_TYPES> PlaylistTypeExtensions = {
  "m3u",
  "wpl"
};

typedef fp::types< string >::list Playlist;

///////////////////////////////////////////////////////////////////////////

template <PlaylistType P> struct PlaylistUtils;

template<> struct PlaylistUtils<M3U> {
  static Playlist create( const fp::FilePath& filePath ) {
    let m3uLineFilter = []( const string& line ) {
      return (!line.empty())  &&
             ( line[0] != '#') &&
             ( line.find_first_of(".mp3") != std::string::npos );
    };

    std::ifstream ifs( filePath );
    return fp::filter( m3uLineFilter, fp::lines(filePath) );
  }
};

template<> struct PlaylistUtils<WPL> {
  static Playlist create( const fp::FilePath& filePath ) {
    using namespace rapidxml;
    file<char> xmlFile( fp::fromString(filePath) );
    xml_document<> doc;
    doc.parse<0>(xmlFile.data());
    let node = doc.first_node("media");
    let wplLines = [&]() mutable -> string {
      if (node) {
        let mp3Attribute = node->first_attribute("src");
        if (mp3Attribute)
          return mp3Attribute->value();
        else 
          return "invalid";
        node = node->next_sibling("media");
      } else {
        return "";
      }
    };
    return fp::takeWhileF( [](const string& s) {
      return !s.empty(); 
    }, wplLines );
  }
};

/////////////////////////////////////////////////////////////////////////////

template<typename MapOp, typename FilterOp, typename Source>
bool filteredMap( MapOp mapOp, FilterOp filterOp, Source source, std::function<bool(result_type_of(MapOp))> successOp = &fp::istrue ) {
  using namespace fp;
  return all(successOp,
             map(mapOp,
                 filter(filterOp,
                        source)));
}

Playlist songs( const fp::FilePath& filePath ) {
  if (        std::regex_match( filePath, extensionRegex( PlaylistTypeExtensions[M3U] ) ) ) {
    return PlaylistUtils<M3U>::create( filePath );
  } else if ( std::regex_match( filePath, extensionRegex( PlaylistTypeExtensions[WPL] ) ) ) {
    return PlaylistUtils<WPL>::create( filePath );
  } else {
    return Playlist();
  }
}

typedef std::function<bool(const fp::FilePath&)> SongOp;
OpType opType( const string& opName ) {
  for (size_t op = 0; op < NUM_OP_TYPES; ++op) {
    if ( OpTypeNames[op] == opName )
      return OpType(op);
  }
  return INVALID_OP_TYPE;
}
SongOp operation( int argc, char **argv ) {

  let op = opType( argv[1] );
  if (INVALID_OP_TYPE == op)
    return invalidOp;

  let opArgC = OpTypeArgs[op];
  if ((opArgC + 2) < argc)
    return invalidOp;

  switch ( op ) {
    case REMOVE:
    case COPY:
    case MOVE:
    default:
      return invalidOp;
  };
}

int main(int argc, char **argv) {

  if (argc < 2)
    return 0;

  let songOperation = operation(argc, argv);

  filteredMap( songOperation, &songFilter, songs(argv[2]) );

  //TODO: parse args
  //TODO: dispatch commands

  return 0;
}
