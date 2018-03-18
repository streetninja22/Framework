#include <iostream>
#include <boost/filesystem.hpp>
#include "framework/assets.h"
#include "framework/InputSystem.h"
#include "framework/Event.h"
#include "framework/includeSDL.h"

class TestReturnType : public EventReturnType
{
public:
	TestReturnType() : EventReturnType(EventType::DEFAULT)
	{
	}

	int getValue()
	{
		return 1;
	}
};

class TestEvent : public Event
{
public:
	TestEvent() : Event(EventType::DEFAULT)
	{
	}

};

evnt::EventBus* bus = new evnt::EventBus();

EventReturnType* testFunction(Event* event)
{
	if (event->getEventType() == evnt::EventType::DEFAULT)
	{
		return new TestReturnType();
	}
	return nullptr;
}

EventReturnType* callKeypressEvent(Event* event)
{
	if (event->getEventType() == evnt::EventType::INPUT)
	{
		inpt::InputEvent* input = dynamic_cast<inpt::InputEvent*>(event);
		
		if (input->getInputType() == inpt::InputEventType::KEYBOARD)
		{
			inpt::KeyboardEvent* keyInput = dynamic_cast<inpt::KeyboardEvent*>(input);

			if (keyInput->getKeyCode() == inpt::KeyCode::KEYCODE_SPACE)
			{
				EventReturnType* value = bus->fireEventNow(new TestEvent);

				if (value != nullptr)
				{
					if (value->getType() == EventType::DEFAULT)
					{
						TestReturnType* valueCast = static_cast<TestReturnType*>(value);

						std::cout << valueCast->getValue() << "\n";
					}
				}
			}
		}
	}
	return nullptr;
}


int main(int argc, char** argv)
{
	SDL_Window* window = SDL_CreateWindow("test", 0, 0, 640, 640, SDL_WINDOW_SHOWN);
	
	inpt::InputSystem input(bus);
	
	bus->addListener(callKeypressEvent);
	bus->addListener(testFunction);
	
	while (true)
	{
		bus->callListeners();
		
		input.updateInput();
		
		
	}
	
	return 0;
}
