/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#ifndef _FP_PRELUDE_STRINGS_H_
#define _FP_PRELUDE_STRINGS_H_

#include "fp_defines.h"
#include "fp_prelude.h"
#include "fp_prelude_lists.h"

#include <sstream>
#include <fstream>
#include <iostream>

namespace fp {

///////////////////////////////////////////////////////////////////////////
// String operations
///////////////////////////////////////////////////////////////////////////

template<typename T>
inline typename types<T>::list& split_helper(const T& s, char delim, typename types<T>::list& elems) {
  std::stringstream ss(s);
  T item;
  while(std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

template<typename T>
inline typename types<T>::list split(const T& s, char delim) {
  typename types<T>::list elems;
  return split_helper(s, delim, elems);
};

template<typename C>
string concat(const C& c, const char* infix = " ", const char* prefix = "", const char* suffix = "") {
  if (length(c) == 0)
    return "";
  
  std::stringstream ss;
  ss << prefix;
  let it = begin(c);
  if (it != end(c)) {
    while (true) {
      ss << show(*it);
      if (++it == end(c)) break;
      ss << infix;
    }
  }
  ss << suffix;

  return ss.str();
}

inline bool istrue(bool b) { return b; }

///////////////////////////////////////////////////////////////////////////
// lines

template<typename T>
inline typename types<T>::list lines(const T& s) {
  return split(s, '\n');
}
inline types<string>::list lines(std::ifstream& ifs) {
  types<string>::list ifsLines;
  string line;
  while (getline(ifs, line))
    ifsLines.push_back(line);
  return ifsLines;
}

///////////////////////////////////////////////////////////////////////////
// unlines

template<typename C>
auto unlines(const C& elems) -> value_type_of(C) {
  return concat(elems, '\n');
}

///////////////////////////////////////////////////////////////////////////
// words
template<typename T>
inline typename types<T>::list words(const T& s) {
  return split(s, ' ');
}

///////////////////////////////////////////////////////////////////////////
// unwords

template<typename T>
auto unwords(const T& elems) -> decltype(concat(elems, ' ')) {
  return concat(elems, ' ');
}

///////////////////////////////////////////////////////////////////////////
// show
template<typename T>
inline fp_enable_if_not_container(T,string) show(const T& t) {
  std::stringstream ss;
  ss << t;
  return ss.str();
}

inline string show(const types<char>::list& c) {
  return string(extent(c));
}

inline string show(const string& s) {
  return s;
}

template<typename C>
inline fp_enable_if_container(C,string) show(const C& c) {

  typedef value_type_of(C) T;
  const bool is_nonstring_container = is_container<T>::value && !std::is_same<string,T>::value;
  const char* infix  = is_nonstring_container ? ",\n" : ", ";
  const char* prefix = is_nonstring_container ? "["   : "[";
  const char* suffix = is_nonstring_container ? "]\n" : "]";

  return concat( c, infix, prefix, suffix );
}


///////////////////////////////////////////////////////////////////////////
// print

inline void putStr(const string& s) {
  std::cout << s;
}

inline void putStrLen(const string& s) {
  std::cout << s << std::endl;
}

template<typename T>
inline void print(const T& t) {
  putStrLen( show(t) );
}

} /* namespace fp */

#endif /* _FP_PRELUDE_STRINGS_H_ */
