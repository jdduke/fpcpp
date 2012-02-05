fpcpp
===========================

fpcpp (functional programming in C++) is a header-based C++ library for enabling functional programming style and features
in modern C++ programs.


Example
-------------

Simple use:


Deleting all mp3's reference in an .m3u file:

    let deleteM3U = [](const string& fileName) -> bool {
    
      enum status { success = 0, failure };
      typedef pair<string, status> line;
      
      ifstream m3uFile(fileName);      

      let m3uLines = [=,&m3uFile]() -> line {
        string s; 
        return getline(playlistFile, s) ? line(s, success) : line("", failure);
      };
      
      let isMp3 = [](const line& l) { 
        return (!l.first.empty())  && 
               ( l.first[0] != '#') &&
               ( l.first.find_first_of(".mp3") != std::string::npos);
      };
      
      let deleteFile = [=](const line& mp3) {
        return remove(mp3.first.c_str()) == 0 ? success : failure; 
      };
      
      return notElem(failure,
                     map(deleteFile, 
                         filter(isMp3, 
                                takeWhile([=](const line& l) { return l.second != failure; },
                                          m3uLines))));                                          
    }

Documentation
-------------

Please see the file called INSTALL for usage details.  
Several samples and tests are included with the source; together with CMake, these can be built and run 
with any compatible compiler (developed with VC10, tested on GCC 4.6).  

Licensing
---------

MIT.
Please see the file called LICENSE.

Contributions
-------------

This project was directly inspired by late-night encounters with Haskell.  FC++ (http://sourceforge.net/projects/fcpp/) 
is an excellent library for pre-C++11 functional programming endeavors. 