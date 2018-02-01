// TimerVersion1.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include "TimedCaller.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <conio.h>
#include <ctime>
#include <thread>
#include <string>
#include <stdlib.h>
#include <windows.h>
//#include<boost/thread/thread.hpp>

#define TIME_DIVIDER 10000


void f(int n)
{
	std::cout << std::endl << "Inside f" << n << " at time =" << std::chrono::high_resolution_clock::now().time_since_epoch().count() / TIME_DIVIDER;
}


int main()
{
	std::shared_ptr<std::queue<std::function<void()>>> fn_queue(new std::queue<std::function<void()>>);
	std::shared_ptr<std::mutex> mutex(new std::mutex);
	std::shared_ptr<std::condition_variable> cv(new std::condition_variable);
	std::vector<std::pair<std::function<void()>, UINT>> fn_producers;
	for (UINT i = 1; i <= 10; i++)
		fn_producers.push_back(std::pair<std::function<void()>, UINT>(std::bind(f, i), i*100));
	TimedCallerV1 tc(fn_queue, mutex, cv, fn_producers);
	tc.start();
	/*std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	f1();*/
	//TimedCaller tc
	_getch();
    return 0;
}

