#pragma once

#include <algorithm>
#include <xfunctional>
#include <vector>
#include <iostream>	
#include <fstream>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <boost/optional/optional.hpp>


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


//OOP wrapper over std::thread
//Call the "start" method in the parent thread which will later call the "run" method later in the new thread
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

//Producer class
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
			bool consumerWaiting = isQueueEmpty();
			pushItem(getNextElement());
			//The element returned by "getNextElement" be NULL since it returns boost::optional<T>, so the quequ could still be empty
			if (consumerWaiting && !isQueueEmpty())
			{
				lk.unlock();
				m_cv.notify_one();
			}
		}
	}

	virtual bool isQueueEmpty() = 0;
	virtual void pushItem(boost::optional<T> queueItem) = 0;
	virtual boost::optional<T> getNextElement() = 0;
public:
	SingleProducer(std::mutex& mutex, std::condition_variable& cv)
		:m_mutex(mutex),
		m_cv(cv)
	{
	}
};

//Consumer class
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


//Producer which pushes elements in the shared queue in fifo fashion
template <class T>
class FifoQueueProducer : public SingleProducer < T >
{
	std::queue<T>& m_eventQueque;
	virtual bool isQueueEmpty()
	{
		return m_eventQueque.empty();
	}

	void pushItem(boost::optional<T> queueItem)
	{
		if (queueItem)
			m_eventQueque.push(*queueItem);
	}

public:
	FifoQueueProducer(std::queue<T>& queue, std::mutex& mutex, std::condition_variable& cv)
		:SingleProducer < T >(mutex,cv),
		m_eventQueque(queue)
	{
	}
};

//Consumer which pulls elements in the shared queue in fifo fashion
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




//Execute the predicate in producer thread
template <class T>
class FifoQueueProducerPredicate : public FifoQueueProducer < T >
{
	std::function<boost::optional<T>()> m_fn;
	virtual boost::optional<T> getNextElement()
	{
		return boost::optional<T>(m_fn());
	}

public:
	FifoQueueProducerPredicate(std::queue<T>& queue, std::mutex& mutex, std::condition_variable& cv, std::function<boost::optional<T>()> fn)
		:FifoQueueProducer < T >(queue, mutex, cv),
		m_fn(fn)
	{
	}
};


//Execute the predicate in consumer thread
template <class T>
class FifoQueueConsumerPredicate : public FifoQueueConsumer < T >
{
	std::function<void(T)> m_fn;
	virtual void onNewItem(T item)
	{
		m_fn(item);
	}

public:
	FifoQueueConsumerPredicate(std::queue<T>& queue, std::mutex& mutex, std::condition_variable& cv, std::function<void(T)> fn)
		:FifoQueueConsumer < T >(queue, mutex, cv),
		m_fn(fn)
	{
	}
};