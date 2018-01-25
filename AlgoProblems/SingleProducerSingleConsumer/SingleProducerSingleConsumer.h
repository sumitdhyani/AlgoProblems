#pragma once
#include "Common.h"
#include <boost/smart_ptr/shared_ptr.hpp>

//The container which synchronizes the events between producer and consumer
//Inherit this class and override "push" and "process" methods
//Call the start method
template <class T>
class SingleProducerSingleConsumer
{
	boost::shared_ptr<FifoQueueProducerPredicate<T>> m_producer;
	boost::shared_ptr<FifoQueueConsumerPredicate<T>> m_consumer;
public:
	SingleProducerSingleConsumer(std::queue<T>& queue,
									std::mutex& mutex,
									std::condition_variable& cv,
									std::function<boost::optional<T>()>& fn_producer,
									std::function<void(T)> fn_consumer
									)
	{
		m_producer = boost::shared_ptr<FifoQueueProducerPredicate<T>>(new FifoQueueProducerPredicate<T>(queue, mutex, cv, fn_producer));
		m_consumer = boost::shared_ptr<FifoQueueConsumerPredicate<T>>(new FifoQueueConsumerPredicate<T>(queue, mutex, cv, fn_consumer));
	}


	void start()
	{
		m_producer->start();
		m_consumer->start();

		(*m_producer)->join();
		(*m_consumer)->join();
	}
};
