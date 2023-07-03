#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <windows.h>
#include <cmath>
#include <vector>
#include <ctime>
#include <random>

#define move_y 1
#define move_x 1


using namespace std;

Uint32 getPixelColor(SDL_Surface* surface, int x, int y)
{
    Uint8* pixel = (Uint8*)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;
    Uint8 r, g, b;
    SDL_GetRGB(*(Uint32*)pixel, surface->format, &r, &g, &b);
    return SDL_MapRGB(surface->format, r, g, b);
}

int rand_value(int a, int b)
{
 random_device rd;
 unsigned seed = rd() ^ static_cast<unsigned>(time(nullptr));
 mt19937 engine(seed);
 uniform_int_distribution<int> distribution(a, b);
  int c = distribution(engine);
   return c;
}

int dist(int x, int y, int x1, int y1)
{
    int diff1 = pow(x1 - x, 2);
    int diff2 = pow(y1 - y, 2);
    return sqrt(diff2 + diff1);
}

int min_d(int arr[])
{ 
    int mini = 0, count = 0;
    int i = 1;
    while (i < 2)
    {
        if (arr[mini] >= 0)
        {
            if (arr[i] < arr[mini] && arr[i] != -1)
            {
                mini = i;
                count++;
            }
            i++;
            continue;
        }
        mini++;
    }
    if (count == 0)
    {
        return -1;
    }
    return mini;
}

bool ispath(int x, int y, SDL_Surface* map_)
{
    Uint32 pixelcolor;
    Uint8 r, g, b;
    pixelcolor = getPixelColor(map_, y, x);
    SDL_GetRGB(pixelcolor, map_->format, &r, &g, &b);
    int blue[3] = { 0, 0, 0 };
    if (int(r) == blue[0] && int(g) == blue[1] && int(b) == blue[2])
    {
        return true;
    }
    return false;
}

void make_array(int arr[446][494], SDL_Surface* map_)
{
    for (int i = 0; i < 446; i++)
    {
        for (int j = 0; j < 494; j++)
        {
            if (ispath(i, j, map_))
            {
                arr[i][j] = 1;
            }
            else
            {
                arr[i][j] = 0;
            }
        }
    }
}


bool findpath(int& x, int& y, int x1, int y1, SDL_Surface* map_)
{
    int dist_arr[2];
    static int flag = 0;
    static int count = 20;
    vector<int[2]> v(1);
    vector<int> visited_x(1);
    vector<int> visited_y(1);
    bool foundpath = false;
    bool isright = false, isleft = false, isup = false, isdown = false;
    v[0][0] = -1; v[0][1] = -1;
    cout << count << endl;
    //Sleep(80);
    if (x == x1 && y == y1)
    {
        return true;
    }
    else
    {
        if (count % 20 == 0)
        {
            //flag = rand() % 4;
            flag = rand_value(0, 3);
        }
        if (flag == 0)
        {
            isright = true;
            isleft = isup = isdown = false;
        }
        else if (flag == 1)
        {
            isleft = true;
            isright = isup = isdown = false;
        }
        else if (flag == 2)
        {
            isup = true;
            isleft = isright = isdown = false;
        }
        else
        {
            isdown = true;
            isleft = isup = isright = false;
        }
        if (isright)
        {
            if (ispath(y, x + 23, map_) && ispath(y + 23, x + 23, map_) && ispath(y + 11, x + 23, map_))
            {
                x++;
            }
            else
            {
                isright = false;
                //flag = rand() % 4;
                flag = rand_value(1, 3);
            }
        }
        else if (isleft)
        {
            if (ispath(y, x - 1, map_) && ispath(y + 23, x - 1, map_) && ispath(y + 11, x - 1, map_))
            {
                x--;
            }
            else
            {
                isleft = false;
                //flag = rand() % 4;
                flag = rand_value(0, 3);
                flag == 1 ? flag = 0 : flag = flag;
            }
        }
        else if (isup)
        {
            if (ispath(y - 1, x, map_) && ispath(y - 1, x + 23, map_) && ispath(y - 1, x + 11, map_))
            {
                y--;
            }
            else
            {
                isup = false;
                flag = rand_value(0, 3);
                flag == 2 ? flag = 1 : flag = flag;
                //flag = rand() % 4;
            }
        }
        else
        {
            if (ispath(y + 23, x, map_) && ispath(y + 23, x + 23, map_) && ispath(y + 23, x + 11, map_))
            {
                y++;
            }
        }
        count += rand_value(1, 10);
    }
    ////////////////////////////////////////////////////////////////////////////////////////////

/*     if (x > x1)
        {
            x--;
            return true;
        }
        else if ((ispath(y, x + 1, map_)) && (x + 1 >= 0 && x + 1 <= 3) &&
            (y != v[0][0] || x + 1 != v[0][1]))
        {
            dist_arr[0] = dist(x1, y1, x + 1, y);
            foundpath = true;
        }
        else
        {
            dist_arr[0] = -1;
        }
        if (y > y1)
        {
            y--;
            return true;
        }
        else if ((ispath(y + 1, x, map_)) && (y + 1 >= 0 && y + 1 <= 3) &&
            (y + 1 != v[0][0] || x != v[0][1]))
        {
            dist_arr[1] = dist(x1, y1, x, y + 1);
            foundpath = true;
        }
        else
        {
            dist_arr[1] = -1;
        }
        if (!foundpath)
        {

            v[0][0] = y;
            v[0][1] = x;
            x = visited_x[visited_x.size() - 1];
            y = visited_y[visited_y.size() - 1];
            visited_x.pop_back();
            visited_y.pop_back();
        }
        else
        {
            switch (min_d(dist_arr))
            {
            case 0:
                visited_x.push_back(x);
                visited_y.push_back(y);
                x++;
                break;
            case 1:
                visited_x.push_back(x);
                visited_y.push_back(y);
                y++;
                break;
            }
        }
        foundpath = false;
    }
    return false;
} */
}



void print_map(int** map_, int row, int col)
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            cout << map_[i][j] << " ";
        }
        cout << "\n";
    }
}

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win = SDL_CreateWindow("Pac Man", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 500, 500, 0);
    SDL_Surface* window_surface = SDL_GetWindowSurface(win);
    SDL_Surface* bg = IMG_Load("black_bg.jpg");
    SDL_Surface* pac = IMG_Load("pacman.png");
    SDL_Surface* map_ = IMG_Load("map.png");
    SDL_Surface* inky = IMG_Load("inky_1.png");
    SDL_Rect pac_rect;
    SDL_Rect inky_rect;
    pac_rect.x = 230; pac_rect.y = 242;
    inky_rect.x = 30; inky_rect.y = 30;
    SDL_Event ev;
    bool isrunning = true;
    bool isleft =false, isright = false, isup=false, isdown=false, isout = false;
    while (isrunning)
    {
        while (SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_QUIT)
            {
                isrunning = false;
            }
            else if (ev.type == SDL_KEYDOWN)
            {
                if (ev.key.keysym.sym == SDLK_UP)
                {
                    isup = true;
                    isdown = false;
                    isleft = false;
                    isright = false;
                }
                else if (ev.key.keysym.sym == SDLK_DOWN)
                {
                    isdown = true;
                    isup = false;
                    isleft = false;
                    isright = false;
                }
                else if (ev.key.keysym.sym == SDLK_RIGHT)
                {
                    isright = true;
                    isdown = false;;
                    isup = false;
                    isleft = false;

                }
                else if (ev.key.keysym.sym == SDLK_LEFT)
                {
                    isleft = true;
                    isright = false;
                    isdown = false;;
                    isup = false;
                }
            }
            else if (ev.type == SDL_KEYUP)
            {
                if (ev.key.keysym.sym == SDLK_UP)
                {
                    isup = false;
                }
                else if (ev.key.keysym.sym == SDLK_DOWN)
                {
                    isdown = false;
                }
                else if (ev.key.keysym.sym == SDLK_RIGHT)
                {
                    isright = false;
                }
                else if (ev.key.keysym.sym == SDLK_LEFT)
                {
                    isleft = false;;
                }
            }
        }
        if (isup)
        {
            if ((ispath(pac_rect.y - 1, pac_rect.x, map_)))
            {
                pac_rect.y -= move_y;
            }
            isup = false;
        }
        if (isdown)
        {
            if ((ispath(pac_rect.y + 25, pac_rect.x, map_)))
            {
                pac_rect.y += move_y;
            }
            isdown = false;
        }
        if (isleft)
        {
            if ((ispath(pac_rect.y, pac_rect.x - 1, map_)))
            {
                pac_rect.x -= move_x;
            }
            isleft = false;
        }
        if (isright)
        {
            if ((ispath(pac_rect.y, pac_rect.x + 25, map_)))
            {
                pac_rect.x += move_x;
            }
            isright = false;
        }
        findpath(inky_rect.x, inky_rect.y, pac_rect.x, pac_rect.y, map_);
        /*if ((inky_rect.x == 210 && inky_rect.y == 242) || isout)
        {
            findpath(inky_rect.x, inky_rect.y, pac_rect.x, pac_rect.y, map_);
            isout = true;
        }
        else if (!isout)
        {
            inky_rect.y -= 1;
        }*/
        SDL_BlitSurface(bg, NULL, window_surface, NULL);
        SDL_BlitSurface(map_, NULL, window_surface, NULL);
        SDL_BlitSurface(pac, NULL, window_surface, &pac_rect);
        SDL_BlitSurface(inky, NULL, window_surface, &inky_rect);
        SDL_UpdateWindowSurface(win);
        Sleep(10);
    }
    return 0;
}

