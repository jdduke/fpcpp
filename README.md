fpcpp
===========================

fpcpp (functional programming in C++) is a lightweight, header-based C++ library for enabling functional programming style and features
in modern C++ programs.


Example
-------------

Generate pi using Monte Carlo sampling:

    double pi(size_t samples = 1000) {    
    
        return 4.0 * length(filter([](double d) { return d <= 1.0; }, 
                                   map([](const pair<double,double>& p) { return fst(p)*fst(p) + snd(p)*snd(p); },
                                       zip(take(samples, uniform(-1.0,1.0)),
                                           take(samples, uniform(-1.0,1.0))))) / samples;
                                           
    }
or

    double pi(size_t samples = 1000) {
    
        let sampler = []() -> int { 
            let x = uniform(-1.,1.); 
            let y = uniform(-1.,1.);
            let dist2 = x*x + y*y;
            return dist2 < 1. ? 1 : 0;
        };

        return 4. * foldl1(addF(), take(samples, sampler)) / samples;
        
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
   
    let multiplyBy4 = curry(multiplyF()), 4);
    let twentyEight = multiplyBy4(7);
    
Compose functions:

    // l1 norm = |x|+|y| = f(g(x),g(y)) where f = add, g = abs

    let l1   = compose2(addF(), absF(), absF());
    assert( l1(1.f, -2.f) == 3.f );
    

    // l2 norm = sqrt(x*x + y*y) = f(g(h(x),h(y))) where f = sqrt, g = plus, h = square

    let square = [](float x) { return x*x; };
    let l2 = compose(sqrtf, compose2(addF(), square, square));
    assert( l2(3.f, 4.f) == 5.f );
    


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