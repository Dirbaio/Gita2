#include "Character.hpp"
#include <cassert>
#include "SceneMain.hpp"

Character::Character(SceneMain* sc) : GameObject(sc)
{
    hasGoal = false;
    mark = MARK_NONE;
    vel = 16.0f;
}

void Character::move(vec2f posf)
{
    const vec2f &pos0 = position;

    vec2f sizs = vec2f(0.15, 0.07);
    vec2f cens = vec2f(0.07, 0.06);

    vec2f scen = sizs-cens;
    vec2f direction = posf - pos0;

    if (direction.y < 0) //Vamos hacia abajo
    {
        //le restamos a la Y la mitad de su tamaño para obtener la Y inferior del sprite
        int yo =     int(pos0.y - scen.y),
                yn = int(posf.y - scen.y),
                xl = int(pos0.x - cens.x + 2),
                xr = int(pos0.x + scen.x - 2);
        for (int y = yo; y >= yn; y--)
            for (int x = xl; x <= xr; x++)
                if (scene->map->solid(x,y) && onDownCollision(x, y))
                {
                    posf.y = int(y) + scen.y;
                    goto vert_exit;
                }

        noDownCollision();
    }
    else if (direction.y > 0) //Vamos hacia arriba
    {
        //le sumamos a la Y la mitad de su tamaño para obtener la Y superior del sprite
        int yo = int(pos0.y + cens.y),
                yn = int(posf.y + cens.y),
                xl = int(pos0.x - cens.x + 2),
                xr = int(pos0.x + scen.x - 2);
        for (int y = yo; y <= yn; y++)
            for (int x = xl; x <= xr; x++)
                if (scene->map->solid(x,y) && onUpCollision(x, y))
                {
                    posf.y = int(y)+1 - cens.y;
                    goto vert_exit;
                }

        noUpCollision();
    }
vert_exit:

    if (direction.x < 0) //Vamos hacia la izquierda
    {
        int xo = int(pos0.x - cens.x),
                xn = int(posf.x - cens.x),
                yb = int(pos0.y - scen.y + 2),
                yt = int(pos0.y + cens.y - 2);
        for (int x = xo; x >= xn; x--)
        {
            for (int y = yb; y <= yt; y++)
            {
                if (scene->map->solid(x,y) && onLeftCollision(x, y))
                {
                    posf.x = int(x)+1 +cens.x;
                    goto horz_exit;
                }
            }
        }

        noLeftCollision();
    }
    else if (direction.x > 0) //Vamos hacia la derecha
    {
        int xo = int(pos0.x + scen.x),
                xn = int(posf.x + scen.x),
                yb = int(pos0.y - scen.y + 2),
                yt = int(pos0.y + cens.y - 2);
        for (int x = xo; x <= xn; x++)
        {
            for (int y = yb; y <= yt; y++)
            {
                if (scene->map->solid(x,y) && onRightCollision(x, y))
                {
                    posf.x = int(x) - scen.x;
                    goto horz_exit;
                }
            }
        }

        noRightCollision();
    }
horz_exit:

    position = posf;
}

void Character::setGoal(vec2f goal) {
    this->goal = goal;

    vec2i from = vec2i(position);
    vec2i to = vec2i(goal);

    if(scene->map->solid(from.x, from.y)) {
        //cout<<"Pathfinding: Current pos is solid. "<<endl;
        return;
    }
    if(scene->map->solid(to.x, to.y)) {
        //cout<<"Pathfinding: Goal is solid."<<endl;
        return;
    }

    //Calculate the path!
    vector<vector<int> > vis(TILESIZE, vector<int>(TILESIZE, -1));

    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};

    bool end = false;

    queue<vec2i> q;
    q.push(to);
    while(!q.empty() && !end)
    {
        int x = q.front().x;
        int y = q.front().y;
        q.pop();

        for(int i = 0; i < 4; i++)
        {
            int x2 = x + dx[i];
            int y2 = y + dy[i];
            if(x2 < 0 || x2 >= TILESIZE) continue;
            if(y2 < 0 || y2 >= TILESIZE) continue;
            if(scene->map->solid(x2, y2)) continue;
            if(vis[x2][y2] != -1) continue;
            vis[x2][y2] = i;
            vec2i v2 (x2, y2);
            if(v2 == from)
                end = true;
            q.push(v2);
        }
    }

    if(vis[from.x][from.y] == -1) {
        cout<<"Pathfinding: ERROR NO PATH FOUND OMG OMG OMG"<<endl;
        return;
    }
    
    hasGoal = true;
    vector<vec2i> v;

    while(from != to)
    {
        v.push_back(from);
        vec2i from2 = from;
        from.x -= dx[vis[from2.x][from2.y]];
        from.y -= dy[vis[from2.x][from2.y]];
    }
    v.push_back(from);

    path = queue<vec2f>();

    vec2i ant(-1, -1);
    vec2f antf(16, 16);
    for(int i = 0; i < v.size(); i++)
    {
        vec2f p (v[i].x*64+Utils::randomInt(8, 56), v[i].y*64+Utils::randomInt(8, 56));
        if(v[i].x == ant.x)
            p.x = antf.x;
        if(v[i].y == ant.y)
            p.y = antf.y;
        ant = v[i];
        antf = p;
        path.push(p);
    }
    path.push(goal);
}

void Character::moveTowardsGoal()
{
    if(!hasGoal) return;

    while(!path.empty() && glm::distance(path.front(), position) < 2)
        path.pop();

    if(path.empty())
    {
        hasGoal = false;
        return;
    }

    vec2f to = path.front();
    moveInDir(to-position);
}

void Character::moveInDir(vec2f dir)
{
    if (glm::length(dir) == 0) return;
    dir = glm::normalize(dir);

    if(dir.x < -0.5f) faceDir = FACE_LEFT;
    if(dir.y < -0.5f) faceDir = FACE_UP;
    if(dir.x >  0.5f) faceDir = FACE_RIGHT;
    if(dir.y >  0.5f) faceDir = FACE_DOWN;

    float delta = scene->input.getFrameTime().asSeconds();
    move(position + dir*delta*vel);
}

bool Character::canSee(const vec2f& pos)
{

    vec2f dir_corpse = pos-position;
    vec2f dir_facing((float) dirInc[faceDir].x, (float) dirInc[faceDir].y);
    dir_corpse = glm::normalize(dir_corpse);
    dir_facing = glm::normalize(dir_facing);

    return glm::dot(dir_corpse,dir_facing) >= 0.0f && scene->city.visible(position, pos);
}


bool Character::onLeftCollision(int x, int j) {return true;}
bool Character::onRightCollision(int x, int j){return true;}
bool Character::onUpCollision(int x, int j)   {return true;}
bool Character::onDownCollision(int x, int j) {return true;}