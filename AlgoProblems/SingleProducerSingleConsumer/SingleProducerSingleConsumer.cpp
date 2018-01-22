// SingleProducerSingleConsumer.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>
#include "Common.h"
#include <conio.h>
#include <windows.h>

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


class SampleSharedFifoQueueTimer : public SharedFifoQueue < int >
{
	int m_num;
	virtual int push()
	{
		Sleep(1000);
		return ++m_num;
	}

	virtual void process(int item)
	{
		std::cout << std::endl<<item << " Received from producer";
	}

public:
	SampleSharedFifoQueueTimer(std::queue<int>& queue, std::mutex& mutex, std::condition_variable& cv) :
		SharedFifoQueue < int >(queue, mutex, cv)
	{
		m_num = 0;
	}
};


class SampleSharedFifoQueueThrottler : public SharedFifoQueue < int >
{
	int m_num;
	int m_limit;
	std::vector<int> m_localQueue;
	virtual int push()
	{
		Sleep(10);
		return ++m_num;
	}

	virtual void process(int item)
	{
		std::cout << std::endl << item << " Received from producer";
	}

public:
	SampleSharedFifoQueueThrottler(std::queue<int>& queue, std::mutex& mutex, std::condition_variable& cv) :
		SharedFifoQueue < int >(queue, mutex, cv)
	{
		m_num = 0;
	}
};

int main(int argc, char* argv[])
{
	/*
	queue.push(1);
	queue.push(1);
	queue.push(1);
	queue.push(1);
	queue.push(1);
	queue.push(1);
	queue.push(1);
	queue.push(1);
	std::mutex mutex;
	std::condition_variable cv;
	SampleFifoProducer p(queue,mutex,cv);
	SampleFifoConsumer c(queue, mutex, cv);

	p.start();
	c.start();
	p->join();
	c->join();*/

	std::queue<int> queue;
	std::mutex mutex;
	std::condition_variable cv;

	SampleSharedFifoQueueTimer pc(queue, mutex, cv);
	pc.start();

	return 0;
}

