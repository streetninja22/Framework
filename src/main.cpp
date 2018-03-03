#include <iostream>
#include <boost/filesystem.hpp>
#include "framework/assets.h"
#include "framework/InputSystem.h"
#include "framework/Event.h"
#include "framework/includeSDL.h"

void printInputEvent(Event* event)
{
	if (event->getEventType() == evnt::EventType::INPUT)
	{
		inpt::InputEvent* input = static_cast<inpt::InputEvent*>(event);
		
		std::cout << input->getInputType() << " ";
		
		
		if (input->getInputType() == inpt::InputEventType::KEYBOARD)
		{
			inpt::KeyboardEvent* keyEvent = static_cast<inpt::KeyboardEvent*>(input);
			
			std::cout << keyEvent->getKeyCode();
		}
		else if (input->getInputType() == inpt::InputEventType::MOUSE)
		{
			inpt::MouseEvent* mouse = static_cast<inpt::MouseEvent*>(input);
			
			std::cout << mouse->getX() << ", " << mouse->getY();
		}
		
		std::cout << '\n';
	}
}

int main(int argc, char** argv)
{
	SDL_Window* window = SDL_CreateWindow("test", 0, 0, 640, 640, SDL_WINDOW_SHOWN);
	
	
	evnt::EventBus* bus = new evnt::EventBus();
	
	inpt::InputSystem input(bus);
	
	bus->addListener(printInputEvent);
	
	while (true)
	{
		bus->callListeners();
		
		input.updateInput();
		
		
	}
	
	return 0;
}
