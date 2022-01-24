#include <iostream>
#include <cassert>
#include <cmath>
#include <set>
#include "var_type_dict.h"
using namespace std;
using namespace MetaNN;

namespace
{
struct A; struct B; struct Weight; struct XX;

struct FParams : public VarTypeDict<struct A,
                                    struct B,
                                    struct Weight> {};

template <typename TIn>
float fun(const TIn& in) {
    auto a = in.template Get<A>();
    auto b = in.template Get<B>();
    auto weight = in.template Get<Weight>();

    return a * weight + b * (1 - weight);
    }
}

#define N 3

void fun_(int(&&input)[N])
{
    for(int i=0; i<N; ++i){
        std::cout << "a: " << input[i] << "\n";
        input[i] = 1;
    }
}

int fun2_()
{
    return int{};
}

void main()
{
    cout << "Test named params...\t";
    float aa(1.3f), bb(2.4f), ww(0.1f); 
    auto res = fun(FParams::Create()
                             .Set<A>(aa)
                             .Set<B>(bb)
                             .Set<Weight>(ww));
    assert(fabs(res - 0.1 * 1.3 - 0.9 * 2.4) < 0.0001);
    cout << res << " done\n";

    int a[N] = {2, 3, 6};
    fun_(std::move(a));

    for(int i=0; i<N; ++i){
        std::cout << "new: " << a[i] << "\n";
    }

    std::string str = "Hello";
    std::vector<std::string> v;
    v.push_back(str);
    std::cout<<"After copy, str is \""<<str<<"\"\n";
    //输出结果为 After copy, str is "Hello"
    v.push_back(std::move(str));
    std::cout<<"After move, str is \""<<str<<"\"\n";
    //输出结果为 After move, str is ""

    std::cout << "fun2_: " << fun2_() << "\n";
}
