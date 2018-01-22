#pragma once

#include <algorithm>
#include <xfunctional>
#include <vector>
#include <iostream>	
#include <fstream>
#include <thread>
#include <queue>
//#include <winnt.h>

#if !defined(UINT)
typedef unsigned int UINT;
#endif

#if !defined(INT)
typedef int INT;
#endif


template <class T, class comp = std::less<T> >
class BinaryHeap
{
	std::vector<T> _arr;
	UINT _size;

	UINT getParentIndex(UINT index)
	{
		if (0 == index)
			return 0;
		else
			return ((index - 1) / 2);
	}

	UINT getLeftChildIndex(UINT index)
	{
		return (index * 2 + 1);
	}

	UINT getRightChildIndex(UINT index)
	{
		return (index * 2 + 2);
	}

public:
	BinaryHeap()
	{
		_size = 0;
	}

	~BinaryHeap(){};

	bool empty()
	{
		return (_size == 0);
	}

	void insert(T val)
	{
		if (_arr.size() >= ++_size)
			_arr[_size - 1] = val;
		else
			_arr.push_back(val);

		UINT currIndex = _size - 1;
		UINT  parentIndex = getParentIndex(currIndex);
		while (comp()(_arr[currIndex], _arr[parentIndex]))
		{
			std::swap(_arr[currIndex], _arr[parentIndex]);
			currIndex = parentIndex;
			parentIndex = getParentIndex(currIndex);
		}
	}

	void removeTop()
	{
		if (empty())
			return;

		_arr[0] = _arr[_size - 1];
		--_size;

		UINT currIndex = 0;

		bool carryOn = true;
		while (carryOn)
		{
			UINT left = (getLeftChildIndex(currIndex) < _size) ? getLeftChildIndex(currIndex) : currIndex;
			UINT right = (getRightChildIndex(currIndex) < _size) ? getRightChildIndex(currIndex) : currIndex;

			UINT indexToCompare = comp()(_arr[left], _arr[right]) ? left : right;
			if (comp()(_arr[indexToCompare], _arr[currIndex]))
			{
				std::swap(_arr[indexToCompare], _arr[currIndex]);
				currIndex = indexToCompare;
			}
			else
				carryOn = false;
		}
	}

	const T& getExtreme() const
	{
		if (_size > 0)
			return _arr[0];
		else
			throw std::runtime_error("Heap is empty and top is being asked");
	}

	UINT getSize() const
	{
		return _size;
	}
};


typedef BinaryHeap<UINT, std::less<UINT>> MinHeap;
typedef BinaryHeap<UINT, std::greater<UINT>> MaxHeap;



class Thread
{
	std::thread* m_thread;
	bool m_ThreadRunning;
	virtual void run() = 0;
public:

	void start()
	{
		m_thread = new std::thread(&Thread::run,this);
	}

	std::thread* operator->()
	{
		if (NULL == m_thread)
			throw std::runtime_error("Thread not started yet");
		else
			return m_thread;
	}

	virtual ~Thread()
	{
		delete m_thread;
	}
};

template <class T>
class SingleProducer : public Thread
{
	std::mutex& m_mutex;
	std::condition_variable& m_cv;

	virtual void run()
	{
		while (true)
		{
			std::unique_lock<std::mutex> lk(m_mutex);
			bool consumerWaiting = false;
			if (isQueueEmpty())
				consumerWaiting = true;
			pushItem(getNextElement());
			if (consumerWaiting)
			{
				lk.unlock();
				m_cv.notify_one();
			}
		}
	}

	virtual bool isQueueEmpty() = 0;
	virtual void pushItem(T queueItem) = 0;
	virtual T getNextElement() = 0;
public:
	SingleProducer(std::mutex& mutex, std::condition_variable& cv)
		:m_mutex(mutex),
		m_cv(cv)
	{
	}
};

template <class T>
class SingleConsumer : public Thread
{
	std::mutex& m_mutex;
	std::condition_variable& m_cv;
	virtual void run()
	{
		while (true)
		{
			std::unique_lock<std::mutex> lk(m_mutex);
			if (isQueueEmpty())
				m_cv.wait(lk);

			while (!isQueueEmpty())
				storeLocally(pullItem());
			lk.unlock();

			processLocalQueueElements();
		}
	}

	virtual void storeLocally(T item) = 0;
	virtual void processLocalQueueElements() = 0;
	virtual bool isQueueEmpty() = 0;
	virtual T pullItem() = 0;
public:
	SingleConsumer(std::mutex& mutex, std::condition_variable& cv)
		:m_mutex(mutex),
		 m_cv(cv)
	{
	}
};


template <class T>
class FifoQueueProducer : public SingleProducer < T >
{
	std::queue<T>& m_eventQueque;
	virtual bool isQueueEmpty()
	{
		return m_eventQueque.empty();
	}

	void pushItem(T queueItem)
	{
		m_eventQueque.push(queueItem);
	}

	virtual T pullItem()
	{
		T item = m_eventQueque.front();
		m_eventQueque.pop();
		return item;
	}

public:
	FifoQueueProducer(std::queue<T>& queue, std::mutex& mutex, std::condition_variable& cv)
		:SingleProducer < T >(mutex,cv),
		m_eventQueque(queue)
	{
	}
};

template <class T>
class FifoQueueConsumer : public SingleConsumer < T >
{
	std::queue<T>& m_eventQueque;
	std::queue<T> m_localEventQueque;
	virtual void onNewItem(T item) = 0;
	virtual bool isQueueEmpty()
	{
		return m_eventQueque.empty();
	}

	virtual void storeLocally(T item)
	{
		m_localEventQueque.push(item);
	}

	virtual void processLocalQueueElements()
	{
		while (!m_localEventQueque.empty())
		{
			onNewItem(m_localEventQueque.front());
			m_localEventQueque.pop();
		}
	}

	virtual T pullItem()
	{
		T item = m_eventQueque.front();
		m_eventQueque.pop();
		return item;
	}

public:
	FifoQueueConsumer(std::queue<T>& queue, std::mutex& mutex, std::condition_variable& cv)
		:SingleConsumer < T >(mutex, cv),
		m_eventQueque(queue)
	{
	}
};




template <class T>
class SharedFifoQueue
{
	
	class FifoQueueProducerShared : public FifoQueueProducer<T>
	{
		SharedFifoQueue<T>& m_fn;
		virtual T getNextElement()
		{
			return m_fn.push();
		}

	public:
		FifoQueueProducerShared(SharedFifoQueue<T>& fn, std::queue<T>& queue, std::mutex& mutex, std::condition_variable& cv) :
			FifoQueueProducer<T>(queue, mutex, cv),
			m_fn(fn)
		{}

	};

	class FifoQueueConsumerShared : public FifoQueueConsumer<T>
	{
		SharedFifoQueue<T>& m_fn;

		virtual void onNewItem(T item)
		{
			m_fn.process(item);
		}

	public:
		FifoQueueConsumerShared(SharedFifoQueue<T>& fn, std::queue<T>& queue, std::mutex& mutex, std::condition_variable& cv) :
			FifoQueueConsumer<T>(queue, mutex, cv),
			m_fn(fn)
		{}
	};


	std::queue<T>& m_eventQueue;
	std::mutex& m_mutex;
	std::condition_variable& m_cv;
	FifoQueueProducerShared* m_producer;
	FifoQueueConsumerShared* m_consumer;
public:

	virtual T push() = 0;
	virtual void process(T item) = 0;

	SharedFifoQueue(std::queue<T>& queue, std::mutex& mutex, std::condition_variable& cv):
		m_eventQueue(queue),
		m_mutex(mutex),
		m_cv(cv)
	{
		m_producer = new FifoQueueProducerShared(*this, m_eventQueue, m_mutex, m_cv);
		m_consumer = new FifoQueueConsumerShared(*this, m_eventQueue, m_mutex, m_cv);
	}

	void start()
	{
		m_producer->start();
		m_consumer->start();

		(*m_producer)->join();
		(*m_consumer)->join();
	}

	~SharedFifoQueue()
	{
		delete m_producer;
		delete m_consumer;
	}
};