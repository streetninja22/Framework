#include <iostream>
#include <boost/filesystem.hpp>
#include "framework/assets.h"
#include "framework/InputSystem.h"
#include "framework/Event.h"
#include "framework/includeSDL.h"
#include "framework/GraphicsSystem.h"
#include "framework/GraphicsEvents.h"
#include "framework/SoundSystem.h"
#include "framework/soundevents.h"


//NOTE: in its current state, main.cpp is entirely used for testing



class GameSystem : public System
{

	sound::Chunk* m_soundEffect;
	
public:
	GameSystem(evnt::EventBus* bus) : System(bus)
	{
		sound::LoadChunkReturnType* returned = dynamic_cast<sound::LoadChunkReturnType*>(bus->fireEventNow(new sound::LoadChunkEvent(file::getResourceDirectory("sound/death.mp3"))));
		
		m_soundEffect = returned->getChunk();
	}
	
	
	
	EventReturnType* eventFired(Event* event) override
	{
		using namespace inpt;
		if (event->getEventType() == EventType::INPUT)
		{
			InputEvent* inputEvent = dynamic_cast<InputEvent*>(event);
			if (inputEvent->getInputType() == InputEventType::KEYBOARD)
			{
				KeyboardEvent* keyEvent = dynamic_cast<KeyboardEvent*>(inputEvent);
				if (keyEvent->getType() == KeyEventType::KEYDOWN)
				{
					switch (keyEvent->getKeyCode())
					{
						case KeyCode::KEYCODE_SPACE:
						{
							addEvent(new sound::ToggleMusicEvent());
							
							break;
						}
							
						case KeyCode::KEYCODE_P:
						{
							addEvent(new sound::PlayChunkEvent(m_soundEffect));
							
							break;
						}
					}
				}
			}
		}
		return nullptr;
	}
		
};




int main(int argc, char** argv)
{
	evnt::EventBus* bus = new evnt::EventBus();
	
	inpt::InputSystem input(bus);

	gfx::GraphicsSystem graphics(bus, "Test", 0, 80, 640, 480);
	
	sound::SoundSystem sound(bus);
	
	
	

	evnt::EventReturnType* returned = bus->fireEventNow(new gfx::LoadTextureEvent(file::getResourceDirectory("gfx/cryofreeze.jpg")));

	gfx::Texture* texture = static_cast<gfx::LoadTextureReturnType*>(returned)->getTexture();
	
	
	
	evnt::EventReturnType* fontReturn = bus->fireEventNow(new gfx::LoadFontEvent(file::getResourceDirectory("fonts/Roboto-Regular.ttf"), 28));
	
	gfx::Font* font = static_cast<gfx::LoadFontReturnType*>(fontReturn)->getFont();
	
	
	
	evnt::EventReturnType* textReturn = bus->fireEventNow(new gfx::LoadTextEvent("Hello, World!", font, {0, 0, 0, 255}));
	
	gfx::Texture* text = static_cast<gfx::LoadTextureReturnType*>(textReturn)->getTexture();
	
	
	
	GameSystem gameSystem(bus);
	
	
	sound::Music* music = static_cast<sound::LoadMusicReturnType*>(bus->fireEventNow(new sound::LoadMusicEvent(file::getResourceDirectory("sound/music.wav"))))->getMusic();
	
	bus->addEvent(new sound::PlayMusicEvent(music));
	
	while (true)
	{
		//This testing rig gets slowly more and more insane as time goes on
		bus->update();
		
		input.updateInput();
		
		graphics.update();

		gfx::RenderRotateImageEvent* event = new gfx::RenderRotateImageEvent(texture, NULL, NULL, 0, NULL, gfx::FLIP_VERTICAL);

		bus->addEvent(new gfx::SetViewportEvent(new gfx::Rect {240, 240, 240, 240}));
		bus->addEvent(event);
		bus->addEvent(new gfx::RenderImageEvent(text, NULL, new gfx::Rect{0, 100, 480, 280}));
		
	}
	
	return 0;
}
