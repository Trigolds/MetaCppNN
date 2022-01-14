// =============================================================================
//                    MetaProg Chapter II Solution 1
//                         Author : Xin Liu
// =============================================================================

#include <stdio.h>
#include <iostream>
#include <string>
#include "boost/type_traits.hpp"

template<typename T>
struct add_const_ref
{
  typedef T const& type;
};

template<typename T>
struct add_const_ref<T&>
{
  typedef T& type;
};

template<bool res>
void print_msg(const std::string& tn);

template<> void print_msg<true>(const std::string& tn)
{
  std::cout << "Test: " << tn  << " pass :)\n";
}
template<> void print_msg<false>(const std::string& tn)
{
  std::cout << "Test: " << tn  << " fail :(\n";
}

int main( int , char* [] )
{
  bool res(true);
  typedef const int Cint;
  typedef const int& CRint;

  res &= boost::is_same<CRint, add_const_ref<CRint>::type>::value;
  res ? print_msg<true>("CRint") : print_msg<false>("CRint");
  res &= !boost::is_same<Cint, add_const_ref<Cint>::type>::value;
  res ? print_msg<true>("Cint") : print_msg<false>("Cint");

  if (res) {
    printf("Test case passes :)\n");
  }
  else {
    printf("Test case fails :(\n");
  }

  return 0;
}
