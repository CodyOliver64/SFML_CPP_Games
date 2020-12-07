#include "SFML/Graphics.hpp"
#include "ZombieArena.h"

int createBackground(VertexArray& rVa, IntRect arena)
{
	const int TILE_SIZE = 50;
	const int TILE_TYPES = 3;
	const int VERTS_IN_QUAD = 4;

	int worldWidth = arena.width / TILE_SIZE;
	int worldHeight = arena.height / TILE_SIZE; // divide by tile size to change pixel coords to tile coords

	rVa.setPrimitiveType(Quads);
	rVa.resize(worldWidth * worldHeight * VERTS_IN_QUAD);

	int currentVertex = 0;

	// Positions vertices in quad and assign texture
	for (int w = 0; w < worldWidth; w++)
	{
		for (int h = 0; h < worldHeight; h++)
		{
			// Vertices placed in quad
			rVa[currentVertex + 0].position = Vector2f(w * TILE_SIZE, h * TILE_SIZE);

			rVa[currentVertex + 1].position = Vector2f((w * TILE_SIZE) + TILE_SIZE, h * TILE_SIZE);

			rVa[currentVertex + 2].position = Vector2f((w * TILE_SIZE) + TILE_SIZE, (h * TILE_SIZE) + TILE_SIZE);

			rVa[currentVertex + 3].position = Vector2f((w * TILE_SIZE), (h * TILE_SIZE) + TILE_SIZE);

			// use wall texture at walls
			if (h == 0 || h == worldHeight - 1 || w == 0 || w == worldWidth - 1)
			{
				rVa[currentVertex + 0].texCoords = Vector2f(0, 0 + TILE_TYPES * TILE_SIZE);
				rVa[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 + TILE_TYPES * TILE_SIZE);
				rVa[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + TILE_TYPES * TILE_SIZE);
				rVa[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + TILE_TYPES * TILE_SIZE);
			}

			// Random floor texture
			else
			{
				srand((int)time(0) + h * w - h);
				int mudOrGrass = (rand() % TILE_TYPES);
				int verticalOffset = mudOrGrass * TILE_SIZE;

				rVa[currentVertex + 0].texCoords = Vector2f(0, 0 + verticalOffset);
				rVa[currentVertex + 1].texCoords = Vector2f(TILE_SIZE, 0 + verticalOffset);
				rVa[currentVertex + 2].texCoords = Vector2f(TILE_SIZE, TILE_SIZE + verticalOffset);
				rVa[currentVertex + 3].texCoords = Vector2f(0, TILE_SIZE + verticalOffset);
			}

			currentVertex = currentVertex + VERTS_IN_QUAD;
		}
	}



	return TILE_SIZE;

}