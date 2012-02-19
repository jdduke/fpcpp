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
std::string concat(const std::vector<T>& elems, char delim = ' ') {
  std::stringstream ss;
  //mapM([&](const T& t) { ss << t; if (tail(elems) != next(&t)) ss << delim; }, elems);
  for (let s = head(elems); s != tail(elems); ++s) {
    ss << *s;
    if (next(s) != tail(elems))
      ss << delim;
  }
  return ss.str();
}

inline bool istrue(bool b) { return b; }

///////////////////////////////////////////////////////////////////////////
// lines

template<typename T>
inline std::vector<T> lines(const T& s) {
  return split(s, '\n');
}
inline std::vector<std::string> lines(std::ifstream& ifs) {
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
inline fp_enable_if_not_container(T,std::string) show(const T& t) {
  std::stringstream ss;
  ss << t;
  return ss.str();
}

std::string show(const std::vector<char>& c) {
  return std::string(head(c), tail(c));
}

std::string show(const std::string& s) {
  return s;
}

template<typename C>
inline fp_enable_if_container(C,std::string) show(const C& c) {
  typedef value_type_of(C) T;
#if 0
  std::string result("[ ");
  return result.append( foldl1( [](const std::string& s0, const std::string& s1) -> std::string {
    std::string result(s0);
    return result.append(", ").append(s1);
  }, map([](const T& t) {
    return show(t);
  }, c) ) ).append(" ]");
#else
  std::string result("[ ");
  std::for_each( extent(c), [&](const T& t) {
    result.append( show(t) ).append(" ");
  });
  return result.append("]");
#endif
}


///////////////////////////////////////////////////////////////////////////
// print

inline void putStr(const std::string& s) {
  std::cout << s;
}

inline void putStrLen(const std::string& s) {
  std::cout << s << std::endl;
}

template<typename T>
inline void print(const T& t) {
  putStrLen( show(t) );
}

} /* namespace fp */

#endif /* _FP_PRELUDE_STRINGS_H_ */
