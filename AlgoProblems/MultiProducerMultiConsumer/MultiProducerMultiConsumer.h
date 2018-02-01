#pragma once
#include "Common.h"
#include <boost/smart_ptr/shared_ptr.hpp>

template <class T>
class MultiProducerSingleConsumerVersion1
{
	std::shared_ptr<bool> m_consumerWaitingFlag;
	std::vector<std::shared_ptr<FifoQueueProducerPredicate<T>>> m_producers;
	std::shared_ptr<FifoQueueConsumerPredicate<T>> m_consumer;
	std::shared_ptr<std::mutex> m_mutex;
	std::shared_ptr<std::condition_variable> m_cv;
public:
	MultiProducerSingleConsumerVersion1(std::shared_ptr<std::queue<T>> queue,
										std::shared_ptr<std::mutex> mutex,
										std::shared_ptr<std::condition_variable> cv,
										const std::vector<std::function<T()>>& fn_producers,
										std::function<void(T)> fn_consumer
										)
										:m_consumerWaitingFlag(std::shared_ptr<bool>(new bool(true)))
	{
		m_mutex = mutex;
		m_cv = cv;
		for (auto item : fn_producers)
			m_producers.push_back(std::shared_ptr<FifoQueueProducerPredicate<T>>(new FifoQueueProducerPredicate<T>(queue, std::shared_ptr<std::unique_lock<std::mutex>>(new std::unique_lock<std::mutex>(*mutex, std::defer_lock)), cv, item, m_consumerWaitingFlag)));

		m_consumer = std::shared_ptr<FifoQueueConsumerPredicate<T>>(new FifoQueueConsumerPredicate<T>(queue, std::shared_ptr<std::unique_lock<std::mutex>>(new std::unique_lock<std::mutex>(*mutex, std::defer_lock)), cv, fn_consumer, m_consumerWaitingFlag));
	}


	void start()
	{
		for (auto thread : m_producers)
			thread->start();

		m_consumer->start();
	}

	~MultiProducerSingleConsumerVersion1()
	{
		for (auto thread : m_producers)
			thread->exit();
		m_cv->notify_all();

		m_consumer->exit();

		for (auto thread : m_producers)
			(*thread)->join();
		(*m_consumer)->join();

	}
};