/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#ifndef _FP_PRELUDE_STRINGS_H_
#define _FP_PRELUDE_STRINGS_H_

#include "fp_defines.h"
#include "fp_curry.h"
#include "fp_prelude.h"

#include <sstream>
#include <fstream>

namespace fp {

///////////////////////////////////////////////////////////////////////////
// String operations
///////////////////////////////////////////////////////////////////////////

template<typename T>
inline std::vector<T>& split_helper(const T& s, char delim, std::vector<T>& elems) {
  std::stringstream ss(s);
  T item;
  while(std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

template<typename T>
inline std::vector<T> split(const T& s, char delim) {
  std::vector<T> elems;
  return split_helper(s, delim, elems);
};

template<typename T>
typename T concat(const std::vector<T>& elems, char delim = ' ') {
  std::stringstream ss;
  std::for_each(head(elems), tail(elems), [=,&ss](const T& s) {
    ss << s << delim;
  });
  let str = ss.str();
  return str.length() > 0 ? str.substr(0, str.length()-1) : "";
}

inline bool istrue(bool b) { return b; }

///////////////////////////////////////////////////////////////////////////
// lines

template<typename T>
inline std::vector<T> lines(const T& s) {
  return split(s, '\n');
}
inline std::vector<std::string> lines(std::ifstream ifs) {
  std::vector<std::string> ifsLines;
  std::string line;
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
inline std::vector<T> words(const T& s) {
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
inline typename std::enable_if<!is_container<T>::value, std::string>::type show(const T& t) {
  std::stringstream ss;
  ss << t;
  return ss.str();
}

template<typename C>
inline typename std::enable_if<is_container<C>::value, std::string>::type show(const C& c) {
  return concat(c, ' ');
}

std::string show(const std::vector<char>& c) {
  return std::string(head(c), tail(c));
}

} /* namespace fp */

#endif /* _FP_PRELUDE_STRINGS_H_ */
