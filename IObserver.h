#pragma once

class IObserver
{
	public:
		virtual void OnMessage() = 0;
		virtual ~IObserver() = default;
};

