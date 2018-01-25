// MultiProducerMultiConsumer.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <chrono>
#include "MultiProducerSingleConsumer.h"
#include <conio.h>
#include <windows.h>
#include <ctime>
//#include <boost/date_time/posix_time/posix_time.hpp>

boost::optional<int> fn1()
{
	static int i = 1;
	while (true)
	{
		//Sleep(rand() % 1000);
		return i += 3;
	}
}

boost::optional<int> fn2()
{
	static int i = 2;
	while (true)
	{
		//Sleep(rand() % 1000);
		return i +=3;
	}
}

boost::optional<int> fn3()
{
	static int i = 3;
	while (true)
	{
		//Sleep(rand()%1000);
		return i += 3;
	}
}

void fn4(int i)
{
	using namespace std::chrono;
	high_resolution_clock::time_point p = high_resolution_clock::now();

	milliseconds ms = duration_cast<milliseconds>(p.time_since_epoch());
	seconds s = duration_cast<seconds>(ms);
	std::time_t t = s.count();
	std::size_t fractional_seconds = ms.count() % 1000;

	std::cout << std::endl << i << " received from producers at time = " << std::ctime(&t)  << fractional_seconds;
}

int main(int argc, char* argv[])
{
	std::queue<int> queue;
	std::mutex mutex;
	std::condition_variable cv;
	std::vector<std::function<boost::optional<int>()>> producerFunctions;
	producerFunctions.push_back(fn1);
	producerFunctions.push_back(fn2);
	producerFunctions.push_back(fn3);

	MultiProducerSingleConsumerVersion1<int> pc(queue, mutex, cv, producerFunctions, fn4);
	pc.start();
	return 0;
}

