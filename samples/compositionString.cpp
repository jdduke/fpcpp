/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2012, Jared Duke.
// This code is released under the MIT License.
// www.opensource.org/licenses/mit-license.php
/////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>

#include <fpcpp.h>

enum {
  FilteredStringLength = 10
};

using std::string;
using std::stringstream;
typedef std::vector<string> strings;

strings& split_helper(const string& s, char delim, strings& elems) {
  stringstream ss(s);
  string item;
  while(std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

strings split(const string &s, char delim) {
  strings elems;
  return split_helper(s, delim, elems);
};

auto filterlines = [=](strings elems) -> strings {
  strings filteredElems;
  std::copy_if(begin(elems), end(elems), back_inserter(filteredElems), [=](const std::string& s) {
    return s.size() < FilteredStringLength;
  });
  return filteredElems;
};

auto mylines = [](string s) -> strings {
  return split(s, ' ');
};

strings lines2 (string s) {
  return split(s, ' ');
}

auto myunlines = [](strings elems) -> string {
  stringstream ss;
  std::for_each(elems.begin(), elems.end(), [&ss](const string& s) {
    ss << s << std::endl;
  });
  return ss.str();
};

string unlines2(strings elems) {
  stringstream ss;
  std::for_each(elems.begin(), elems.end(), [&ss](const string& s) {
    ss << s << std::endl;
  });
  return ss.str();
}

string inlines2(string input) {
  if (!input.empty())
    return input;
  else
  {
    std::cout << "Please enter a series of strings using separated by spaces." << std::endl;
    std::string line;
    std::getline(std::cin, line);
    return line;
  }
};

auto inlines = [](string input) -> std::string {
  return inlines2(input);
};

int main(int argc, char** argv) {

  using namespace fp;
  using namespace fp_operators;

  static const std::string testInput = "one two fifty-fivehundred eight-thousand-ninety-nine ten 0 sixtyfivethousand";

  auto lines_inlines  = mylines + make_function(inlines2);
  auto unlines_filter = myunlines + filterlines;
  auto shortLines     = unlines_filter.with(lines_inlines);
  auto shortLines2    = unlines_filter + lines_inlines;
  auto shortLines3    = make_function(unlines2) + filterlines + mylines + inlines;
  auto shortLines4    = (myunlines + filterlines).with(mylines + inlines);

  auto print = [](std::string s) {
    std::cout << s << std::endl << std::endl;
  };

  print( unlines(filterlines(lines(inlines(testInput)))));
  print( shortLines (testInput) );
  print( shortLines2(testInput) );
  print( shortLines3(testInput) );
  print( shortLines4(testInput) );

  return 0;
}
