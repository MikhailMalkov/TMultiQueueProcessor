#include "TConsumer.h"
#include "TObservable.h"
#include "TMessageQueue.h"
#include "TMultiQueueProcessor.h"

int main()
{
	try 
	{
		MultiQueueProcessor<int, int> processor(
			std::make_unique<Observable<int, int>>(),
			std::make_unique<MessageQueue<int, int>>());
		
		std::shared_ptr<Consumer<int, int>> consumer_1 = std::make_shared<Consumer<int, int>>("Vasya");
		std::shared_ptr <Consumer<int, int>> consumer_2 = std::make_shared<Consumer<int, int>>("Vova");
		std::shared_ptr<Consumer<int, int>> consumer_3 = std::make_shared<Consumer<int, int>>("Masha");
		std::shared_ptr <Consumer<int, int>> consumer_4 = std::make_shared<Consumer<int, int>>("Sasha");
		
		processor.Subscribe(1, consumer_1);
		processor.Subscribe(2, consumer_2);
		processor.Subscribe(3, consumer_3);
		processor.Subscribe(4, consumer_4);
		
		for (int i = 1; i < MessageQueue<int, int>::MaxCapacity; ++i)// message int(0) is not handled, due to checking: value != Value{}
		{
			processor.Enqueue(1, i);
			processor.Enqueue(2, i);
			processor.Enqueue(3, i);
			processor.Enqueue(4, i);
		}
	}
	catch (const std::exception& e) 
	{
		std::cerr << e.what(); // std::cerr is not thread safe, only for test task
	}
	catch (...)
	{
		std::cerr << " unknown exception ";
	}
	return 0;
}