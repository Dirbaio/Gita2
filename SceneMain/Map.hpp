#ifndef MAP_H
#define MAP_H

#include "GameObject.hpp"
#include "graphics/Model.hpp"
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

		Map(SceneMain* scene);
		virtual ~Map();

		virtual void update(float deltaTime);
		virtual void draw() const;

		int getWidth() const {return tiles.size();}
		int getHeight() const {return tiles[0].size();}

		Tile tile(int x, int y) const;
		Tile& tile(int x, int y);

	private:
		std::vector<std::vector<Tile> > tiles;
		Model model;
};


#endif // MAP_H
