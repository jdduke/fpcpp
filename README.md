fpcpp
===========================

fpcpp (functional programming in C++) is a lightweight, header-based C++ library for enabling functional programming style and features
in modern C++ programs.


Example
-------------

Generate pi using Monte Carlo sampling:

    double pi(size_t samples = 1000) {
        using namespace fp;
        typedef std::pair<double,double> point;
        let dxs = map([](const point& p) { return p.first*p.first + p.second*p.second; },
                      zip(take(samples, rand_range(-1.0,1.0)),
                          take(samples, rand_range(-1.0,1.0))));
        return 4.0 * filter([](double d) { return d <= 1.0; }, dxs).size() / dxs.size();
    }

Map an operation across a filtered source and check for success: 

    template<typename SuccessOp, typename MapOp, typename FilterOp, typename Source> {
    bool filteredMap(SuccessOp, MapOp mapOp, FilterOp filterOp, Source source) { 
        using namespace fp;
        return all(successOp,
                   map(mapOp,
                       filter(filterOp,
                              source)));
    }

With which we can, for example, delete all MP3's referenced in an M3U playlist:

    let deleteM3UFiles = [](const string& fileName) -> bool {
        
        let isMp3 = [](const string& s) { 
            return (!s.empty())  && 
                   ( s[0] != '#') &&
                   ( s.find_first_of(".mp3") != string::npos );
        };
      
        let deleteFile = [=](const string& mp3) {
            return remove(mp3.c_str()) == 0; 
        };
      
        return filteredMap(istrue, deleteFile, isMp3, lines(std::ifstream(fileName)));                                   
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