#pragma once

#include "IConsumer.h"
#include <memory>
template<typename Key, typename Value>
class IObservable
{
	public:
		virtual void Subscribe(Key id, std::shared_ptr<IConsumer<Key, Value>> consumer) = 0;
		virtual void Unsubscribe(Key id) = 0;
		virtual void SetState(const Key id, const Value& value) = 0;
		virtual ~IObservable() = default;
};