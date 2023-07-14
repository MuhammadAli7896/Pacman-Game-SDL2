#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <windows.h>
#include <conio.h>
#include <cmath>
#include <vector>
#include <ctime>
#include <random>
#include <ctime>
#include <string>
#include <cstring>

#define move_y 1
#define move_x 1
using namespace std;

int rand_value(int a, int b);
Uint32 getPixelColor(SDL_Surface* surface, int x, int y);


class Character
{
protected:
	SDL_Surface* img;
	SDL_Rect rect;

public:

	Character(string img_path, int x, int y)
	{
		img = IMG_Load(img_path.c_str());
		rect.x = x;
		rect.y = y;
	}

	Character(string img_path)
	{
		img = IMG_Load(img_path.c_str());
		rect.x = 0;
		rect.y = 0;
	}

	~Character()
	{
		SDL_FreeSurface(img);
	}

	SDL_Surface* get_surface()
	{
		return img;
	}

	void set_surface(string img_path)
	{
		img = IMG_Load(img_path.c_str());
	}

	void set_surface(SDL_Surface* surface)
	{
		img = surface;
	}

	int get_y()
	{
		return rect.y;
	}

	void set_y(int y)
	{
		rect.y = y;
	}

	int get_x()
	{
		return rect.x;
	}

	void set_x(int x)
	{
		rect.x = x;
	}

	SDL_Rect* get_rect()
	{
		return &rect;
	}


	Uint32 getPixelColor(SDL_Surface* surface, int x, int y)
	{
		Uint8* pixel = (Uint8*)surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel;
		Uint8 r, g, b;
		SDL_GetRGB(*(Uint32*)pixel, surface->format, &r, &g, &b);
		return SDL_MapRGB(surface->format, r, g, b);
	}

	bool ispath(int x, int y, SDL_Surface* map_)
	{
		// checking that the path is not a wall 
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
	virtual string animate(string part = "") = 0;
};


class Pacman :public Character
{
	bool caneat, isleft, isright, isup, isdown;

public:

	Pacman(string img_path) :Character(img_path)
	{
		caneat = false;
		isleft = false;
		isright = false;
		isup = false;
		isdown = false;
	}

	bool get_caneat()
	{
		return caneat;
	}

	void set_caneat(bool val)
	{
		caneat = val;
	}

	bool get_isleft()
	{
		return isleft;
	}

	void set_isleft(bool val)
	{
		isleft = val;
	}

	bool get_isright()
	{
		return isright;
	}

	void set_isright(bool val)
	{
		isright = val;
	}

	bool get_isup()
	{
		return isup;
	}

	void set_isup(bool val)
	{
		isup = val;
	}

	bool get_isdown()
	{
		return isdown;
	}

	void set_isdown(bool val)
	{
		isdown = val;
	}

	void teleport()
	{
		// function for teleportation of pacman from left to right and vice-versa
		if (rect.x >= 420 && (rect.y >= 210 && rect.y <= 245))
		{
			rect.x = 19;
			rect.y = 220;
		}
		else if (rect.x <= 18 && (rect.y >= 210 && rect.y <= 245))
		{
			rect.x = 419;
			rect.y = 220;
		}
	}

	string animate(string part = "")
	{
		// animation for pacman 
		string img[3] = { "pacman.png", "pacman_" + part + "_1.png", "pacman_" + part + "_2.png" };
		static int num = 0;
		if (num == 2)
		{
			num = 0;
		}
		return img[num++];
	}

	SDL_Surface* haseaten(SDL_Rect* food_rect, SDL_Surface* food_surface, int& c)
	{
		// function for eating food
		if ((rect.x <= food_rect->x + 25 && rect.x >= food_rect->x) && (rect.y <= food_rect->y + 25 && rect.y >= food_rect->y))
		{
			food_surface = nullptr;
			caneat = true;
			c += 20;
		}
		return food_surface;
	}

	void move_right()
	{
		rect.x += move_x;
	}

	void move_left()
	{
		rect.x -= move_x;
	}

	void move_up()
	{
		rect.y -= move_y;
	}

	void move_down()
	{
		rect.y += move_y;
	}

};


class Enemy :public Character
{
	string name;
	int num;

public:

	Enemy(string img_path, string n) :Character(img_path)
	{
		num = 0;
		name = n;
	}

	string animate(string part = "")
	{
		// function to animate for enemy class
		string img_arr[4] = { name + "_1.png", name + "_2.png", name + "_3.png", name + "_4.png" };
		if (num == 3)
		{
			num = 0;
		}
		return img_arr[num++];
	}

	SDL_Surface* haseaten(SDL_Rect* pac_rect, bool& iseaten, int& score)
	{
		// function for pacman eating enemy
		if (((pac_rect->x >= rect.x - 5 && pac_rect->x <= rect.x) && (pac_rect->y >= rect.y - 5 && pac_rect->y <= rect.y + 23)) ||
			((pac_rect->x >= rect.x && pac_rect->x <= rect.x + 5) && (pac_rect->y >= rect.y - 5 && pac_rect->y <= rect.y + 23)) ||
			((pac_rect->y >= rect.y && pac_rect->y <= rect.y + 5) && (pac_rect->x >= rect.x - 5 && pac_rect->x <= rect.x + 23)) ||
			((pac_rect->y >= rect.y - 5 && pac_rect->y <= rect.y) && (pac_rect->x >= rect.x - 5 && pac_rect->x <= rect.x + 23)))
		{
			img = nullptr;
			iseaten = true;
		}
		return img;
	}

	bool findpath(int x1, int y1, int& flag, SDL_Surface* map_, bool iseaten)
	{
		// function for enemy movement and enemy eating pacman
		static int count = 0;
		bool foundpath = false;
		bool isright = false, isleft = false, isup = false, isdown = false;
		if ((((rect.x >= x1 - 5 && rect.x <= x1) && (rect.y >= y1 - 5 && rect.y <= y1 + 23)) ||
			((rect.x >= x1 && rect.x <= x1 + 5) && (rect.y >= y1 - 5 && rect.y <= y1 + 23) ||
				((rect.y >= y1 && rect.y <= y1 + 5) && (rect.x >= x1 - 5 && rect.x <= x1 + 23)) ||
				((rect.y >= y1 - 5 && rect.y <= y1) && (rect.x >= x1 - 5 && rect.x <= x1 + 23)))
			&& (!iseaten)))
		{
			return true;
		}
		else
		{
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
				if (ispath(rect.y, rect.x + 23, map_) && ispath(rect.y + 20, rect.x + 23, map_) && ispath(rect.y + 11, rect.x + 23, map_) && (rect.x + 23 >= 0 && rect.x + 23 <= 446))
				{
					rect.x++;
					return false;
				}
				else
				{
					isright = false;
					flag = rand_value(2, 3);
					if (flag == 2)
					{
						if (!ispath(rect.y - 4, rect.x, map_) && !ispath(rect.y - 4, rect.x + 23, map_) && !ispath(rect.y - 4, rect.x + 11, map_) && !(rect.y - 4 >= 0 && rect.y - 4 <= 494))
						{
							flag = 3;
							return false;
						}
					}
					else
					{
						if (!ispath(rect.y + 23, rect.x, map_) && !ispath(rect.y + 23, rect.x + 23, map_) && !ispath(rect.y + 23, rect.x + 11, map_) && !(rect.y + 23 >= 0 && rect.y + 23 <= 494))
						{

							flag = 2;
							return false;
						}

					}
				}
			}
			else if (isleft)
			{
				if (ispath(rect.y, rect.x - 1, map_) && ispath(rect.y + 20, rect.x - 1, map_) && ispath(rect.y + 11, rect.x - 1, map_) && (rect.x - 1 >= 0 && rect.x - 1 <= 446))
				{
					rect.x--;
					return false;
				}
				else
				{
					isleft = false;
					flag = rand_value(2, 3);
					if (flag == 2)
					{
						if (!ispath(rect.y - 4, rect.x, map_) && !ispath(rect.y - 4, rect.x + 23, map_) && !ispath(rect.y - 4, rect.x + 11, map_) && !(rect.y - 4 >= 0 && rect.y - 4 <= 494))
						{
							flag = 3;
							return false;
						}
					}
					else
					{
						if (!ispath(rect.y + 23, rect.x, map_) && !ispath(rect.y + 23, rect.x + 20, map_) && !ispath(rect.y + 23, rect.x + 11, map_) && !(rect.y + 23 >= 0 && rect.y + 23 <= 494))
						{
							flag = 2;
							return false;
						}

					}
				}
			}
			else if (isup)
			{
				if (ispath(rect.y - 1, rect.x, map_) && ispath(rect.y - 1, rect.x + 20, map_) && ispath(rect.y - 1, rect.x + 11, map_) && (rect.y - 1 >= 0 && rect.y - 1 <= 494))
				{
					rect.y--;
					return false;
				}
				else
				{
					isup = false;
					flag = rand_value(0, 1);
					if (flag == 1)
					{
						if (!ispath(rect.y, rect.x - 4, map_) && !ispath(rect.y + 23, rect.x - 4, map_) && !ispath(rect.y + 11, rect.x - 4, map_) && !(rect.x - 4 >= 0 && rect.x - 4 <= 446))
						{
							flag = 0;
							return false;
						}
					}
					else
					{
						if (!ispath(rect.y, rect.x + 23, map_) && !ispath(rect.y + 23, rect.x + 23, map_) && !ispath(rect.y + 11, rect.x + 23, map_) && !(rect.x + 23 >= 0 && rect.x + 23 <= 446))
						{
							flag = 1;
							return false;
						}
					}
				}
			}
			else
			{
				if (ispath(rect.y + 23, rect.x, map_) && ispath(rect.y + 23, rect.x + 20, map_) && ispath(rect.y + 23, rect.x + 9, map_) && (rect.y + 23 >= 0 && rect.y + 23 <= 494))
				{
					rect.y++;
					return false;
				}
				else
				{
					isdown = false;
					flag = rand_value(0, 1);
					if (flag == 1)
					{
						if (!ispath(rect.y, rect.x - 5, map_) && !ispath(rect.y + 23, rect.x - 5, map_) && !ispath(rect.y + 11, rect.x - 5, map_) && !(rect.x - 5 >= 0 && rect.x - 5 <= 446))
						{
							flag = 0;
							return false;
						}
					}
					else
					{
						if (!ispath(rect.y, rect.x + 23, map_) && !ispath(rect.y + 20, rect.x + 23, map_) && !ispath(rect.y + 11, rect.x + 23, map_) && !(rect.x + 23 >= 0 && rect.x + 23 <= 446))
						{
							flag = 1;
							return false;
						}
					}
				}
			}
			count++;
			return false;
		}
	}
};

class Game
{
	SDL_Window* win;
	SDL_Surface* window_surface;
	SDL_Surface* bg;
	SDL_Surface* map_;
	SDL_Event ev;
	Pacman* pac;
	Enemy* enemies[4];
	SDL_Surface* cherry;
	SDL_Surface* banana;
	SDL_Surface* pear;
	SDL_Surface* orange;
	SDL_Surface* strw;
	SDL_Rect cherry_rect;
	SDL_Rect banana_rect;
	SDL_Rect pear_rect;
	SDL_Rect orange_rect;
	SDL_Rect strw_rect;
	int enemy_flag[4];
	int score;
	bool isrunning;
	bool enemy_iseaten[4];

public:

	Game()
	{
		bg = IMG_Load("black_bg.jpg");
		map_ = IMG_Load("map.png");
		pac = new Pacman("pacman.png");
		enemies[0] = new Enemy("inky_1.png", "inky");
		enemies[1] = new Enemy("pinky_1.png", "pinky");
		enemies[2] = new Enemy("blinky_1.png", "blinky");
		enemies[3] = new Enemy("clyde_1.png", "clyde");
		pac->set_x(223); pac->set_y(224);
		enemies[0]->set_x(30); enemies[0]->set_y(10);
		enemies[1]->set_x(414); enemies[1]->set_y(10);
		enemies[2]->set_x(30); enemies[2]->set_y(471);
		enemies[3]->set_x(414); enemies[3]->set_y(471);
		enemy_iseaten[0] = enemy_iseaten[1] = enemy_iseaten[2] = enemy_iseaten[3] = false;
		cherry = IMG_Load("cherry.png");
		banana = IMG_Load("banana.png");
		pear = IMG_Load("pear.png");
		orange = IMG_Load("orange.png");
		strw = IMG_Load("strawberry.png");
		enemy_flag[0] = 0;
		enemy_flag[1] = 0;
		enemy_flag[2] = 1;
		enemy_flag[3] = 1;
		cherry_rect.x = 90; cherry_rect.y = 12;
		strw_rect.x = 75; strw_rect.y = 460;
		banana_rect.x = 360; banana_rect.y = 460;
		pear_rect.x = 364; pear_rect.y = 12;
		orange_rect.x = 210; orange_rect.y = 78;
		score = 0;
		isrunning = true;
	}

	~Game()
	{
		delete pac;
		for (int i = 0; i < 4; i++)
		{
			delete enemies[i];
		}
		delete enemies;

		SDL_FreeSurface(cherry);
		SDL_FreeSurface(strw);
		SDL_FreeSurface(banana);
		SDL_FreeSurface(orange);
		SDL_FreeSurface(pear);
		SDL_FreeSurface(bg);
		SDL_FreeSurface(map_);
		SDL_DestroyWindow(win);
		SDL_FreeSurface(window_surface);
		IMG_Quit();
		SDL_Quit();
	}

	bool ispath(int x, int y, SDL_Surface* map_)
	{
		// checking that the path is not a wall
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


	void game()
	{
		win = SDL_CreateWindow("Pac Man", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 446, 600, 0);
		window_surface = SDL_GetWindowSurface(win);
		double time = 0;
		clock_t begin = clock();
		int c = 0, count = 0;
		main_menu();
		while (isrunning)
		{
			while (SDL_PollEvent(&ev))
			{
				if (ev.type == SDL_QUIT)
				{
					isrunning = false;
					exit(0);
				}
				else if (ev.type == SDL_KEYDOWN)
				{
					if (ev.key.keysym.sym == SDLK_UP)
					{
						pac->set_isup(true);
						pac->set_isdown(false);
						pac->set_isleft(false);
						pac->set_isright(false);
					}
					else if (ev.key.keysym.sym == SDLK_DOWN)
					{
						pac->set_isup(false);
						pac->set_isdown(true);
						pac->set_isleft(false);
						pac->set_isright(false);
					}
					else if (ev.key.keysym.sym == SDLK_RIGHT)
					{
						pac->set_isup(false);
						pac->set_isdown(false);
						pac->set_isleft(false);
						pac->set_isright(true);

					}
					else if (ev.key.keysym.sym == SDLK_LEFT)
					{
						pac->set_isup(false);
						pac->set_isdown(false);
						pac->set_isleft(true);
						pac->set_isright(false);
					}
				}
			}
			if (pac->get_isup())
			{
				pac->set_surface(pac->animate("oben").c_str());
				if (pac->ispath(pac->get_y() - 1, pac->get_x(), map_) && pac->ispath(pac->get_y() - 1, pac->get_x() + 18, map_) && pac->ispath(pac->get_y() - 1, pac->get_x() + 10, map_))
				{
					pac->move_up();
				}
			}
			if (pac->get_isdown())
			{
				pac->set_surface(pac->animate("unten").c_str());
				if (ispath(pac->get_y() + 23, pac->get_x(), map_) && ispath(pac->get_y() + 23, pac->get_x() + 18, map_) && ispath(pac->get_y() + 23, pac->get_x() + 10, map_))
				{
					pac->move_down();
				}
			}
			if (pac->get_isleft())
			{
				pac->set_surface(pac->animate("links").c_str());
				if (ispath(pac->get_y(), pac->get_x() - 1, map_) && ispath(pac->get_y() + 18, pac->get_x() - 1, map_) && ispath(pac->get_y() + 10, pac->get_x() - 1, map_))
				{
					pac->move_left();
				}
			}
			if (pac->get_isright())
			{
				pac->set_surface(pac->animate("rechts").c_str());
				if (ispath(pac->get_y(), pac->get_x() + 23, map_) && ispath(pac->get_y() + 18, pac->get_x() + 23, map_) && ispath(pac->get_y() + 10, pac->get_x() + 23, map_))
				{
					pac->move_right();
				}
			}
			cherry = foodeaten(&cherry_rect, cherry, pac->get_rect(), pac->get_caneat(), c);
			banana = foodeaten(&banana_rect, banana, pac->get_rect(), pac->get_caneat(), c);
			orange = foodeaten(&orange_rect, orange, pac->get_rect(), pac->get_caneat(), c);
			pear = foodeaten(&pear_rect, pear, pac->get_rect(), pac->get_caneat(), c);
			strw = foodeaten(&strw_rect, strw, pac->get_rect(), pac->get_caneat(), c);
			if (pac->get_caneat())
			{
				time = double((clock() - begin) / CLOCKS_PER_SEC);
				if (!enemy_iseaten[0])
					enemies[0]->set_surface("escaping_ghost_1.png");
				if (!enemy_iseaten[1])
					enemies[1]->set_surface("escaping_ghost_1.png");
				if (!enemy_iseaten[2])
					enemies[2]->set_surface("escaping_ghost_1.png");
				if (!enemy_iseaten[3])
					enemies[3]->set_surface("escaping_ghost_1.png");
				enemies[0]->set_surface(enemies[0]->haseaten(pac->get_rect(), enemy_iseaten[0], score));
				enemies[1]->set_surface(enemies[1]->haseaten(pac->get_rect(), enemy_iseaten[1], score));
				enemies[2]->set_surface(enemies[2]->haseaten(pac->get_rect(), enemy_iseaten[2], score));
				enemies[3]->set_surface(enemies[3]->haseaten(pac->get_rect(), enemy_iseaten[3], score));
			}
			if (time >= c)
			{
				pac->set_caneat(false);
				time = 0;
				begin = clock();
				if (!enemy_iseaten[0])
					enemies[0]->set_surface("inky_1.png");
				if (!enemy_iseaten[1])
					enemies[1]->set_surface("pinky_1.png");
				if (!enemy_iseaten[2])
					enemies[2]->set_surface("blinky_1.png");
				if (!enemy_iseaten[3])
					enemies[3]->set_surface("clyde_1.png");
				pac->set_caneat(false);
			}
			if ((enemies[0]->findpath(pac->get_x(), pac->get_y(), enemy_flag[0], map_, enemy_iseaten[0]) ||
				enemies[1]->findpath(pac->get_x(), pac->get_y(), enemy_flag[1], map_, enemy_iseaten[1]) ||
				enemies[2]->findpath(pac->get_x(), pac->get_y(), enemy_flag[2], map_, enemy_iseaten[2]) ||
				enemies[3]->findpath(pac->get_x(), pac->get_y(), enemy_flag[3], map_, enemy_iseaten[3])) &&
				(!pac->get_caneat()))
			{
				SDL_Surface* gameover = IMG_Load("gameover.jpg");
				SDL_Rect g_rect;
				SDL_BlitSurface(gameover, NULL, window_surface, NULL);
				SDL_UpdateWindowSurface(win);
				isrunning = false;
				break;
			}
			if ((enemy_iseaten[0]) && (enemy_iseaten[1]) && (enemy_iseaten[2]) && (enemy_iseaten[3]))
			{
				SDL_Surface* gameover = IMG_Load("win2.jpg");
				SDL_Rect g_rect;
				g_rect.y = 120;
				g_rect.x = 5;
				SDL_BlitSurface(bg, NULL, window_surface, NULL);
				SDL_BlitSurface(gameover, NULL, window_surface, &g_rect);
				SDL_UpdateWindowSurface(win);
				isrunning = false;
				break;
			}
			SDL_BlitSurface(bg, NULL, window_surface, NULL);
			SDL_BlitSurface(map_, NULL, window_surface, NULL);
			SDL_BlitSurface(pac->get_surface(), NULL, window_surface, pac->get_rect());
			if (!enemy_iseaten[0])
				SDL_BlitSurface(enemies[0]->get_surface(), NULL, window_surface, enemies[0]->get_rect());
			if (!enemy_iseaten[1])
				SDL_BlitSurface(enemies[1]->get_surface(), NULL, window_surface, enemies[1]->get_rect());
			if (!enemy_iseaten[2])
				SDL_BlitSurface(enemies[2]->get_surface(), NULL, window_surface, enemies[2]->get_rect());
			if (!enemy_iseaten[3])
				SDL_BlitSurface(enemies[3]->get_surface(), NULL, window_surface, enemies[3]->get_rect());
			pac->teleport();
			if (count % 100 == 0)
			{
				enemies[0]->set_surface(enemies[0]->animate());
				enemies[1]->set_surface(enemies[1]->animate());
				enemies[2]->set_surface(enemies[2]->animate());
				enemies[3]->set_surface(enemies[3]->animate());
			}
			SDL_BlitSurface(cherry, NULL, window_surface, &cherry_rect);
			SDL_BlitSurface(banana, NULL, window_surface, &banana_rect);
			SDL_BlitSurface(pear, NULL, window_surface, &pear_rect);
			SDL_BlitSurface(orange, NULL, window_surface, &orange_rect);
			SDL_BlitSurface(strw, NULL, window_surface, &strw_rect);
			SDL_UpdateWindowSurface(win);
			count++;
			Sleep(6);
		}
		//SDL_Delay(3000);

		//game(); // restarting
	}

	int main_menu()
	{
		// starting image loading
		SDL_Surface* main = IMG_Load("main.jpg");
		SDL_Rect rect;
		SDL_BlitSurface(main, NULL, window_surface, NULL);
		SDL_UpdateWindowSurface(win);
		while (SDL_PollEvent(&ev) != 0 || SDL_PollEvent(&ev) == 0)
		{
			if (ev.type == SDL_KEYDOWN)
			{
				if (ev.key.keysym.sym == SDLK_KP_ENTER)
				{
					return 1;
				}
			}
		}
		return 1;
	}

	SDL_Surface* foodeaten(SDL_Rect* food_rect, SDL_Surface* food_surface, SDL_Rect* pac_rect, bool caneat, int& c)
	{
		// function for eating food
		if ((((pac_rect->x >= food_rect->x - 10 && pac_rect->x <= food_rect->x) && (pac_rect->y >= food_rect->y - 10 && pac_rect->y <= food_rect->y + 28)) ||
			((pac_rect->x >= food_rect->x && pac_rect->x <= food_rect->x + 10) && (pac_rect->y >= food_rect->y - 10 && pac_rect->y <= food_rect->y + 28)) ||
			((pac_rect->y >= food_rect->y && pac_rect->y <= food_rect->y + 10) && (pac_rect->x >= food_rect->x - 10 && pac_rect->x <= food_rect->x + 28)) ||
			((pac_rect->y >= food_rect->y - 10 && pac_rect->y <= food_rect->y) && (pac_rect->x >= food_rect->x - 10 && pac_rect->x <= food_rect->x + 28)))
			&& (food_surface != nullptr))
		{
			food_surface = nullptr;
			pac->set_caneat(true);
			c += 10;
		}
		return food_surface;
	}

};


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




bool ispath(int x, int y, SDL_Surface* map_)
{
	// to check that the path is not a wall 
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


bool findpath(int& x, int& y, int x1, int y1, int& flag, SDL_Surface* map_, SDL_Surface* enemy)
{
	// function for movement of enemy
	static int count = 0;
	bool foundpath = false;
	bool isright = false, isleft = false, isup = false, isdown = false;
	if (((x <= x1 && x >= x1 - 10) && (y <= y1 && y >= y1 - 10)) || ((x <= x1 && x >= x1 + 10) && (y <= y1 && y >= y1 + 10)) && (enemy != nullptr))
	{
		return true;
	}
	else
	{
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
			if (ispath(y, x + 23, map_) && ispath(y + 20, x + 23, map_) && ispath(y + 11, x + 23, map_) && x + 23 >= 0 && x + 23 <= 446)
			{
				x++;
				return false;
			}
			else
			{
				isright = false;
				flag = rand_value(2, 3);
				if (flag == 2)
				{
					if (!ispath(y - 4, x, map_) && !ispath(y - 4, x + 23, map_) && !ispath(y - 4, x + 11, map_) && !(y - 4 >= 0 && y - 4 <= 494))
					{
						flag = 3;
						return false;
					}
				}
				else
				{
					if (!ispath(y + 23, x, map_) && !ispath(y + 23, x + 23, map_) && !ispath(y + 23, x + 11, map_) && !(y + 23 >= 0 && y + 23 <= 494))
					{

						flag = 2;
						return false;
					}

				}
			}
		}
		else if (isleft)
		{
			if (ispath(y, x - 1, map_) && ispath(y + 20, x - 1, map_) && ispath(y + 11, x - 1, map_) && x - 1 >= 0 && x - 1 <= 446)
			{
				x--;
				return false;
			}
			else
			{
				isleft = false;
				flag = rand_value(2, 3);
				if (flag == 2)
				{
					if (!ispath(y - 4, x, map_) && !ispath(y - 4, x + 23, map_) && !ispath(y - 4, x + 11, map_) && !(y - 4 >= 0 && y - 4 <= 494))
					{
						flag = 3;
						return false;
					}
				}
				else
				{
					if (!ispath(y + 23, x, map_) && !ispath(y + 23, x + 20, map_) && !ispath(y + 23, x + 11, map_) && !(y + 23 >= 0 && y + 23 <= 494))
					{
						flag = 2;
						return false;
					}

				}
			}
		}
		else if (isup)
		{
			if (ispath(y - 1, x, map_) && ispath(y - 1, x + 20, map_) && ispath(y - 1, x + 11, map_) && y - 1 >= 0 && y - 1 <= 494)
			{
				y--;
				return false;
			}
			else
			{
				isup = false;
				flag = rand_value(0, 1);
				if (flag == 1)
				{
					if (!ispath(y, x - 4, map_) && !ispath(y + 23, x - 4, map_) && !ispath(y + 11, x - 4, map_) && !(x - 4 >= 0 && x - 4 <= 446))
					{
						flag = 0;
						return false;
					}
				}
				else
				{
					if (!ispath(y, x + 23, map_) && !ispath(y + 23, x + 23, map_) && !ispath(y + 11, x + 23, map_) && !(x + 23 >= 0 && x + 23 <= 446))
					{
						flag = 1;
						return false;
					}
				}
			}
		}
		else
		{
			if (ispath(y + 23, x, map_) && ispath(y + 23, x + 20, map_) && ispath(y + 23, x + 9, map_) && y + 23 >= 0 && y + 23 <= 494)
			{
				y++;
				return false;
			}
			else
			{
				isdown = false;
				flag = rand_value(0, 1);
				if (flag == 1)
				{
					if (!ispath(y, x - 5, map_) && !ispath(y + 23, x - 5, map_) && !ispath(y + 11, x - 5, map_) && !(x - 5 >= 0 && x - 5 <= 446))
					{
						flag = 0;
						return false;
					}
				}
				else
				{
					if (!ispath(y, x + 23, map_) && !ispath(y + 20, x + 23, map_) && !ispath(y + 11, x + 23, map_) && !(x + 23 >= 0 && x + 23 <= 446))
					{
						flag = 1;
						return false;
					}
				}
			}
		}
		count++;
		Sleep(10);
		return false;
	}
}



string pacmananimate(string part)
{
	// function for animation of pacman
	string img[3] = { "pacman.png", "pacman_" + part + "_1.png", "pacman_" + part + "_2.png" };
	static int num = 0;
	if (num == 2)
	{
		num = 0;
	}
	return img[num++];
}


int main(int argc, char** argv)
{
	SDL_Event ev;
	while (true) {
		Game* PacMan = new Game;
		PacMan->game();
		SDL_PollEvent(&ev);
		SDL_Delay(4000);
		if (ev.type == SDL_QUIT)
		{
			break;
		}
		// restarting 
	}

	return 0;
}