#include <iostream>
#include <boost/filesystem.hpp>
#include "framework/assets.h"
#include "framework/InputSystem.h"
#include "framework/Event.h"
#include "framework/includeSDL.h"
#include "framework/GraphicsSystem.h"
#include "framework/GraphicsEvents.h"


int main(int argc, char** argv)
{
	evnt::EventBus* bus = new evnt::EventBus();
	
	inpt::InputSystem input(bus);

	gfx::GraphicsSystem graphics(bus, "Test", 0, 80, 640, 480);

	evnt::EventReturnType* returned = bus->fireEventNow(new gfx::LoadTextureEvent(file::getResourceDirectory("cryofreeze.jpg")));

	gfx::Texture* texture = static_cast<gfx::LoadTextureReturnType*>(returned)->getTexture();
	
	while (true)
	{
		bus->update();
		
		input.updateInput();
		
		graphics.update();

		gfx::RenderImageEvent* event = new gfx::RenderImageEvent(texture, NULL, NULL);

		bus->addEvent(event);
		
	}
	
	return 0;
}
