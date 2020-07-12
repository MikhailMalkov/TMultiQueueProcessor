#pragma once

#include "IConsumer.h"
#include <string>
#include <iostream>

template<typename Key, typename Value>
class Consumer final : public IConsumer<Key, Value>
{
	public:
		Consumer(const std::string& name):m_name(name)
		{
		
		}
		
		virtual void Consume(Key id, const Value& value) override
		{
			//some payload
			// std::cout is not thread safety thing but for test task it is ok
			std::cout << "Consumer: " << m_name
				<< " got a message: " << value
				<< " from queue id: " << id << std::endl;
	    };
	
	private:
		const std::string m_name;
};