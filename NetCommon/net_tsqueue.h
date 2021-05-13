#pragma once
#include "net_common.h"

namespace bbc
{
	namespace net
	{
		template<typename T>
		class tsqueue
		{
		public:
			tsqueue() = default;
			tsqueue(const tsqueue<T>&) = delete;
			virtual ~tsqueue() { clear(); }

		public:
			// returns and maintains item at the front of queue
			const T& front()
			{
				std::scoped_lock lock(muxQueue);
				return deQueue.front();
			}
			// returns and maintains item at the back of queue
			const T& back()
			{
				std::scoped_lock lock(muxQueue);
				return deQueue.back();
			}
			void push_front(const T& item)
			{
				std::scoped_lock lock(muxQueue);
				deQueue.emplace_front(std::move(item));
			}
			void push_back(const T& item)
			{
				std::scoped_lock lock(muxQueue);
				deQueue.emplace_back(std::move(item));
			}
			size_t count()
			{
				std::scoped_lock lock(muxQueue);
				return deQueue.size();
			}
			void clear()
			{
				std::scoped_lock lock(muxQueue);
				deQueue.clear();
			}
			bool empty()
			{
				std::scoped_lock lock(muxQueue);
				return deQueue.empty();
			}

			// remove AND return items from the front of the queue
			T pop_front()
			{
				std::scoped_lock lock(muxQueue);
				auto t = std::move(deQueue.front());
				deQueue.pop_front();
				return t;
			}
			// remove AND return items from the back of the queue
			T pop_front()
			{
				std::scoped_lock lock(muxQueue);
				auto t = std::move(deQueue.back());
				deQueue.pop_back();
				return t;
			}

		protected:
			std::mutex muxQueue;
			std::deque<T> deQueue;
		};
	}
}