#pragma once
#include "Common.h"
#include <stdlib.h>
#include <boost/smart_ptr/shared_ptr.hpp>





template <class T>
class TimedCaller
{
	std::shared_ptr<bool> m_consumerWaitingFlag;
	std::vector<std::shared_ptr<FifoDelayedCaller<T>>> m_producers;
	std::shared_ptr<FifoQueueConsumerPredicate<T>> m_consumer;
	std::shared_ptr<std::mutex> m_mutex;
	std::shared_ptr<std::condition_variable> m_cv;
public:
	TimedCaller(std::shared_ptr<std::queue<T>> queue,
				std::shared_ptr<std::mutex> mutex,
				std::shared_ptr<std::condition_variable> cv,
				const std::vector<std::pair<T,UINT>>& fn_producers
				)
				:m_consumerWaitingFlag(std::shared_ptr<bool>(new bool(true)))
	{
		m_mutex = mutex;
		m_cv = cv;
		for (auto item : fn_producers)
			m_producers.push_back(std::shared_ptr<FifoDelayedCaller<T>>(new FifoDelayedCaller<T>(queue, std::shared_ptr<std::unique_lock<std::mutex>>(new std::unique_lock<std::mutex>(*mutex, std::defer_lock)), cv, returnSameThing<T>(item.first), item.second, m_consumerWaitingFlag) ));

		m_consumer = std::shared_ptr<FifoQueueConsumerPredicate<T>>(new FifoQueueConsumerPredicate<T>(queue, std::shared_ptr<std::unique_lock<std::mutex>>(new std::unique_lock<std::mutex>(*mutex, std::defer_lock)), cv, callItAsAFunction<T>, m_consumerWaitingFlag));
	}


	void start()
	{
		for (auto thread : m_producers)
			thread->start();

		m_consumer->start();
	}

	~TimedCaller() 
	{
		for (auto thread : m_producers)
			thread->exit();
		m_cv->notify_all();

		m_consumer->exit();

		for (auto thread : m_producers)
			(*thread)->join();
		(*m_consumer)->join();
	};
};


typedef TimedCaller<std::function<void()>> TimedCallerV1;