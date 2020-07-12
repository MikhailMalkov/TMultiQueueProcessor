#pragma once

template<typename Key, typename Value>
class IMultiQueueProcessor
{
	
	virtual void Subscribe(Key id, IConsumer<Key, Value>* consumer) = 0;
	virtual void Unsubscribe(Key id) = 0;
	
};











