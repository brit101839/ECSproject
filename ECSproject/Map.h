//
// Map.h

#ifndef __Map__
#define __Map__
#include "ECS/Sprite.h"
#include "textureManager.h"

#define Window_w_Size 1351
#define Window_h_Size 760

class Map
{
private:

	int _map[16][30];

	Sprite* _drit;
	Sprite* _grass;
	Sprite* _water;

public:

	Map();
	~Map();

	void LoadMap(int arr[16][30]);
	void DrawMap();

};

#endif // !__Map__