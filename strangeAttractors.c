#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "SDL2/SDL.h"

//gcc strangeAttractors.c -O2 -lm -lSDL2 -lSDL2main

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;
const int centerX = SCREEN_WIDTH / 2;
const int centerY = SCREEN_HEIGHT / 2;

typedef struct P
{
    double x, y, z;
} P;

void create_attractor(struct P* p, int number_of_points, int tail_length)
{
    int i, j;
    double tx, ty, tz;
    int array_length = number_of_points * tail_length;

    for (i = 0; i < array_length; i += tail_length)
    {

        tx = 0.002*((double)rand() / (double)RAND_MAX) - 0.001;
        ty = 0.002*((double)rand() / (double)RAND_MAX) - 0.001;
        tz = 0.002*((double)rand() / (double)RAND_MAX) - 0.001;

        for (j = 0; j < tail_length; j++)
        {
            p[i + j].x = tx;
            p[i + j].y = ty;
            p[i + j].z = tz;
        }
    }
}

int find_chaotic_attractor(double *a, int a_len)
{

    double x, y, x2, y2, x3, y3, tempx, tempy, tempx2, tempy2;
    double dx, dy, dx2, dy2, dx3, dy3;
    double Dx, Dy, D0, D;
    double dt = 0.001;
    double lyapunov;
    double l, g, w; 

    int found = 0;
    int bound;
    int counter = 0;
    double min = 0.000001;
    double max = 100;
    
    for(int i = 0; i < 10000; i++)
    {
        for(int j = 0; j < a_len; j++)
        {
            a[j] = 4*((double)rand() / (double)RAND_MAX) - 2;
        }
        
        bound = 0;

        x = 0.002*((double)rand() / (double)RAND_MAX) - 0.001;
        y = 0.002*((double)rand() / (double)RAND_MAX) - 0.001;

        x3 = 0.002*((double)rand() / (double)RAND_MAX) - 0.001;
        y3 = 0.002*((double)rand() / (double)RAND_MAX) - 0.001;

        x2 = x + 0.0001*((double)rand() / (double)RAND_MAX);
        y2 = y + 0.0001*((double)rand() / (double)RAND_MAX);

        Dx = x2 - x;
        Dy = y2 - y;

        D0 = sqrt(Dx*Dx + Dy*Dy); 
        
        counter += 1;
        lyapunov = 0;

        for(int k = 0; k < 100000; k++)
        {
            tempx = x;
            tempy = y;

            dx = a[0] + a[1]*x + a[2]*x*x + a[3]*x*y + a[4]*y + a[5]*y*y; 
            dy = a[6] + a[7]*x + a[8]*x*x + a[9]*x*y + a[10]*y + a[11]*y*y;

            dx3 = a[0] + a[1]*x3 + a[2]*x3*x3 + a[3]*x3*y3 + a[4]*y3 + a[5]*y3*y3; 
            dy3 = a[6] + a[7]*x3 + a[8]*x3*x3 + a[9]*x3*y3 + a[10]*y3 + a[11]*y3*y3;

            x = dx; 
            y = dy;

            x3 = dx3; 
            y3 = dy3;

            if (fabs(x - tempx) < min && fabs(y - tempy) < min)
            {
                bound = 1;
                break; 
            }
             
            if (fabs(x3 - x) < min && fabs(y3 - y) < min)
            {
                bound = 11;
                break;
            }

            if (fabs(x) > max || fabs(y) > max)
            {
                bound = 2;
                break;
            }

            if (i > 1000)
            {
     
                dx2 = a[0] + a[1]*x2 + a[2]*x2*x2 + a[3]*x2*y2 + a[4]*y2 + a[5]*y2*y2; 
                dy2 = a[6] + a[7]*x2 + a[8]*x2*x2 + a[9]*x2*y2 + a[10]*y2 + a[11]*y2*y2;
                
                x2 = dx2; 
                y2 = dy2; 

                Dx = x - x2;
                Dy = y - y2;

                D = sqrt(Dx*Dx + Dy*Dy); 

                lyapunov += log(fabs(D/D0));

                x2 = x + (D0*Dx)/D;
                y2 = y + (D0*Dy)/D;
               
            }
        }

        if (bound == 0 && lyapunov > 0)
        {
            found = 1;
            //printf("%lf\n", lyapunov);
            printf("%d\n", counter);
            return found;
        }
       
    }
    printf("%d\n", bound);
    return found;
}

void polynomial_attractor(struct P* p, int number_of_points, int tail_length, double* a)
{
    int i, j, k;
    double dx, dy, dz;
    double dt = 0.000001;
    int array_length = number_of_points * tail_length;

    for (i = tail_length-1; i < array_length; i += tail_length)
    {
        dx = a[0] + a[1]*p[i].x + a[2]*p[i].x*p[i].x + a[3]*p[i].x*p[i].y + a[4]*p[i].y + a[5]*p[i].y*p[i].y; 
        dy = a[6] + a[7]*p[i].x + a[8]*p[i].x*p[i].x + a[9]*p[i].x*p[i].y + a[10]*p[i].y + a[11]*p[i].y*p[i].y; 

        p[i].x =  dx;
        p[i].y = dy;

        k = i+1 - tail_length; 
        for (j = 0; j < tail_length-1; j++)
        {
            p[k + j].x = p[k + j + 1].x;
            p[k + j].y = p[k + j + 1].y;
            p[k + j].z = p[k + j + 1].z;
        }
    }
}

void lorenz_attractor(struct P* p, int number_of_points, int tail_length, double a, double b, double c)
{
    int i, j, k;
    double dx, dy, dz;
    double dt = 0.005;
    int array_length = number_of_points * tail_length;

    for (i = tail_length-1; i < array_length; i += tail_length)
    {
        dx = (a * (p[i].y - p[i].x)) * dt;
        dy = (p[i].x * (b - p[i].z) - p[i].y) * dt;
        dz = (p[i].x*p[i].y - c*p[i].z) * dt;

        p[i].x = p[i].x + dx;
        p[i].y = p[i].y + dy;
        p[i].z = p[i].z + dz;

        k = i+1 - tail_length; 
        for (j = 0; j < tail_length-1; j++)
        {
            p[k + j].x = p[k + j + 1].x;
            p[k + j].y = p[k + j + 1].y;
            p[k + j].z = p[k + j + 1].z;
        }
    }
}
 
void render_attractor(struct P* p, int number_of_points, int tail_length, double scale, double centerX, double centerY)
{
    int i, j;
    int array_length = number_of_points * tail_length; 
    int red = 64, green = 128, blue = 255;
    int ru = red / ((tail_length % (red-1)) + 1) + 1;
    int gu = green / ((tail_length % (green-1)) + 1) + 1;
    int bu = blue / ((tail_length % (blue-1)) + 1) + 1;

    double x0, y0, x1, y1;
     
    for (i = 0; i < array_length; i += tail_length)
    {
        int r = 64, g = 128, b = 255;
        for (j = 0; j < tail_length-1; j++)
        {
            x0 = centerX + p[i + j].x * scale;    
            y0 = centerY + p[i + j].y * scale;

            x1 = centerX + p[i + j + 1].x * scale;    
            y1 = centerY + p[i + j + 1].y * scale;

            if (r - ru >= 0) r = r - ru;
            if (g - gu >= 0) g = g - gu;
            if (b - bu >= 0) b = b - bu;


            //SDL_SetRenderDrawColor(renderer, red - r, green - g, blue - b, 255);
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderDrawLine(renderer, x0, y0, x1, y1);
        }
    }
}


void mat_mul_v(struct P* p, struct P* rp,int array_length, double m[3][3])
{
    double a, b, c;
    
    for (int i = 0; i < array_length; i++)
    {
        a = p[i].x*m[0][0] + p[i].y*m[0][1] + p[i].z*m[0][2];
        b = p[i].x*m[1][0] + p[i].y*m[1][1] + p[i].z*m[1][2];
        c = p[i].x*m[2][0] + p[i].y*m[2][1] + p[i].z*m[2][2];
        rp[i].x = a;
        rp[i].y = b;
        rp[i].z = c;
     }
}



int main()
{

    if (SDL_Init(SDL_INIT_VIDEO) < 0) printf("SDL2 video failed to initialize: \n", SDL_GetError());
    else printf("System is ready to go!\n");

    window = SDL_CreateWindow("Hello SDL",
                               SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               SCREEN_WIDTH, SCREEN_HEIGHT,
                               SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window,
                                   -1,
                                   SDL_RENDERER_ACCELERATED);
    
    srand(time(NULL));
    int number_of_points = 10000; 
    int tail_length = 2;
    int array_length = number_of_points * tail_length;
    struct P points[array_length];
    struct P Rpoints[array_length];

    double a = 10;
    double b = 28;
    double c = 8.0/3.0;

    double scale = 100;
    double rx = 0, ry = 0, rz = 0;

    double e[12];
    int found;

    found = find_chaotic_attractor(e, 12);
    //printf("%d\n", found); 
    if (found == 0) return 1;


    create_attractor(points, number_of_points, tail_length);
    

    int count = 0;
    SDL_bool running = SDL_TRUE;
    while (running == SDL_TRUE)
    {

        double rotateX[3][3] = { 
                                 {1         , 0         , 0},
                                 {0, cos(rx), -sin(rx)},
                                 {0, sin(rx), cos(rx)}
                               };
 
        double rotateY[3][3] = { 
                                 {cos(ry), 0, -sin(ry)},
                                 {0         , 1, 0          },
                                 { sin(ry), 0, cos(ry)}
                               };
 
        double rotateZ[3][3] = { 
                                 {cos(rz), -sin(rz), 0},
                                 {sin(rz), cos(rz), 0},
                                 {0         ,0          , 1} 
                               };
 

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = SDL_FALSE;
                break;
            }

            if (event.type == SDL_KEYDOWN)
            {
                if(event.key.keysym.sym == SDLK_UP)
                {
                    rx += 0.05;
                }

                if(event.key.keysym.sym == SDLK_DOWN)
                {
                    rx -= 0.05;
                }

                if(event.key.keysym.sym == SDLK_RIGHT)
                {
                    ry += 0.05;
                }

                if(event.key.keysym.sym == SDLK_LEFT)
                {
                    ry -= 0.05;
                }

                if(event.key.keysym.sym == SDLK_i)
                {
                    scale += 1;
                }

                if(event.key.keysym.sym == SDLK_o)
                {
                    if (scale > 1) scale -= 1;
                }

            }
        }
 

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        //lorenz_attractor(points, number_of_points, tail_length, a, b, c);
        
        polynomial_attractor(points, number_of_points, tail_length, e);

        mat_mul_v(points, Rpoints, array_length, rotateX);
        mat_mul_v(Rpoints, Rpoints, array_length, rotateY);

        render_attractor(Rpoints, number_of_points, tail_length, scale, centerX, centerY);

        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}



























