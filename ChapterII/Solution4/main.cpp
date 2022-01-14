// =============================================================================
//                    MetaProg Chapter II Solution 3
//                         Author : Xin Liu
// =============================================================================

#include <stdio.h>
#include <iostream>
#include <string>
#include <cassert>
#include "boost/type_traits.hpp"

template<typename T>
struct get_description
{
	static std::string value;
	operator const char*()
	{
		return value.c_str();
	}
};
 
template<typename T>
std::string get_description<T>::value = "can not deduce the type";
 
template<>
std::string get_description<int>::value = "int";
 
template<>
std::string get_description<char>::value = "char";
 
template<>
std::string get_description<short>::value = "short";
 
template<>
std::string get_description<long>::value = "long";
 
template<>
std::string get_description<float>::value = "float";
 
template<>
std::string get_description<double>::value = "double";
 
template<typename T>
struct get_description<const T>
{
	operator const char*()
	{
		static std::string ret = "const ";
		ret += get_description<T>();
		return ret.c_str();
	}
};
 
template<typename T>
struct get_description<T*>
{
	operator const char*()
	{
		static std::string ret;
		ret +=get_description<T>();
		ret += "* ";
		return ret.c_str();
	}
};
 
template<typename T>
struct get_description<T&>
{
	operator const char* ()
	{
		static std::string ret;
		ret += get_description<T>();
		ret += "& ";
		return ret.c_str();
	}
};
 
 
int main()
{
  std::cout << get_description<int>() << std::endl;
	std::cout << get_description<const long>()<< std::endl;
	std::cout << get_description<int&>()<< std::endl;
	std::cout << get_description<double*>()<< std::endl;
	return 0;
}