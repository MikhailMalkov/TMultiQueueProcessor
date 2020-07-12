#pragma once
#include "IObservable.h"
#include <mutex>
#include <unordered_map>

template<typename Key, typename Value>
class Observable final :public IObservable<Key, Value>
{
	public:
		
		void Subscribe(Key id, std::shared_ptr<IConsumer<Key, Value>> consumer) override
		{
			std::lock_guard<std::mutex> lock{ m_mtx };
			if (!consumer)
				return;
			auto iter = m_consumers.find(id);
			if (iter == m_consumers.end())
			{
				m_consumers.insert(std::make_pair(id, consumer));
			}
		};
		
		void Unsubscribe(Key id) override
		{
			std::lock_guard<std::mutex> lock{ m_mtx };
			auto iter = m_consumers.find(id);
			if (iter != m_consumers.end())
				m_consumers.erase(id);
		};
		
		void SetState(const Key id, const Value& value) override
		{
			std::lock_guard<std::mutex> lock{ m_mtx };
			auto consumerIter = m_consumers.find(id);
			if (consumerIter != m_consumers.end())
				consumerIter->second->Consume(id, value);
		}
		
	private:
		
		std::unordered_map<Key, std::shared_ptr<IConsumer<Key, Value>>> m_consumers;
		std::mutex m_mtx;
};