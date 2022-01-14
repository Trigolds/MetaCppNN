// =============================================================================
//                    MetaProg Chapter II Solution 2
//                         Author : Xin Liu
// 版权声明：本文为CSDN博主「yanziguilai」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
// 原文链接：https://blog.csdn.net/xiaoliangsky/article/details/25249355
// =============================================================================

#include <stdio.h>
#include <iostream>
#include <string>
#include "boost/type_traits.hpp"
#include "boost/mpl/bool.hpp"

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

template<typename U, typename T>
struct is_same: boost::mpl::bool_<false> { };
template<typename T>
struct is_same<T, T>: boost::mpl::bool_<true> { };

template<typename TC, typename TX, typename TY, bool same>
struct replace_type_imp;

template<typename TC, typename TX, typename TY>
struct replace_type
{
  static const bool value = is_same<TC, TX>::value;
  typedef typename replace_type_imp<TC, TX, TY, value>::type type;
};


//TC:void const*, TX:void const
template<typename TC, typename TX, typename TY>
struct replace_type_imp<TC(), TX, TY, false>
{
	typedef typename replace_type<TC, TX, TY>::type type();
};
//TC:int const*, TX:int const
template<typename TC, typename TX, typename TY>
struct replace_type_imp<TC*, TX, TY, false>
{
	typedef typename replace_type<TC, TX, TY>::type* type;
};
//TC::int const&, TX:int const
template<typename TC, typename TX, typename TY>
struct replace_type_imp<TC&, TX, TY, false>
{
	typedef typename replace_type<TC, TX, TY>::type& type;
};
//TC::int const[], TX:int const
template<typename TC, typename TX, typename TY>
struct replace_type_imp<TC[], TX, TY, false>
{
	typedef typename replace_type<TC, TX, TY>::type type[];
};
//TC::int const[N], TX:int const
template<typename TC, typename TX, typename TY, int N>
struct replace_type_imp<TC[N], TX, TY, false>
{
	typedef typename replace_type<TC, TX, TY>::type type[N];
};
//TC:: char* (*)(char*), TX: char*, TY: int
template<typename TC, typename TX, typename TY, typename Arg1>
struct replace_type_imp<TC(Arg1), TX, TY, false>
{
	typedef typename replace_type<TC, TX, TY>::type type(typename replace_type<Arg1, TX, TY>::type);//#1
};
//TC:: char* (*)(char*, const char* ), TX: char*, TY:: int
template<typename TC, typename TX, typename TY, typename Arg1, typename Arg2>
struct replace_type_imp<TC(Arg1, Arg2), TX, TY, false>
{
	typedef typename replace_type<TC, TX, TY>::type type(typename replace_type<Arg1, TX, TY>::type,
		typename replace_type<Arg2, TX, TY>::type);
};
template<typename TC, typename TX, typename TY>
struct replace_type_imp<TC, TX, TY, true>
{
	typedef TY type;
};


int main( int , char* [] )
{
  bool res(true), rres(true);

  typedef char& (*FunPoint[])(char&);
	typedef char& Rchar;
	typedef const int v1;
	typedef const int v2;
	typedef int* v3;
	typedef int* (*IntPoint[])(int*);

  rres = is_same<v1, v2>::value;
	if (rres) std::cout << "same\n\n";
	else std::cout << "false\n\n" << std::endl;
  res &= rres;

  rres = boost::is_same<v1, v2>::value;
	if (rres) std::cout << "same\n\n";
	else std::cout << "false\n\n" << std::endl;
  res &= rres;

	typedef replace_type<v1[10], v2, v3>::type v4;
  rres = is_same<v3[10], v4>::value;
	if (rres) std::cout << "same\n\n";
	else std::cout << "false\n\n" << std::endl;
 
	if (boost::is_same<v3[10], v4>::value) std::cout << "same\n\n";
	else std::cout << "false\n\n" << std::endl;
 
	typedef replace_type<FunPoint, Rchar, v3>::type v5;
	if (is_same<IntPoint, v5>::value) std::cout << "same\n\n";
	else std::cout << "false\n\n" << std::endl;
 
	if (boost::is_same<IntPoint, v5>::value) std::cout << "same\n\n";
	else std::cout << "false\n\n" << std::endl;
 
 
	typedef replace_type<v3 const, v3 const, v2>::type v6;
	if (is_same<v2, v6>::value) std::cout << "same\n\n";
	else std::cout << "false\n\n" << std::endl;
 
	if (boost::is_same<v2, v6>::value) std::cout << "same\n\n";
	else std::cout << "false\n\n" << std::endl;

  if (res) {
    printf("Test case passes :)\n");
  }
  else {
    printf("Test case fails :(\n");
  }

  return 0;
}
