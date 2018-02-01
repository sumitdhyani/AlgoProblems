#pragma once
#include "Common.h"
#include <map>

template <class T>
struct FCallData
{
	typedef std::chrono::system_clock::time_point time_point;

	T m_obj;
	time_point m_lastSentTime;
	UINT m_assignedDelay;
	time_point m_exitTime;

	FCallData(T obj, time_point lastSentTime, UINT assignedDelay)
		:m_obj(obj),
		m_lastSentTime(lastSentTime),
		m_assignedDelay(assignedDelay)
	{
	}
};

template <class T>
struct comp
{
	bool operator()(const FCallData<T>& f1, const FCallData<T>& f2)
	{
		return (f1.m_exitTime < f2.m_exitTime);
	}
};

template <class T>
class returnSameThingWithDelay : public returnSameThing<T>
{
	typedef std::chrono::system_clock::time_point time_point;
	std::shared_ptr<returnSameThing> m_baseObject;
	std::shared_ptr<std::unique_lock<std::mutex>> m_lock;
	std::shared_ptr<std::condition_variable> m_cv;
	UINT m_delay;
public:
	boost::optional<T> operator()()
	{
		m_cv->wait_for(*m_lock, std::chrono::milliseconds(m_delay));
		return (*m_baseObject)();
	}

	returnSameThingWithDelay(T object, std::shared_ptr<returnSameThing<T>> baseObject, std::shared_ptr<std::unique_lock<std::mutex>> lock, std::shared_ptr<std::condition_variable> cv, UINT delay) :
							 returnSameThing<T>(object),
							 m_baseObject(baseObject),
							 m_lock(lock),
							 m_cv(cv),
							 m_delay(delay)
	{}
};



class TimerCaller
{
	typedef std::function<void()> VoidFunc;
	typedef FifoDelayedCaller<VoidFunc> Producer;
	typedef FifoQueueConsumerPredicate<VoidFunc> Consumer;
	typedef std::unique_lock<std::mutex> Lock;

	std::shared_ptr<std::queue<VoidFunc>> m_queue;
	std::map<long, std::shared_ptr<Producer>> m_producers;

	std::shared_ptr<Consumer> m_consumer;
	std::shared_ptr<bool> m_consumerWaitingFlag;
	std::shared_ptr<std::mutex> m_mutex;
	std::shared_ptr<std::condition_variable> m_cv;

public:
	TimerCaller(std::shared_ptr<std::queue<VoidFunc>> queue,
				std::shared_ptr<std::mutex> mutex,
				std::shared_ptr<std::condition_variable> cv
				)
				:m_queue(queue),
				 m_mutex(mutex),
				 m_cv(cv)
				 
	{
		m_consumerWaitingFlag = std::shared_ptr<bool>(new bool(true));

		m_consumer = std::shared_ptr<Consumer>(new Consumer(queue,
															std::shared_ptr<Lock>(new Lock(*mutex, std::defer_lock)),
															cv,
															callItAsAFunction < VoidFunc >,
															m_consumerWaitingFlag
															)
											   );
		m_consumer->start();
	}

	long addTimer(std::function<void()> func, UINT delay)
	{
		std::shared_ptr<Lock> lk(new Lock(*m_mutex, std::defer_lock));
		Lock lock(*m_mutex);
		std::shared_ptr<Producer> producer(new Producer( m_queue,
														 lk,
														 m_cv,
														 returnSameThing<VoidFunc>(func),
														 delay,
														 m_consumerWaitingFlag
													   )
										   );
		producer->start();
		long timerId = std::chrono::high_resolution_clock::now().time_since_epoch().count() + rand();//Add rand() to make sure the key is unique, in case this method is called very quickly at the same time
		m_producers[timerId] = producer;
		return timerId;
	}

	bool deleteTimer(long timerId)
	{
		auto searchResult = m_producers.find(timerId);
		if (m_producers.end() != searchResult)
		{
			searchResult->second->exit();
			return true;
		}
		else
			return false;

	}

	~TimerCaller()
	{
		std::unique_lock<std::mutex> lock(*m_mutex);
		for (auto it : m_producers)
			it.second->exit();

		m_consumer->exit();

		for (auto it : m_producers)
			(*it.second)->join();

		(*m_consumer)->join();
	};
};
