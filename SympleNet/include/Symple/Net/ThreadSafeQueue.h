#pragma once

#include "Symple/Net/Common.h"

namespace Symple::Net
{
	#define SYNCHRONIZED std::scoped_lock lock(m_Mutex)

	template<typename T>
	class ThreadSafeQueue final
	{
	private:
		std::mutex m_Mutex;
		std::deque<T> m_Deq;
	public:
		ThreadSafeQueue() = default;
		ThreadSafeQueue(const ThreadSafeQueue &) = delete;
		~ThreadSafeQueue()
		{ Clear(); }

		T &Front()
		{
			SYNCHRONIZED;
			return m_Deq.front();
		}

		T &Back()
		{
			SYNCHRONIZED;
			return m_Deq.back();
		}

		const T &Front() const
		{
			SYNCHRONIZED;
			return m_Deq.front();
		}

		const T &Back() const
		{
			SYNCHRONIZED;
			return m_Deq.back();
		}

		void PushBack(const T &item)
		{
			SYNCHRONIZED;
			m_Deq.emplace_back(std::move(item));
		}

		void PushFront(const T &item)
		{
			SYNCHRONIZED;
			m_Deq.emplace_fron(std::move(item));
		}

		bool IsEmpty()
		{
			SYNCHRONIZED;
			return m_Deq.empty();
		}

		void Clear()
		{
			SYNCHRONIZED;
			m_Deq.clear();
		}

		size_t Count()
		{
			SYNCHRONIZED;
			return m_Deq.size();
		}

		T PopFront()
		{
			SYNCHRONIZED;
			auto item = std::move(m_Deq.front());
			m_Deq.pop_front();
			return item;
		}

		T PopBack()
		{
			SYNCHRONIZED;
			auto item = std::move(m_Deq.back());
			m_Deq.pop_back();
			return item;
		}
	};

	#undef SYNCHRONIZED
}