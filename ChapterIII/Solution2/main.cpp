// =============================================================================
//                    MetaProg Chapter III Solution 2
//                         Author : Xin Liu
// =============================================================================

#include <iostream>
#include <limits>
#include <boost/type_traits.hpp>
#include <boost/mpl/apply.hpp>
//#include <boost/mpl/vector_c.hpp>
//#include <boost/mpl/transform.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/equal.hpp>
//#include <boost/mpl/plus.hpp>
//#include <boost/mpl/multiplies.hpp>


namespace mpl = boost::mpl;
using namespace mpl::placeholders;

////////////////////////////////////////////////////////////////////////////////

struct add_pointer_f
{
    template <typename T>
    struct  apply: boost::add_pointer<T> {};
};

template <typename F, typename Arg>
struct apply1: F::template apply<Arg> {};

template <typename F, typename X>
struct twice: apply1<F, typename apply1<F, X>::type> {};

template <typename X>
struct twice2: twice<typename mpl::lambda<boost::add_pointer<_1> >::type, X> {};

template <typename F, typename Arg>
struct twice3: apply1<
    typename mpl::lambda<F>::type,
    typename apply1<typename mpl::lambda<F>::type, Arg>::type
    >
    {};

template <typename F, typename Arg>
struct twice4: mpl::apply<F, typename mpl::apply<F, Arg>::type> {};


int main()
{
    static_assert(boost::is_same<
        twice<add_pointer_f, int>::type, 
        int**
        >::value, 
        "OtherDimensions should be same as Dimensions.");
    
    static_assert(boost::is_same<
        twice2<int>::type, 
        int**
        >::value, 
        "OtherDimensions should be same as Dimensions.");
    
    int* x;
    twice3<add_pointer_f, int>::type                p = &x;
    twice3<boost::add_pointer<_1>, int>::type       q = &x;
    **p = 0;
    **q = 0;

    twice4<add_pointer_f, int>::type                m = &x;
    twice4<boost::add_pointer<_1>, int>::type       n = &x;
    **m = 0;
    **n = 0;
}
