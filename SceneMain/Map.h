#ifndef MAP_H
#define MAP_H

#include "GameObject.hpp"

class Map : public GameObject
{
public:
    enum TileType {
        Roadway,
        Sidewalk,
        Crosswalk,
        Garden,
        House,
        Fence
    };

    struct Tile {
        TileType type;
        bool isSolid() const {
			return (type == House || type == Fence);
        }
    };

private:
    std::vector<std::vector<Tile> > tiles;

public:
    Map(SceneMain* scene);
    virtual ~Map();

    virtual void update(float deltaTime);
    virtual void draw() const;

    int getWidth() const {return tiles.size();}
    int getHeight() const {return tiles[0].size();}

    Tile getTile(int x, int y) const;
};


#endif // MAP_H
