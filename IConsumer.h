#pragma once
#include "IObserver.h"

template<typename Key, typename Value>
class IConsumer 
{
	public:
		virtual void Consume(Key id, const Value& value) = 0;
		virtual ~IConsumer() = default;
}; 
