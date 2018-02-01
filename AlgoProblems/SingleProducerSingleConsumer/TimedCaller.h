#pragma once
#include <boost/date_time/posix_time/posix_time.hpp>

template <class T>
class TimedCaller
{
	SingleProducerSingleConsumer<T>* m_pc;

	TimedCaller()
};

template <T>
struct TimeChecker
{
	namespace bpt = boost::posix_time;
	std::list <itemSentData> m_queue;
	struct itemSentData
	{
		T m_item;
		UINT m_interval;
		bpt::ptime m_lastSentTime;

		itemSentData(T item, UINT interval, bpt::ptime lastSentTime)
		{
			m_item = item;
			m_interval = interval;
			m_lastSentTime = lastSentTime;
		}
	};

	boost::optional<T> operator()
	{
		for (auto it : m_queue)
		{
			if ()
		}
	}

	void addItem(T item, UINT )
	{
		m_queue.push_b
	}
};