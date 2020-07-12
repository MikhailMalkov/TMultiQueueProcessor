#pragma once

template<typename Key, typename Value>
class IMessageQueue 
{
	public:
		virtual void Enqueue(Key id, Value value) = 0;
		virtual Value Dequeue(Key id) = 0;
		virtual ~IMessageQueue() = default;
};
