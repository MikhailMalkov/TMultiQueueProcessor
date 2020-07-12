#pragma once

template<typename Key, typename Value>
class IMessageQueues
{
	virtual void PushMessage(Key id, Value value) = 0;
	virtual void PopMessage(Key id) = 0;
};