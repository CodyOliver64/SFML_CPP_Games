#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;

class HUD
{
private:
	Font m_Font;
	Text m_StartText;
	Text m_TimeText;
	Text m_LevelText;

public:
	HUD();
	Text getMessage();
	Text getLevel();
	Text getTime();

	void setLevel(String text);
	void setTime(String text);
};