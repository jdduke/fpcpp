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
    
Curry functions:
   
    let multiplyBy4 = curry(std::multiplies<int>(), 4);
    let sixteen     = multiplyBy4(4);
    
Compose functions:

    // l1 norm = |x|+|y| = f(g(x),g(y)) where f = add, g = abs

    let fabs = [](float x) { return std::fabs(x); };
    let l1   = compose2(std::plus<float>(), fabs, fabs);
    
    show( l1(1.f, 2.f) );


    // l2 norm = sqrt(x*x + y*y) = f(g(h(x),h(y))) where f = sqrt, g = plus, h = square

    let sqrtf  = [](float x) { return std::sqrtf(x); };
    let square = [](float x) { return x*x; };
    let l2 = compose(sqrtf, compose2(std::plus<float>(), square, square));
    
    show( l2(1.f, 2.f) );
    

    
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