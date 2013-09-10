#include "Map.hpp"
#include "SceneMain.hpp"
#include "Game.hpp"
#include <cmath>

using namespace std;



struct Street
{
		int pos;
		int sidewalk;
		int roadway;
		//width = sidewalk*2+roadway;
		//roadway = 0 => calle peatonal :D

		int start() { return pos; }
		int end() { return pos+sidewalk*2+roadway; }
		int sstart() { return pos+sidewalk; }
		int send() { return pos+sidewalk+roadway; }
		bool peatonal() { return roadway == 0; }
};

const int streetTypes[][2] = {
	{2, 0},
	{1, 2},
	{1, 3},
	{2, 4},
};
vector<Street> generateStreets(int size)
{
	vector<Street> v;

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

		i += Utils::randomInt(5, 10);
	}
	return v;
}

struct Apple {
		bool x, y;
};

Map::Map(SceneMain* scene) : GameObject(scene)
{
	int sizex = 180;
	int sizey = 120;
	tiles = vector<vector<Tile> > (sizex, vector<Tile>(sizey));

	vector<Street> sx = generateStreets(sizey);
	vector<Street> sy = generateStreets(sizex);

	//Generate the map
	for(int y = 0; y < sizey; y++)
		for(int x = 0; x < sizex; x++)
			tile(x, y).type = Map::Garden;

	int xmin = sy[0].pos;
	int xmax = sy[sy.size()-1].pos+sy[sy.size()-1].sidewalk*2+sy[sy.size()-1].roadway;
	int ymin = sx[0].pos;
	int ymax = sx[sx.size()-1].pos+sx[sx.size()-1].sidewalk*2+sx[sx.size()-1].roadway;

	vector<vector<Apple> > aa (sy.size(), vector<Apple>(sx.size()));
	for(int x = 0; x < sy.size(); x++)
		for(int y = 0; y < sx.size(); y++)
		{
			Apple& a = aa[x][y];
			if(x == 0)
				a.x = false;
			else
				a.x = Utils::randomBool(80);
			if(y == 0)
				a.y = false;
			else
				a.y = Utils::randomBool(80);
		}

	for(int x = 0; x < sy.size(); x++)
		for(int y = 0; y < sx.size(); y++)
		{
			Apple& a = aa[x][y];
			if(a.x)
			{
				int x1 = sy[x-1].start();
				int x2 = sy[x].end();
				int y1 = sx[y].start();
				int y2 = sx[y].end();
				for(int xx = x1; xx < x2; xx++)
					for(int yy = y1; yy < y2; yy++)
						tile(xx, yy).type = Map::Sidewalk;
			}
			if(a.y)
			{
				int y1 = sx[y-1].start();
				int y2 = sx[y].end();
				int x1 = sy[x].start();
				int x2 = sy[x].end();
				for(int xx = x1; xx < x2; xx++)
					for(int yy = y1; yy < y2; yy++)
						tile(xx, yy).type = Map::Sidewalk;
			}
		}

	for(int x = 0; x < sy.size(); x++)
		for(int y = 0; y < sx.size(); y++)
		{
			Apple& a = aa[x][y];
			if(a.x)
			{
				int x1 = sy[x-1].sstart();
				int x2 = sy[x].send();
				int y1 = sx[y].sstart();
				int y2 = sx[y].send();
				for(int xx = x1; xx < x2; xx++)
					for(int yy = y1; yy < y2; yy++)
						tile(xx, yy).type = Map::Roadway;
			}
			if(a.y)
			{
				int y1 = sx[y-1].sstart();
				int y2 = sx[y].send();
				int x1 = sy[x].sstart();
				int x2 = sy[x].send();
				for(int xx = x1; xx < x2; xx++)
					for(int yy = y1; yy < y2; yy++)
						tile(xx, yy).type = Map::Roadway;
			}
		}
	/*
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
	}*/

	srand(time(0));

	//Generate the mesh
	vector<Vertex::Element> elements;
	elements.push_back(Vertex::Element(Vertex::Attribute::Position , Vertex::Element::Float, 3));
	elements.push_back(Vertex::Element(Vertex::Attribute::Color    , Vertex::Element::Float, 3));

	Vertex::Format format(elements);
	Mesh* mesh = new Mesh(format,0,false);

	struct Vertex {
			Vertex(vec3f pos, vec3f color) : pos(pos) , color(color) {}
			vec3f pos,color;
	};
	vector<Vertex> data;
	for(int y = 0; y < sizey; y++)
		for(int x = 0; x < sizex; x++)
		{
			vec3f color = vec3f(0.3, 0.5, 0.1);
			if((x+y)%2 == 0)
				color = vec3f(0.2, 0.4, 0.1);

			if(tiles[x][y].type == Map::Sidewalk)
				color = vec3f(0.4, 0.4, 0.5);
			if(tiles[x][y].type == Map::Roadway)
				color = vec3f(0.2, 0.3, 0.3);

			data.push_back(Vertex(vec3f(x, 0,  y), color));
			data.push_back(Vertex(vec3f(x, 0,  y+1), color));
			data.push_back(Vertex(vec3f(x+1, 0,  y), color));
			data.push_back(Vertex(vec3f(x, 0,  y+1), color));
			data.push_back(Vertex(vec3f(x+1, 0,  y+1), color));
			data.push_back(Vertex(vec3f(x+1, 0,  y), color));
		}

	mesh->setVertexData(&data[0],data.size());
	model.mesh = mesh;
	model.program = scene->shaderColor;
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
	mat4f transform = scene->getState().projection*scene->getState().view;
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

vec2i Map::getRandomStreet() const
{
	bool repeat = true;
	int x, y;
	while(repeat)
	{
		x = Utils::randomInt(0, getWidth()-1);
		y = Utils::randomInt(0, getHeight()-1);
		repeat = tile(x, y).isSolid();
	}
	return vec2i(x, y);
}
