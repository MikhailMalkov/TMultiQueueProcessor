#pragma once
#include "IMessageQueue.h"
#include <list>
#include <memory>
#include <unordered_map>
#include <mutex>

template<typename Key, typename Value>
class MessageQueue final : public IMessageQueue<Key, Value>
{
	public:
		static const int MaxCapacity = 1000;
		
		virtual void Enqueue(Key id, Value value) override
		{
			std::lock_guard<std::mutex> lock{ m_mtx };
			auto iter = m_queues.find(id);
			if (iter != m_queues.end())
			{
				if (iter->second.size() < MaxCapacity)
					iter->second.push_back(value);
			}
			else
			{
				m_queues.insert(std::make_pair(id, std::list<Value>()));
				iter = m_queues.find(id);
				if (iter != m_queues.end())
				{
					if (iter->second.size() < MaxCapacity)
						iter->second.push_back(value);
				}
			}
		};
		
		virtual Value Dequeue(Key id) override
		{
			std::lock_guard<std::mutex> lock{ m_mtx };
			auto iter = m_queues.find(id);
			if (iter != m_queues.end())
			{
				if (iter->second.size() > 0)
				{
					auto front = iter->second.front();
					iter->second.pop_front();
					return front;
				}
			}
			return Value{};
		};
	
	private:
		std::unordered_map<Key, std::list<Value>> m_queues;
		std::mutex m_mtx;
};
