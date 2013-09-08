#include "Map.hpp"
#include "SceneMain.hpp"
#include "Game.hpp"
#include <cmath>

Map::Map(SceneMain* scene) : GameObject(scene)
{
	int size = 30;
	tiles = std::vector<std::vector<Tile> > (size, std::vector<Tile>(size));

	//Generate the map
	for(int y = 0; y < size; y++)
		for(int x = 0; x < size; x++)
		{
			tiles[x][y].type = Map::Garden;
		}

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
	for(int y = 0; y < size; y++)
		for(int x = 0; x < size; x++)
		{
			vec3f color = vec3f(1.0, 1.0, 1.0);
			if((x+y)%2 == 0)
				color = vec3f(0.0, 1.0, 1.0);
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
