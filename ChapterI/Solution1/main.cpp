// =============================================================================
//                    MetaProg Chapter I Solution 1
//                         Author : Xin Liu
// =============================================================================

#include <stdio.h>
#include <iostream>
#include <string>

constexpr int fun(int a) {return a + 1;}

static int call_count = 3;
constexpr int fun2(int a)
{
  return a + (call_count++);
}

template<typename T>
struct Fun_ {using type = T; };

template<>
struct Fun_<int> {using type = unsigned int; };

template<>
struct Fun_<long> {using type = unsigned long; };

template<typename T>
using Fun = typename Fun_<T>::type;

template<int a, int b>
struct Add_
{
  constexpr static int value = a + b;
};

template<int a, int b>
constexpr int Add = a + b;

template<template <typename> class T1, typename T2>
struct Fun2_
{
  using type = typename T1<T2>::type;
};

template<template <typename> class T1, typename T2>
using Fun2 = typename Fun2_<T1,T2>::type;

template<bool AddOrRemoveRef> struct Fun3_;

template<>
struct Fun3_<true> 
{
  template<typename T>
  using type = std::add_lvalue_reference<T>;
};

template<>
struct Fun3_<false>
{
  template<typename T>
  using type = std::remove_reference<T>;
};

// template<typename T>
// template<bool AddOrRemove>
// using Fun3 = typename Fun3_<AddOrRemove>::template type<T>;

template<typename T>
using Res_ = typename Fun3_<false>::template type<T>;

template<typename T>
struct RemoveReferenceConst_
{
  private:
    using inter_type = typename std::remove_reference_t<T>;
  public:
    using type = typename std::remove_const_t<inter_type>;
};

template<typename T>
using RemoveReferenceConst = typename RemoveReferenceConst_<T>::type;

struct A;
struct B;

template<typename T>
constexpr size_t Fun5 = 0;

template<>
constexpr size_t Fun5<A> = 1;

template<>
constexpr size_t Fun5<B> = 6;

template<typename TW>
struct Wrapper
{
  template<typename T, typename TDummy = void>
  struct Fun_
  {
    constexpr static size_t value = 0;
  };

  template<typename TDummy>
  struct Fun_<int, TDummy>
  {
    constexpr static size_t value = 10;
  };
};

template Wrapper<int>;

template<bool Check, std::enable_if_t<Check>* = nullptr>
auto fun(){
  return (int)0;
}
template<bool Check, std::enable_if_t<!Check>* = nullptr>
auto fun(){
  return (double)1.5;
}

template<bool Check>
auto wrap2()
{
  return fun<Check>();
}

template<size_t...inputs>
constexpr size_t Accumulate = 0;

template<size_t CurInput, size_t...inputs>
constexpr size_t Accumulate<CurInput, inputs...>
  = CurInput + Accumulate<inputs...>;

template<typename T>
size_t get_size()
{
  return sizeof(T);
}

template<typename T, int i>
bool is_equal()
{
  return get_size<T>() == i;
}

template<size_t N>
constexpr bool is_odd = ((N%2) == 1);

template<bool cur, typename TNext>
constexpr bool AndValue = false;

template<typename TNext>
constexpr bool AndValue<true, TNext> = typename TNext::value;


template<size_t N>
struct AllOdd_
{
  constexpr static bool is_cur_odd = is_odd<N>;
  //constexpr static bool is_pre_odd = AllOdd_<N-2>::value;
  constexpr static bool value = AndValue<is_cur_odd, AllOdd_<N-2> >;
};

template<>
struct AllOdd_<0>
{
  constexpr static bool value = is_odd<0>;
};

template<>
struct AllOdd_<1>
{
  constexpr static bool value = is_odd<1>;
};

template<typename D>
struct Base
{
  static void Fun(){
    D::Imp();
  }
};

struct Derive: public Base<Derive>
{
  static void Imp(){
    std::cout << "Imp from derive class\n";
  }
};

template<typename D>
struct Base_
{
  template<typename T>
  void Fun(const T& input){
    D* ptr = static_cast<D*>(this);
    ptr->Imp(input);
  }
};

struct Derive_: public Base_<Derive_>
{
  template<typename T>
  void Imp(const T& input){
    std::cout << input << "\n";
  }
};

int main( int , char* [] )
{
  std::cout << fun(1) << "\n";
  int a(2);
  std::cout << fun(a) << "\n";
  std::cout << fun2(a) << "\n";

  Fun_<int>::type h = 3;
  if(h > 1) std::cout << "compare int .vs. unsigned int\n";

  Fun<int> hh = 3;
  if(hh > 1) std::cout << "compare int .vs. unsigned int\n";

  std::remove_reference<int&>::type h1 = 3;
  std::remove_reference_t<int&> h2 = 3;
  std::cout << fun(h1) << "\n";
  std::cout << fun(h2) << "\n";

  constexpr int x = Add_<2,3>::value;
  constexpr int y = Add<2,3>;
  std::cout << x << "\n";
  std::cout << y << "\n";

  Fun2<std::remove_reference, int&> hhh = 3;
  std::cout << hhh << "\n";

  Fun3_<false>::type<int&>::type hhhh = 4;
  std::cout << hhhh << "\n";

  Res_<int&>::type  h5 = 5;
  std::cout << h5 << "\n";

  RemoveReferenceConst<const int&> h6 = 6;
  std::cout << h6 << "\n";

  size_t h7 = Fun5<B>;
  std::cout << h7 << "\n";

  std::cout << Wrapper<int>::Fun_<int>::value << "\n";

  std::cout << wrap2<true>() << "\n";
  std::cout << wrap2<false>() << "\n";

  constexpr size_t res = Accumulate<1,2,3,4,5>;
  std::cout << res << "\n";

  std::cout << "size of double " << get_size<double>() << "\n";

  std::cout << "size of int is equal to 4 " << is_equal<int, 4>() << "\n";
  std::cout << "size of int is equal to 8 " << is_equal<int, 8>() << "\n";
  std::cout << "size of float is equal to 4 " << is_equal<int, 4>() << "\n";
  std::cout << "size of float is equal to 8 " << is_equal<int, 8>() << "\n";

  std::cout << AllOdd_<2>::value << "\n";

  Derive::Fun();

  Derive_ d;
  d.Fun("wtf");

  return 0;
}
