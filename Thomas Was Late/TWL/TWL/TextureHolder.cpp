#include "TextureHolder.h"
#include <assert.h>

TextureHolder* TextureHolder::m_s_Instance = nullptr;

// Constructor that enforces a singleton pattern, can only be created once
TextureHolder::TextureHolder()
{
	assert(m_s_Instance == nullptr);
	m_s_Instance = this;
}

sf::Texture& TextureHolder::GetTexture(std::string const& filename)
{
	auto& map = m_s_Instance->m_Textures;

	auto keyValuePair = map.find(filename);

	// check if texture is in the map
	if (keyValuePair != map.end())
	{
		return keyValuePair->second;
	}
	else
	{
		auto& texture = map[filename]; // adds texture to map if not found
		texture.loadFromFile(filename);
		return texture;
	}
}