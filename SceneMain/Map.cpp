#include "Map.h"

Map::Map(SceneMain* scene) : GameObject(scene)
{
    int size = 30;
    tiles = std::vector<std::vector<Tile> > (size, std::vector<Tile>(size));

    for(int y = 0; y < size; y++)
        for(int x = 0; x < size; x++)
        {
            tiles[x][y].type = Map::Garden;
        }
}

Map::~Map()
{

}


void Map::update(float deltaTime)
{

}

void Map::draw() const
{

}

Map::Tile Map::getTile(int x, int y) const
{
	if(x < 0 || x >= getWidth() || y < 0 || y >= getHeight())
    {
        Tile t;
        t.type = Map::House;
        return t;
    }

    return tiles[x][y];
}
