#pragma once
#include "Common.h"

template <class T>
class MultiProducerSingleConsumerVersion1
{
	std::shared_ptr<bool> m_consumerWaitingFlag;
	std::vector<boost::shared_ptr<FifoQueueProducerPredicate<T>>> m_producers;
	boost::shared_ptr<FifoQueueConsumerPredicate<T>> m_consumer;
public:
	MultiProducerSingleConsumerVersion1(std::shared_ptr<std::queue<T>> queue,
										std::shared_ptr<std::mutex> mutex,
										std::shared_ptr<std::condition_variable> cv,
										const std::vector<std::function<T()>>& fn_producers,
										std::function<void(T)> fn_consumer
										)
		 :m_consumerWaitingFlag(std::shared_ptr<bool>(new bool(true))),
	{
		for (auto item : fn_producers)
			m_producers.push_back(boost::shared_ptr<FifoQueueProducerPredicate<T>>(new FifoQueueProducerPredicate<T>(queue, mutex, cv, item, m_consumerWaitingFlag)));

		m_consumer = boost::shared_ptr<FifoQueueConsumerPredicate<T>>(new FifoQueueConsumerPredicate<T>(queue, mutex, cv, fn_consumer, m_consumerWaitingFlag));
	}


	void start()
	{
		for (auto item : m_producers)
			item->start();

		m_consumer->start();

		for (auto item : m_producers)
			(*item)->join();

		(*m_consumer)->join();
	}
};


//template <class T>
//class MultiProducerSingleConsumerVersion2
//{
//	std::queue<boost::optional<int>>& m_eventQueue;
//	std::mutex* m_mutex;
//	std::condition_variable* m_cv;
//	std::vector<std::function<boost::optional<T>()>> m_fn_producers;
//	std::function<void(T)> m_fn_consumer;
//	std::vector<boost::shared_ptr<FifoQueueProducerPredicate<T>>> m_producers;
//	boost::shared_ptr<FifoQueueConsumerPredicate<T>> m_consumer;
//public:
//	MultiProducerSingleConsumerVersion2(std::queue<boost::optional<int>>& queue,
//										const std::vector<std::function<boost::optional<T>()>>& fn_producers,
//										std::function<void(T)> fn_consumer
//										) :
//										m_eventQueue(queue),
//										m_fn_producers(fn_producers),
//										m_fn_consumer(fn_consumer)
//	{
//	}
//
//
//	void start()
//	{
//		for (auto item : m_fn_producers)
//		{
//			FifoQueueProducerPredicate<T>* producerThread = new FifoQueueProducerPredicate<T>(m_eventQueue, m_mutex, m_cv, item);
//			producerThread->start();
//			m_producers.push_back(boost::shared_ptr<FifoQueueProducerPredicate<T>>(producerThread));
//		}
//
//		m_consumer = boost::shared_ptr<FifoQueueConsumerPredicate<T>>(new FifoQueueConsumerPredicate<T>(m_eventQueue, m_mutex, m_cv, m_fn_consumer));
//		m_consumer->start();
//
//		for (auto thread : m_producers)
//			(*thread)->join();
//
//		(*m_consumer)->join();
//	}
//};