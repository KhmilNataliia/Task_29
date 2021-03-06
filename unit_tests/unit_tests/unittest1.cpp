#include "stdafx.h"
#include "CppUnitTest.h"

#include "catch.hpp"
#include "threadpool.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unit_tests
{	
	int mul(int a, int b)
	{
		return a * b;
	}

	TEST_CASE("test1")
	{
		int r1;
		bool r2;
		string r3;
		thread_pool pool;
		function<string(int, int)> func = [&](int a, int b)
		{
			string str = "I like listening 'Powerwolf' " + to_string(a) + to_string(b);
			return str;
		};

		auto t1 = pool.run_async<int>(&mul, 5, 13);
		auto t2 = pool.run_async<bool>([](){ return true; });
		auto t3 = pool.run_async<string>(func, 20, 17);
		while (!t1->ready);
		r1 =t1->data;
		while (!t2->ready);
		r2= t2->data;
		while (!t3->ready);
		r3= t3->data;
		REQUIRE(r1 == 65);
	};


	TEST_CASE("test2")
	{
		int r1;
		bool r2;
		string r3;
		thread_pool pool;
		function<string(int, int)> func = [&](int a, int b)
		{
			string str = "I like listening 'Powerwolf' " + to_string(a) + to_string(b);
			return str;
		};

		auto t1 = pool.run_async<int>(&mul, 5, 13);
		auto t2 = pool.run_async<bool>([](){ return true; });
		auto t3 = pool.run_async<string>(func, 20, 17);
		while (!t1->ready);
		r1 = t1->data;
		while (!t2->ready);
		r2 = t2->data;
		while (!t3->ready);
		r3 = t3->data;
		REQUIRE(r2 == true);
	};

	TEST_CASE("test3")
	{
		int r1;
		bool r2;
		string r3;
		thread_pool pool;
		function<string(int, int)> func = [&](int a, int b)
		{
			string str = "my birthday is " + to_string(a) +"."+ to_string(b);
			return str;
		};

		auto t1 = pool.run_async<int>(&mul, 5, 13);
		auto t2 = pool.run_async<bool>([](){ return true; });
		auto t3 = pool.run_async<string>(func, 22, 12);
		while (!t1->ready);
		r1 = t1->data;
		while (!t2->ready);
		r2 = t2->data;
		while (!t3->ready);
		r3 = t3->data;
		REQUIRE(r3=="my birthday is 22.12");
	};
}