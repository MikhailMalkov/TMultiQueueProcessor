#pragma once

#include <thread>
#include <mutex>
#include <memory>
#include <functional>
#include <unordered_set>
#include <condition_variable>
#include <chrono>
#include <atomic>
#include "IConsumer.h"
#include "IObservable.h"
#include "IMessageQueue.h"


template<typename Key, typename Value>
class MultiQueueProcessor
{
	public:
		
		MultiQueueProcessor(std::unique_ptr<IObservable<Key, Value>> ptrObservable,
			std::unique_ptr < IMessageQueue<Key, Value>> ptrMessageQueue) final
			:m_ptrObservable(std::move(ptrObservable)),
			m_ptrMessageQueue(std::move(ptrMessageQueue)),
			m_bRunning{ true },
			m_thread(std::bind(&MultiQueueProcessor::Process, this))
		{
			if (!m_ptrObservable || !m_ptrMessageQueue)
				throw std::invalid_argument("m_ptrObservable and m_ptrMessageQueue have to be initialized");
		}

		~MultiQueueProcessor()
		{
			StopProcessing();
			if(m_thread.joinable())
				m_thread.join();
		}

		void StopProcessing()
		{
			std::lock_guard<std::mutex> lock{ m_mtx };
			m_bRunning = false;
			m_cv.notify_all();
		}

		void Subscribe(Key id, std::shared_ptr<IConsumer<Key, Value>> consumer)
		{
			std::lock_guard<std::mutex> lock{ m_mtx };
			if (!consumer)
				return;
			m_ptrObservable->Subscribe(id, consumer);
			m_subscribedIdsCach.insert(id);
		}

		void Unsubscribe(Key id)
		{
			std::lock_guard<std::mutex> lock{ m_mtx };
			m_ptrObservable->Unsubscribe(id);
			m_subscribedIdsCach.erase(id);
		}

		void Enqueue(Key id, Value value)
		{
			std::lock_guard<std::mutex> lock{ m_mtx };
			m_ptrMessageQueue->Enqueue(id, value);
			m_bReceivedMessage = true;
			m_cv.notify_all();
		}

		Value Dequeue(Key id)
		{
			std::lock_guard<std::mutex> lock{ m_mtx };
			m_ptrMessageQueue->Dequeue(id);
		}

private:
	
	void Process()
	{
		try
		{
			while (m_bRunning)
			{
				std::unique_lock<std::mutex> lock{ m_mtx };
				m_cv.wait(lock, [&]() { return m_bReceivedMessage || !m_bRunning; });

				if (m_bReceivedMessage)
				{
					for (auto const& id :m_subscribedIdsCach)
					{
						while (true)
						{
							if(auto value = m_ptrMessageQueue->Dequeue(id); value != Value{})
								m_ptrObservable->SetState(id, value);
							else
								break;
						}
						m_bReceivedMessage = false;
					}
				}
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what(); // std::cerr is not thread safe, only for the test task
		}
		catch (...)
		{
			std::cerr << " unknown error";
		}
	}

private:
	
	std::unique_ptr<IObservable<Key, Value>> m_ptrObservable;
	std::unique_ptr<IMessageQueue<Key, Value>> m_ptrMessageQueue;
	
	std::unordered_set<Key> m_subscribedIdsCach;
	
	std::atomic<bool> m_bRunning;
	bool m_bReceivedMessage;
	
	std::mutex m_mtx;
	std::thread m_thread;
	std::condition_variable m_cv;
};