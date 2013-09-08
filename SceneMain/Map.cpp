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

const int streetTypes[][2] = {
	{2, 0},
	{1, 2},
	{1, 3},
	{2, 4},
};
std::vector<Street> generateStreets(int size)
{
	std::vector<Street> v;

	for(int i = 1; i < size; i++)
	{
		int t = rand()%4;
		Street s;
		s.pos = i;
		s.sidewalk = streetTypes[t][0];
		s.roadway = streetTypes[t][1];
		i += s.sidewalk*2+s.roadway;
		if(i < size-1)
			v.push_back(s);

		i += randInt(16, 20);
	}
	return v;
}

Map::Map(SceneMain* scene) : GameObject(scene)
{
	int sizex = 180;
	int sizey = 120;
	tiles = std::vector<std::vector<Tile> > (sizex, std::vector<Tile>(sizey));

	std::vector<Street> sx = generateStreets(sizey);
	std::vector<Street> sy = generateStreets(sizex);

	//Generate the map
	for(int y = 0; y < sizey; y++)
		for(int x = 0; x < sizex; x++)
			tile(x, y).type = Map::Garden;

	int xmin = sy[0].pos;
	int xmax = sy[sy.size()-1].pos+sy[sy.size()-1].sidewalk*2+sy[sy.size()-1].roadway;
	int ymin = sx[0].pos;
	int ymax = sx[sx.size()-1].pos+sx[sx.size()-1].sidewalk*2+sx[sx.size()-1].roadway;

	for(int i = 0; i < sx.size(); i++)
	{
		Street& s = sx[i];
		int width = s.sidewalk*2+s.roadway;
		for(int x = xmin; x < xmax; x++)
			for(int y = 0; y < width; y++)
				tile(x, s.pos+y).type = Map::Sidewalk;
	}
	for(int i = 0; i < sy.size(); i++)
	{
		Street& s = sy[i];
		int width = s.sidewalk*2+s.roadway;
		for(int x = ymin; x < ymax; x++)
			for(int y = 0; y < width; y++)
				tile(s.pos+y, x).type = Map::Sidewalk;
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
