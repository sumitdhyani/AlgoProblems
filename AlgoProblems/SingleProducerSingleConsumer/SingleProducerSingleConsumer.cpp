// SingleProducerSingleConsumer.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>
#include "Common.h"
#include <conio.h>
#include <windows.h>
#include "SingleProducerSingleConsumer.h"

class MyThread1 : public Thread
{
	virtual void run()
	{
		std::cout << std::endl << "MyThread1::run, "<<"thread id = "<<std::to_string(std::this_thread::get_id().hash());
	}
};

class MyThread2 : public Thread
{
	virtual void run()
	{
		std::cout << std::endl << "MyThread2::run, " << "thread id = " << std::to_string(std::this_thread::get_id().hash());
	}
};



class SampleFifoProducer : public FifoQueueProducer < int >
{
	virtual boost::optional<int> getNextElement()
	{
		static int i = 0;
		std::cout << i << " Pushed in producer thread, ThreadId = " << std::to_string(std::this_thread::get_id().hash()) << std::endl;
		return i++;
	}
public:
	SampleFifoProducer(std::queue<int>& queue, std::mutex& mutex, std::condition_variable& cv)
		:FifoQueueProducer < int >(queue, mutex, cv)
	{
	}
};


class SampleFifoConsumer : public FifoQueueConsumer < int >
{
	void onNewItem(int item)
	{
		std::cout << item << " Received in the consumer thread, ThreadId = " << std::to_string(std::this_thread::get_id().hash()) << std::endl;
	}
	virtual void storeLocally(int item)
	{

	}
public:
	SampleFifoConsumer(std::queue<int>& queue, std::mutex& mutex, std::condition_variable& cv)
		:FifoQueueConsumer<int>(queue, mutex, cv)
	{
	}
};

boost::optional<int> push()
{
	static int i = 0;
	Sleep(1000);
	return ++i;
}

void process(int item)
{
	std::cout << std::endl << item << " Received from producer";
}

int main(int argc, char* argv[])
{
	std::queue<int> queue;
	std::mutex mutex;
	std::condition_variable cv;

	/*SampleSharedFifoQueueTimer pc(queue, mutex, cv);
	pc.start();*/

	SingleProducerSingleConsumer<int> pc(queue, mutex, cv, std::function<boost::optional<int>()>(push), std::function<void(int)>(process));
	pc.start();

	return 0;
}

