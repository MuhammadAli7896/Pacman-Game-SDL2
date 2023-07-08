#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <windows.h>
#include <cmath>
#include <vector>
#include <ctime>
#include <random>
#include <ctime>

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

bool findpath(int& x, int& y, int x1, int y1, int& flag, SDL_Surface* map_)
{
	int dist_arr[2];
	static int count = 0;
	bool foundpath = false;
	bool isright = false, isleft = false, isup = false, isdown = false;
	if (((x <= x1 && x >= x1 - 10) && (y <= y1 && y >= y1 - 10)) || ((x <= x1 && x >= x1 + 10) && (y <= y1 && y >= y1 + 10)))
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

string pacmanani(string part)
{
	string img[3] = { "pacman.png", "pacman_" + part + "_1.png", "pacman_" + part + "_2.png" };
	static int num = 0;
	if (num == 2)
	{
		num = 0;
	}
	return img[num++];
}

void place_pile(SDL_Surface* map_, SDL_Surface* window_surface, SDL_Window* win)
{
	SDL_Surface* pille = IMG_Load("pille.png");
	SDL_Surface* super_pille = IMG_Load("superpille_3.png");
	SDL_Rect pille_rect;
	pille_rect.x = 0; pille_rect.y = 0;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{

			if (ispath(pille_rect.y, pille_rect.x, map_))
			{
				SDL_BlitSurface(pille, NULL, window_surface, &pille_rect);
				SDL_UpdateWindowSurface(win);
			}
			pille_rect.x += 5;
		}
		pille_rect.y++;
	}
}

void give_randpos(int& x, int& y, SDL_Surface* map_)
{
	int test_x;
	int test_y;
	while (1)
	{
		test_x = rand_value(0, 400);
		test_y = rand_value(0, 400);
		if (ispath(test_y + 23, test_x, map_) && ispath(test_y + 23, test_x + 23, map_) && ispath(test_y + 23, test_x + 11, map_) &&
			ispath(test_y + 23, test_x + 23, map_) && ispath(test_y, test_x + 23, map_) && ispath(test_y + 11, test_x + 23, map_) &&
			ispath(test_y, test_x, map_) && ispath(test_y + 11, test_x, map_) && ispath(test_y, test_x + 23, map_) && ispath(test_y, test_x + 11, map_)
			)
		{
			x = test_x;
			y = test_y;
			break;
		}
	}
}

SDL_Surface* haseaten(SDL_Rect* food_rect, SDL_Surface* food_surface, SDL_Rect* pac_rect, bool& caneat, int& c)
{
	if ((pac_rect->x <= food_rect->x + 25 && pac_rect->x >= food_rect->x) && (pac_rect->y <= food_rect->y + 25 && pac_rect->y >= food_rect->y))
	{
		food_surface = nullptr;
		caneat = true;
		c += 20;
	}
	return food_surface;
}

SDL_Surface* haseaten(SDL_Rect* food_rect, SDL_Surface* food_surface, SDL_Rect* pac_rect, bool& iseaten)
{
	if ((pac_rect->x <= food_rect->x + 23 && pac_rect->x >= food_rect->x) && (pac_rect->y <= food_rect->y + 23 && pac_rect->y >= food_rect->y))
	{
		food_surface = nullptr;
		iseaten = true;
	}
	return food_surface;
}

//bool iseaten(SDL_Rect *enemy_rect, SDL_Rect *pac_rect)
//{
//
//
//}

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
	SDL_Surface* pinky = IMG_Load("pinky_1.png");
	SDL_Surface* blinky = IMG_Load("blinky_1.png");
	SDL_Surface* clyde = IMG_Load("clyde_1.png");
	SDL_Surface* cherry = IMG_Load("cherry.png");
	SDL_Surface* banana = IMG_Load("banana.png");
	SDL_Surface* pear = IMG_Load("pear.png");
	SDL_Surface* orange = IMG_Load("orange.png");
	SDL_Surface* strw = IMG_Load("strawberry.png");
	SDL_Rect pac_rect;
	SDL_Rect inky_rect;
	SDL_Rect pinky_rect;
	SDL_Rect blinky_rect;
	SDL_Rect clyde_rect;
	SDL_Rect cherry_rect;
	SDL_Rect banana_rect;
	SDL_Rect pear_rect;
	SDL_Rect orange_rect;
	SDL_Rect strw_rect;
	bool inky_iseaten = false;
	bool pinky_iseaten = false;
	bool clyde_iseaten = false;
	bool blinky_iseaten = false;
	pac_rect.x = 223; pac_rect.y = 224;
	inky_rect.x = 30; inky_rect.y = 10;
	blinky_rect.x = 414; blinky_rect.y = 10;
	pinky_rect.x = 30; pinky_rect.y = 471;
	clyde_rect.x = 414; clyde_rect.y = 471;
	give_randpos(cherry_rect.x, cherry_rect.y, map_);
	give_randpos(banana_rect.x, banana_rect.y, map_);
	give_randpos(orange_rect.x, orange_rect.y, map_);
	give_randpos(strw_rect.x, strw_rect.y, map_);
	give_randpos(pear_rect.x, pear_rect.y, map_);
	double time = 0;
	clock_t begin = clock();
	SDL_Event ev;
	int flag = 0;
	int flag1 = 0;
	int flag2 = 1;
	int flag3 = 1;
	int c = 0;
	bool isrunning = true;
	bool isleft = false, isright = false, isup = false, isdown = false, isout = false, caneat = false;
	SDL_UpdateWindowSurface(win);
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
		}
		if (isup)
		{
			pac = IMG_Load(pacmanani("oben").c_str());
			if (ispath(pac_rect.y - 1, pac_rect.x, map_) && ispath(pac_rect.y - 1, pac_rect.x + 20, map_) && ispath(pac_rect.y - 1, pac_rect.x + 10, map_))
			{
				pac_rect.y -= move_y;
			}
		}
		if (isdown)
		{
			pac = IMG_Load(pacmanani("unten").c_str());
			if (ispath(pac_rect.y + 23, pac_rect.x, map_) && ispath(pac_rect.y + 23, pac_rect.x + 20, map_) && ispath(pac_rect.y + 23, pac_rect.x + 10, map_))
			{
				pac_rect.y += move_y;
			}
		}
		if (isleft)
		{
			pac = IMG_Load(pacmanani("links").c_str());
			if (ispath(pac_rect.y, pac_rect.x - 1, map_) && ispath(pac_rect.y + 20, pac_rect.x - 1, map_) && ispath(pac_rect.y + 10, pac_rect.x - 1, map_))
			{
				pac_rect.x -= move_x;
			}
		}
		if (isright)
		{
			pac = IMG_Load(pacmanani("rechts").c_str());
			if (ispath(pac_rect.y, pac_rect.x + 23, map_) && ispath(pac_rect.y + 20, pac_rect.x + 23, map_) && ispath(pac_rect.y + 10, pac_rect.x + 23, map_))
			{
				pac_rect.x += move_x;
			}
		}
		cherry = haseaten(&cherry_rect, cherry, &pac_rect, caneat, c);
		banana = haseaten(&banana_rect, banana, &pac_rect, caneat, c);
		orange = haseaten(&orange_rect, orange, &pac_rect, caneat, c);
		pear = haseaten(&pear_rect, pear, &pac_rect, caneat, c);
		strw = haseaten(&strw_rect, strw, &pac_rect, caneat, c);
		if (caneat)
		{
			time = double((clock() - begin) / CLOCKS_PER_SEC);
			if (!inky_iseaten)
				inky = IMG_Load("escaping_ghost_1.png");
			if (!clyde_iseaten)
				clyde = IMG_Load("escaping_ghost_1.png");
			if (!pinky_iseaten)
				pinky = IMG_Load("escaping_ghost_1.png");
			if (!blinky_iseaten)
				blinky = IMG_Load("escaping_ghost_1.png");
			inky = haseaten(&inky_rect, inky, &pac_rect, inky_iseaten);
			pinky = haseaten(&pinky_rect, pinky, &pac_rect, pinky_iseaten);
			blinky = haseaten(&blinky_rect, blinky, &pac_rect, blinky_iseaten);
			clyde = haseaten(&clyde_rect, clyde, &pac_rect, clyde_iseaten);
			if (inky == nullptr && pinky == nullptr && blinky == nullptr && clyde == nullptr)
				isrunning = false;
		}
		if (time >= 20)
		{
			caneat = false;
			time = 0;
			inky = IMG_Load("inky_1.png");
			pinky = IMG_Load("pinky_1.png");
			blinky = IMG_Load("blinky_1.png");
			clyde = IMG_Load("clyde_1.png");
		}
		if ((findpath(inky_rect.x, inky_rect.y, pac_rect.x, pac_rect.y, flag, map_) ||
			findpath(blinky_rect.x, blinky_rect.y, pac_rect.x, pac_rect.y, flag1, map_) ||
			findpath(pinky_rect.x, pinky_rect.y, pac_rect.x, pac_rect.y, flag2, map_) ||
			findpath(clyde_rect.x, clyde_rect.y, pac_rect.x, pac_rect.y, flag3, map_)) && (!caneat))
		{
			isrunning = false;
		}
		SDL_BlitSurface(bg, NULL, window_surface, NULL);
		SDL_BlitSurface(map_, NULL, window_surface, NULL);
		SDL_BlitSurface(pac, NULL, window_surface, &pac_rect);
		SDL_BlitSurface(inky, NULL, window_surface, &inky_rect);
		SDL_BlitSurface(pinky, NULL, window_surface, &pinky_rect);
		SDL_BlitSurface(blinky, NULL, window_surface, &blinky_rect);
		SDL_BlitSurface(clyde, NULL, window_surface, &clyde_rect);
		SDL_BlitSurface(cherry, NULL, window_surface, &cherry_rect);
		SDL_BlitSurface(banana, NULL, window_surface, &banana_rect);
		SDL_BlitSurface(pear, NULL, window_surface, &pear_rect);
		SDL_BlitSurface(orange, NULL, window_surface, &orange_rect);
		SDL_BlitSurface(strw, NULL, window_surface, &strw_rect);
		SDL_UpdateWindowSurface(win);
		Sleep(6);
	}
	SDL_Delay(3000);
	return 0;
}

