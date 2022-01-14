// =============================================================================
//                    MetaProg Chapter II Solution 3
//                         Author : Xin Liu
// 版权声明：本文为CSDN博主「天色微凉」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
// 原文链接：https://blog.csdn.net/qq_33882435/article/details/111593233
// =============================================================================

#include <stdio.h>
#include <iostream>
#include <string>
#include <cassert>
#include "boost/type_traits.hpp"

struct A
{
	virtual ~A() {}
};
struct B : A
{

};

template <class Target, class Source>
inline Target polymorphic_downcast(Source *x)
{
	assert(dynamic_cast<Target>(x) == x);
	return static_cast<Target>(x);
}

template <class Target, class Source>
//route to ref func
inline Target polymorphic_downcast(Source &x)
{
	assert(dynamic_cast<typename std::add_pointer<typename std::remove_reference<Target>::type>::type>(&x) == &x);
	return static_cast<Target>(x);
}

int main()
{
  bool res(true);
	B b;
	A* a_ptr = &b;
	B* b_ptr = polymorphic_downcast<B*>(a_ptr);
	res &= (b_ptr == a_ptr);
	A& a_ref = b;
	//route to ref func
	B& b_ref = polymorphic_downcast<B&>(a_ref);
	res &= (&b_ref == &a_ref);
	if (res){
		printf("Test case passes :)\n");
	}
	else{
		printf("Test case fails :(\n");
	}
	return 0;
}