#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

double points[8][3] = {
                   {-0.5, -0.5, 0.5}, 
                   {0.5, -0.5, 0.5},
                   {0.5, 0.5, 0.5},
                   {-0.5, 0.5, 0.5},
                   {-0.5, -0.5, -0.5}, 
                   {0.5, -0.5, -0.5},
                   {0.5, 0.5, -0.5},
                   {-0.5, 0.5, -0.5} };

void rotateZ(double v[8][3])
{
    double x = 0.031415926;
    double Zm[3][3] = { {cos(x), -sin(x), 0},
                        {sin(x), cos(x), 0},
                        {0, 0, 1} };

    for(int i = 0; i < 8; i++)
    {
        //printf("%lf %lf %lf\n", v[0][0], v[0][1], v[0][2]);
        for(int j = 0; j < 3; j++)
        {
            v[i][j] = v[i][0]*Zm[j][0] + v[i][1]*Zm[j][1] + v[i][2]*Zm[j][2];
        }
    }
}

void rotateY(double v[8][3])
{
    double x = 0.031415926;
    double Ym[3][3] = { {cos(x), 0, -sin(x)},
                        {0,      1, 0      },
                        {sin(x), 0, cos(x)}};


    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            v[i][j] = v[i][0]*Ym[j][0] + v[i][1]*Ym[j][1] + v[i][2]*Ym[j][2];
        }
    }
}

void rotateX(double v[8][3])
{
    double x = 0.031415926;
    double Xm[3][3] = { {1, 0     ,  0},
                        {0, cos(x), -sin(x)},
                        {0, sin(x), cos(x)}};


    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            v[i][j] = v[i][0]*Xm[j][0] + v[i][1]*Xm[j][1] + v[i][2]*Xm[j][2];
        }
    }
}

void project(double v[8][3])
{

    for(int i = 0; i < 8; i++)
    {
        double d = 1.1;
        double z = 1 / (d - points[i][2]);
        double P[3][3] = { {z, 0, 0},
                           {0, z, 0},
                           {0, 0, 0} };

        for(int j = 0; j < 3; j++)
        {
            v[i][j] = v[i][0]*P[j][0] + v[i][1]*P[j][1] + v[i][2]*P[j][2];
        }
    }
}

void link(double points[8][3])
{
    int S = 200;
    int X = 320;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for(int i = 0; i < 4; i++)
    {
        SDL_RenderDrawLine(renderer, X + S*(points[i][0]),
                                     X + S*(points[i][1]),
                                     X + S*(points[(i+1) % 4][0]),
                                     X + S*(points[(i+1) % 4][1]));

        SDL_RenderDrawLine(renderer, X + S*(points[i+4][0]),
                                     X + S*(points[i+4][1]),
                                     X + S*(points[4 + ((i+1) % 4)][0]),
                                     X + S*(points[4 + ((i+1) % 4)][1]));

        SDL_RenderDrawLine(renderer, X + S*(points[i][0]),
                                     X + S*(points[i][1]),
                                     X + S*(points[4 + (i % 4)][0]),
                                     X + S*(points[4 + (i % 4)][1]));
    }
}

void draw_cube(double points[8][3])
{
    SDL_Rect point;
    point.w = 2;
    point.h = 2;
    for(int i = 0; i < 8; i++)
    {
        point.x = 320 + 200*(points[i][0]);
        point.y = 320 + 200*(points[i][1]);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &point);
    }
}

int main()
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Video Initialization Error: ", SDL_GetError());
    }
    else{ printf("System is Ready to Go!\n"); }

    window = SDL_CreateWindow("Cube",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int running = 1;
    while(running == 1)
    {
        SDL_Event event;
        
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                running = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        rotateZ(points);
        rotateY(points);
        rotateX(points);
        project(points);
        link(points);
        draw_cube(points);
        
        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}









