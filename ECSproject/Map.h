//
// Map.h

#ifndef __Map__
#define __Map__
#include "Sprite.h"
#include "readTileMap/readTileMap_json.h"
#include "ECS/ECS.h"

class Game;

class Map
{
private:

	Game& _game;
	int _MapWidth, _MapHeight;
	GLfloat _tileSize;

public:

	explicit Map(Game& gameInstance, std::string mapJASON);
	~Map();

	void loadTileMap(TileMapData tileMap);

	void DrawMap();
};

#endif // !__Map__