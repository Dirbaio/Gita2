#include "Map.hpp"
#include "SceneMain.hpp"
#include "Game.hpp"
#include <cmath>

int randInt(int min, int max)
{
	return min+rand()%(max-min+1);
}


struct Street
{
		int pos;
		int sidewalk;
		int roadway;
		//width = sidewalk*2+roadway;
		//roadway = 0 => calle peatonal :D
};

std::vector<Street> generateStreets(int size)
{
	std::vector<Street> v;

	for(int i = 1; i < size; i++)
	{
		Street s;
		s.pos = i;
		s.sidewalk = randInt(0, 2);
		s.roadway = randInt(0, 4);
		v.push_back(s);
		i += s.sidewalk*2+s.roadway+16;
	}
	return v;
}

Map::Map(SceneMain* scene) : GameObject(scene)
{
	int sizex = 180;
	int sizey = 120;
	tiles = std::vector<std::vector<Tile> > (sizex, std::vector<Tile>(sizey));

	std::vector<Street> sx = generateStreets(sizex);
	std::vector<Street> sy = generateStreets(sizey);

	//Generate the map
	for(int y = 0; y < sizey; y++)
		for(int x = 0; x < sizex; x++)
			tile(x, y).type = Map::Garden;

	const int blockSizeMin = 4;
	const int blockSizeMax = 7;

	//Generate streets
	for(int y = blockSizeMin; y+blockSizeMin < sizey; y++)
	{
		int start = randInt(0, sizex/2-10);
		int end = randInt(sizex/2+10, sizex);
		for(int x = start; x < end; x++)
			tile(x, y).type = Map::Sidewalk;
		y += randInt(blockSizeMin, blockSizeMax);
	}

	for(int x = blockSizeMin; x+blockSizeMin < sizex; x++)
	{
		int start = randInt(0, sizey/2-10);
		int end = randInt(sizey/2+10, sizey);
		for(int y = start; y < end; y++)
			tile(x, y).type = Map::Sidewalk;
		x += randInt(blockSizeMin, blockSizeMax);
	}
	srand(time(0));

	//Generate the mesh
	std::vector<Vertex::Element> elements;
	elements.push_back(Vertex::Element(Vertex::Attribute::Position , Vertex::Element::Float, 3));
	elements.push_back(Vertex::Element(Vertex::Attribute::Color    , Vertex::Element::Float, 3));

	Vertex::Format format(elements);
	Mesh* mesh = new Mesh(format,0,false);

	struct Vertex {
			Vertex(vec3f pos, vec3f color) : pos(pos) , color(color) {}
			vec3f pos,color;
	};
	std::vector<Vertex> data;
	for(int y = 0; y < sizey; y++)
		for(int x = 0; x < sizex; x++)
		{
			vec3f color = vec3f(1.0, 1.0, 1.0);
			if((x+y)%2 == 0)
				color = vec3f(0.0, 1.0, 1.0);

			if(tiles[x][y].type == Map::Sidewalk)
				color = vec3f(0.5, 0.5, 1.0);

			data.push_back(Vertex(vec3f(x, 0,  y), color));
			data.push_back(Vertex(vec3f(x, 0,  y+1), color));
			data.push_back(Vertex(vec3f(x+1, 0,  y), color));
			data.push_back(Vertex(vec3f(x, 0,  y+1), color));
			data.push_back(Vertex(vec3f(x+1, 0,  y+1), color));
			data.push_back(Vertex(vec3f(x+1, 0,  y), color));
		}

	mesh->setVertexData(&data[0],data.size());
	model.mesh = mesh;
	model.program = parentScene->shaderExample;
}

Map::~Map()
{
	delete model.mesh;
}


void Map::update(float deltaTime)
{

}

void Map::draw() const
{
	mat4f transform = parentScene->getState().projection*parentScene->getState().view;
	model.program->uniform("modelViewProjectionMatrix")->set(transform);
	model.draw();
}

Map::Tile Map::tile(int x, int y) const
{
	assert(x >= 0 && x < getWidth() && y >= 0 && y < getHeight());
	return tiles[x][y];
}


Map::Tile& Map::tile(int x, int y)
{
	assert(x >= 0 && x < getWidth() && y >= 0 && y < getHeight());
	return tiles[x][y];
}
