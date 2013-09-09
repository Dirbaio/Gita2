#include "Npc.hpp"

#include "SceneMain.hpp"

Npc::Npc(SceneMain* scene) : Character(scene)
{
    this->hasGoal = false;
    this->mark = MARK_NONE;
}

void Npc::setGoal(vec2f goal) {
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
    vector<vector<int> > vis(scene->map->getWidth(), vector<int>(scene->map->getHeight(), -1));

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
            if(x2 < 0 || x2 >= scene->map->getWidth()) continue;
            if(y2 < 0 || y2 >= scene->map->getHeight()) continue;
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

        vec2f p (v[i].x + 0.5, v[i].y + 0.5);
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

vec2f Npc::dirTowardsGoal()
{
    if(!hasGoal) return vec2f(0, 0);

	while(!path.empty() && glm::distance(path.front(), position) < 0.2)
        path.pop();

    if(path.empty())
    {
        hasGoal = false;
        return vec2f(0, 0);
    }

    vec2f to = path.front();
    return to-position;
}
