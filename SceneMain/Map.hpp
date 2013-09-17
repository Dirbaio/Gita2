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
			Building,
			Fence,
			Grass,
			Water
		};

		struct Tile {
				TileType type;
				bool isSolid() const {
					return (type == Building || type == Fence || type == Water);
				}
				bool isGrass() const {
					return (type == Grass);
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
		Tile tile(vec2f p) const { return tile(int(p.x), int(p.y)); }
		Tile& tile(vec2f p) { return tile(int(p.x), int(p.y)); }
		Tile tile(vec2i p) const { return tile(int(p.x), int(p.y)); }
		Tile& tile(vec2i p) { return tile(int(p.x), int(p.y)); }
		inline bool validTile(int x, int y) const {
			return x >= 0 && x < getWidth() && y >= 0 && y < getHeight();
		}
		inline bool validTile(vec2i p) const {
			return validTile(p.x, p.y);
		}

        bool solid(int x, int y) const {return tile(x, y).isSolid(); }
        bool solid(float x, float y) const {return solid(int(x), int(y)); }
		bool lineOfSight(vec2f from, vec2f to) const;

		vec2i getRandomStreet() const;
	private:
		void placeHouses();
		void placeHouses2();
		bool placeHouse(int type);
		void placeHouse(int x, int y, int type);
		bool houseFitsAt(int x, int y, int type);
		std::vector<std::vector<Tile> > tiles;
		Model model;
};


#endif // MAP_H
