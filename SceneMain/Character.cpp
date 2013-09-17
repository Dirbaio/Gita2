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
	faceDir = FACE_DOWN;

	deadrot = Utils::randomFloat(0, 360);
	texName = "person";
	action = "Idle";
	drawDead = false;
}

void Character::ensureAnim(std::string name)
{
	if(name == currAnim) return;
	currAnim = name;
	anim.SelectAnim(name);
}

void Character::draw() const
{
	mat4f m(1.0);
	m = glm::translate(m, pos);
    //m = glm::rotate(m,GLOBALCLOCK.getElapsedTime().asSeconds()*50,vec3f(0,0,1));
    //m = glm::scale(m,scale);
	mat4f transform = scene->getState().projection*scene->getState().view*m;


	if(drawDead)
	{
		scene->bloodModel.program->uniform("modelViewProjectionMatrix")->set(transform);
		scene->bloodModel.program->uniform("color")->set(vec4f(0.9, 0.1, 0.2, 0.7));
		scene->bloodModel.draw();
	}

	scene->shadowModel.program->uniform("modelViewProjectionMatrix")->set(transform);
	scene->shadowModel.program->uniform("color")->set(vec4f(0.0, 0.0, 0.0, 0.3));
	scene->shadowModel.draw();

	if(drawDead)
	{
		transform = glm::translate(transform, vec3f(0, 0.03, 0));
		transform = glm::rotate(transform, deadrot, vec3f(0, 1, 0));
		transform = glm::rotate(transform, 90.0f, vec3f(0, 0, 1));
		transform = glm::rotate(transform, 90.0f, vec3f(0, 1, 0));
		transform = glm::translate(transform, vec3f(0, -0.3, 0));
	}
	else
	{
		transform = glm::rotate(transform, -26.0f, vec3f(1, 0, 0));
	}
	scene->personModel.program->uniform("modelViewProjectionMatrix")->set(transform);

	TextureManager::useTexture(texName, GL_TEXTURE2);
	scene->personModel.program->uniform("tex")->set(2);

	vec4f frame = vec4f(anim.getCurrentFrame());
	vec2i size = TextureManager::getTextureSize(texName);
	frame.x /= size.x;
	frame.y /= size.y;
	frame.z /= size.x;
	frame.w /= size.y;
	scene->personModel.program->uniform("texBounds")->set(frame);
	scene->personModel.draw();


}

void Character::update(float deltaTime)
{
    vec2f dir = moveCharacter(deltaTime);
    moveInDir(dir, deltaTime);

	string action2 = action;
	if(action2 == "Idle" && glm::length(dir) > 0.01)
		action2 = "Walking";

	if      (faceDir == FACE_UP)    ensureAnim(action2+"Up");
	else if (faceDir == FACE_DOWN)  ensureAnim(action2+"Down");
	else if (faceDir == FACE_LEFT)  ensureAnim(action2+"Left");
	else if (faceDir == FACE_RIGHT) ensureAnim(action2+"Right");

	anim.Update(deltaTime);
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
	vec2f dirCorpse = pos-position;
	if(glm::length(dirCorpse) > 12)
		return false;

	vec2f dirFacing((float) dirInc[faceDir].x, (float) dirInc[faceDir].y);
	dirCorpse = glm::normalize(dirCorpse);
	dirFacing = glm::normalize(dirFacing);

	return glm::dot(dirCorpse,dirFacing) >= 0.0f && scene->map->lineOfSight(position, pos);
}


bool Character::onLeftCollision(int x, int j) {return true;}
bool Character::onRightCollision(int x, int j){return true;}
bool Character::onUpCollision(int x, int j)   {return true;}
bool Character::onDownCollision(int x, int j) {return true;}
