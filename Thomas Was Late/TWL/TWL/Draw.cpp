#include "Engine.h"

void Engine::draw()
{
	m_Window.clear(Color::White);

	// Fullscreen
	if (!m_SplitScreen)
	{
		m_Window.setView(m_BGMainView);
		m_Window.draw(m_BackgroundSprite);
		m_Window.setView(m_MainView);

		m_Window.draw(m_VALevel, &m_TextureTiles);

		m_Window.draw(m_Thomas.getSprite());
		m_Window.draw(m_Bob.getSprite());
	}
	// Splitscreen
	else
	{
		// Draw Thomas side
		m_Window.setView(m_BGLeftView);
		m_Window.draw(m_BackgroundSprite);
		m_Window.setView(m_LeftView);

		m_Window.draw(m_VALevel, &m_TextureTiles);

		m_Window.draw(m_Bob.getSprite());
		m_Window.draw(m_Thomas.getSprite());

		// Draw Bobs side
		m_Window.setView(m_BGRightView);
		m_Window.draw(m_BackgroundSprite);
		m_Window.setView(m_RightView);

		m_Window.draw(m_VALevel, &m_TextureTiles);

		m_Window.draw(m_Thomas.getSprite());
		m_Window.draw(m_Bob.getSprite());
	}

	// Draw Hud
	m_Window.setView(m_HudView);
	m_Window.draw(m_HUD.getLevel());
	m_Window.draw(m_HUD.getTime());
	if (!m_Playing)
	{
		m_Window.draw(m_HUD.getMessage());
	}



	m_Window.display();
}