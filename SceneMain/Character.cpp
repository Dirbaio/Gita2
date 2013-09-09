#include "Character.hpp"
#include <cassert>
#include "SceneMain.hpp"
#include "graphics/Vertex.hpp"
#include "graphics/Model.hpp"
#include "graphics/Mesh.hpp"
#include "../Game.hpp"

Character::Character(SceneMain* sc) : GameObject(sc)
{
    vel = 2.50f;
    position = vec2f(3, 3);

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
	data.push_back(Vertex(vec3f(-0.5, 0.0, 0), vec3f(0.0, 0.0, 1.0)));
	data.push_back(Vertex(vec3f( 0.5, 0.0, 0), vec3f(1.0, 0.0, 0.0)));
	data.push_back(Vertex(vec3f(-0.5, 2.0, 0), vec3f(0.0, 1.0, 0.0)));
	data.push_back(Vertex(vec3f( 0.5, 0.0, 0), vec3f(0.0, 0.0, 1.0)));
	data.push_back(Vertex(vec3f( 0.5, 2.0, 0), vec3f(1.0, 0.0, 0.0)));
	data.push_back(Vertex(vec3f(-0.5, 2.0, 0), vec3f(0.0, 1.0, 0.0)));

    mesh->setVertexData(&data[0],data.size());
    model.mesh = mesh;
    model.program = scene->shaderExample;
}

void Character::draw() const
{
	mat4f m(1.0);
	m = glm::translate(m, pos);
    //m = glm::rotate(m,GLOBALCLOCK.getElapsedTime().asSeconds()*50,vec3f(0,0,1));
    //m = glm::scale(m,scale);

    mat4f transform = scene->getState().projection*scene->getState().view*m;
    model.program->uniform("modelViewProjectionMatrix")->set(transform);
    model.draw();
}

void Character::update(float deltaTime)
{
    vec2f dir = moveCharacter(deltaTime);
    moveInDir(dir, deltaTime);
    pos = vec3f(position.x, 0, position.y);
}

vec2f Character::moveCharacter(float deltaTime)
{
    return vec2f(0, 0);
}

void Character::move(vec2f posf)
{
    const vec2f &pos0 = position;

    float rad = 0.3;
    float margin = 0.1;

    vec2f direction = posf - pos0;

    if (direction.y < 0) //Vamos hacia abajo
    {
        //le restamos a la Y la mitad de su tamaño para obtener la Y inferior del sprite
        int yo =     int(pos0.y - rad),
                yn = int(posf.y - rad),
                xl = int(pos0.x - rad+margin),
                xr = int(pos0.x + rad-margin);
        for (int y = yo; y >= yn; y--)
            for (int x = xl; x <= xr; x++)
                if (scene->map->solid(x,y) && onDownCollision(x, y))
                {
                    posf.y = int(y) + 1 + rad;
                    goto vert_exit;
                }

        noDownCollision();
    }
    else if (direction.y > 0) //Vamos hacia arriba
    {
        //le sumamos a la Y la mitad de su tamaño para obtener la Y superior del sprite
        int yo = int(pos0.y + rad),
                yn = int(posf.y + rad),
                xl = int(pos0.x - rad+margin),
                xr = int(pos0.x + rad-margin);
        for (int y = yo; y <= yn; y++)
            for (int x = xl; x <= xr; x++)
                if (scene->map->solid(x,y) && onUpCollision(x, y))
                {
                    posf.y = int(y) - rad;
                    goto vert_exit;
                }

        noUpCollision();
    }
vert_exit:

    if (direction.x < 0) //Vamos hacia la izquierda
    {
        int xo = int(pos0.x - rad),
                xn = int(posf.x - rad),
                yb = int(pos0.y - rad+margin),
                yt = int(pos0.y + rad-margin);
        for (int x = xo; x >= xn; x--)
            for (int y = yb; y <= yt; y++)
                if (scene->map->solid(x,y) && onLeftCollision(x, y))
                {
                    posf.x = int(x)+1 + rad;
                    goto horz_exit;
                }

        noLeftCollision();
    }
    else if (direction.x > 0) //Vamos hacia la derecha
    {
        int xo = int(pos0.x + rad),
                xn = int(posf.x + rad),
                yb = int(pos0.y - rad+margin),
                yt = int(pos0.y + rad-margin);
        for (int x = xo; x <= xn; x++)
            for (int y = yb; y <= yt; y++)
                if (scene->map->solid(x,y) && onRightCollision(x, y))
                {
                    posf.x = int(x) - rad;
                    goto horz_exit;
                }

        noRightCollision();
    }
horz_exit:

    position = posf;
}


static vec2i dirInc[] = {
    vec2i(0,-1),
    vec2i(0, 1),
    vec2i(-1,0),
    vec2i( 1,0)
};

void Character::moveInDir(vec2f dir, float deltaTime)
{
    if (glm::length(dir) == 0) return;
    dir = glm::normalize(dir);

    if(dir.x < -0.5f) faceDir = FACE_LEFT;
    if(dir.y < -0.5f) faceDir = FACE_UP;
    if(dir.x >  0.5f) faceDir = FACE_RIGHT;
    if(dir.y >  0.5f) faceDir = FACE_DOWN;

    vec2f dest = position + dir*deltaTime*vel;
    move(vec2f(position.x, dest.y));
    move(vec2f(dest.x, position.y));
}

bool Character::canSee(const vec2f& pos)
{
    vec2f dir_corpse = pos-position;
    vec2f dir_facing((float) dirInc[faceDir].x, (float) dirInc[faceDir].y);
    dir_corpse = glm::normalize(dir_corpse);
    dir_facing = glm::normalize(dir_facing);

    return glm::dot(dir_corpse,dir_facing) >= 0.0f; // && scene->city.visible(position, pos);
}


bool Character::onLeftCollision(int x, int j) {return true;}
bool Character::onRightCollision(int x, int j){return true;}
bool Character::onUpCollision(int x, int j)   {return true;}
bool Character::onDownCollision(int x, int j) {return true;}
