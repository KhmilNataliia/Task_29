#include <iostream>
#include <string>

#include "threadpool.h"

using namespace std;


int mul(int a, int b)
{
	return a * b;
}

int main()
{
	thread_pool pool;
    function<string(int, int)> func = [&](int a, int b)
	{
		string str = "I like listening 'Powerwolf' " + to_string(a) + to_string(b);
		return str;
	};

	auto t1 = pool.run_async<int>(&mul, 7, 13);
	auto t2 = pool.run_async<bool>([](){ return true; });
	auto t3 = pool.run_async<string>(func,20,17);
	while (!t1->ready);
	std::cout << t1->data << std::endl;
	while (!t2->ready);
	std::cout << t2->data << std::endl;
	while (!t3->ready);
	std::cout << t3->data << std::endl;

	system("pause");
}

