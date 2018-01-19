// SingleProducerSingleConsumer.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>
#include "Common.h"

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
	virtual int getNextElement()
	{
		static int i = 0;
		std::cout << i << " Pushed in producer thread, ThreadId = " << std::to_string(std::this_thread::get_id().hash()) << std::endl;
		return i++;
	}
public:
	SampleFifoProducer(std::vector<int>& queue, std::mutex& mutex, std::condition_variable& cv)
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
public:
	SampleFifoConsumer(std::vector<int>& queue, std::mutex& mutex, std::condition_variable& cv) 
		:FifoQueueConsumer<int>(queue, mutex, cv)
	{
	}
};


int main(int argc, char* argv[])
{
	std::vector<int> queue;
	queue.push_back(1);
	queue.push_back(1);
	queue.push_back(1);
	queue.push_back(1);
	queue.push_back(1);
	queue.push_back(1);
	queue.push_back(1);
	queue.push_back(1);
	std::mutex mutex;
	std::condition_variable cv;
	SampleFifoProducer p(queue,mutex,cv);
	SampleFifoConsumer c(queue, mutex, cv);

	p.start();
	c.start();
	p->join();
	c->join();

	return 0;
}

