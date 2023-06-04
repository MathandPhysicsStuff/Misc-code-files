#include <stdio.h>
#include <SDL2/SDL.h>
#include <immintrin.h>

double map(double number, double lower_bound, double upper_bound, double new_lower_bound, double new_upper_bound); 
__m256d _map(__m256d _number, double ub, double lb, double n_ub, double n_lb);
int hsb_to_rgb(int* array, double hue, double saturation, double brightness);
void draw_mandelbrot_set(int iterations, double a_lower_bound, double a_upper_bound,
                                         double b_lower_bound, double b_upper_bound);
void draw_mandelbrot_set_SIMD(int iterations, double a_lower_bound, double a_upper_bound,
                                         double b_lower_bound, double b_upper_bound);

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;
const int WIDTH_SHIFT = SCREEN_WIDTH / 2;
const int HEIGHT_SHIFT = SCREEN_HEIGHT / 2;

int main()
{   

    SDL_Rect rect;
    rect.x = WIDTH_SHIFT-1;
    rect.y = HEIGHT_SHIFT-1;
    rect.w = 2;
    rect.h = 2;
    int iterations = 1024;
    double center_x = 0;
    double center_y = 0;
    double distance_from_center_x = 2;
    double distance_from_center_y = 2;

    double a_lower_bound;
    double a_upper_bound;
    double b_lower_bound;
    double b_upper_bound;
    
    double step = 0.05;
    double zoom_in = 0.95;
    double zoom_out = 1.05;
    int zoomed = 0;


    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Video Initialization Error: ", SDL_GetError());
    }

    else
    {
        printf("System is ready to go!\n");
    }

    window = SDL_CreateWindow("Mandelbrot Set",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    renderer = SDL_CreateRenderer(window,
                                  -1,
                                  SDL_RENDERER_ACCELERATED);

    int running = 1;
    while (running == 1)
    {
       
        SDL_Event event;

        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                running = 0;
            }
            
            //const Uint8* state = SDL_GetKeyboardState(NULL);
            //if(state[SDL_SCANCODE_RIGHT]) {center_x += step; }

            if(event.type == SDL_KEYDOWN)
            {
                 
                if(event.key.keysym.sym == SDLK_RIGHT)
                {
                    center_x += step; 
                }
                

                if(event.key.keysym.sym == SDLK_LEFT)
                {
                    center_x -= step;    
                }

                if(event.key.keysym.sym == SDLK_UP)
                {
                    center_y -= step;    
                }

                if(event.key.keysym.sym == SDLK_DOWN)
                {
                    center_y += step;    
                }

                if(event.key.keysym.sym == SDLK_i)
                {
                    distance_from_center_x *= zoom_in;
                    distance_from_center_y *= zoom_in;
                    step *= zoom_in;
                    zoomed += 1;
                    printf("zoomed %d\n", zoomed);
                }

                if(event.key.keysym.sym == SDLK_o)
                {
                    distance_from_center_x *= zoom_out;
                    distance_from_center_y *= zoom_out;
                    step *= zoom_out;
                }

                if(event.key.keysym.sym == SDLK_z)
                {
                   iterations += 8;
                   printf("%d\n", iterations);
                }

                if(event.key.keysym.sym == SDLK_x)
                {
                    if(iterations > 8)
                    {
                        iterations -= 8; 
                    }
                    else
                    {
                        iterations = 0;  
                    }
                }

            }
        }

        a_lower_bound = center_x - distance_from_center_x;
        a_upper_bound = center_x + distance_from_center_x;
        b_lower_bound = center_y - distance_from_center_y;
        b_upper_bound = center_y + distance_from_center_y;
 

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        //draw_mandelbrot_set(iterations, a_lower_bound, a_upper_bound, b_lower_bound, b_upper_bound);
        draw_mandelbrot_set_SIMD(iterations, a_lower_bound, a_upper_bound, b_lower_bound, b_upper_bound);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &rect); 

        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
}

double map(double number, double lower_bound, double upper_bound, double new_lower_bound, double new_upper_bound) 
{
    double percent = number / (upper_bound - lower_bound);
    double new_range = new_upper_bound - new_lower_bound;
    double scaled_number = new_range * percent;

    return new_lower_bound + scaled_number;
}

int hsb_to_rgb(int* array, double hue, double saturation, double brightness)
{
    double red, green, blue;
    double hue_percent = (int)hue % 60 / 60.0;
    double b = brightness * 255;
     
    if(0 <= hue && hue < 60)
    {
        red = 1;
        blue = 1 - saturation;
        green = blue + hue_percent*(red - blue);
    }
    else if(60 <= hue && hue < 120)
    {
        green = 1;
        blue = 1 - saturation;
        red = green - hue_percent*(green - blue);
    }
    else if(120 <= hue && hue < 180)
    {
        green = 1;
        red = 1 - saturation;
        blue = red + hue_percent*(green - red);
    }
    else if(180 <= hue && hue < 240)
    {
        blue = 1;
        red = 1 - saturation;
        green = blue - hue_percent*(blue - red);
    }
    else if(240 <= hue && hue < 300)
    {
        blue = 1;
        green = 1 - saturation;
        red = green + hue_percent*(blue - green);
    }
    else if(300 <= hue && hue < 360)
    {
        red = 1;
        green = 1 - saturation;
        blue = red - hue_percent*(red - green);
    }
    array[0] = red*b;
    array[1] = green*b;
    array[2] = blue*b;
}

void draw_mandelbrot_set(int iterations, double a_lower_bound, double a_upper_bound,
                                         double b_lower_bound, double b_upper_bound)

{
    int x, y;
    double a, b;
    double a_abs, b_abs;
    double next_a, next_b;
    double real_number, imaginary_number;
    int color[3];
    
    for(y = 0; y < SCREEN_WIDTH; y++)
    {
        for(x = 0; x < SCREEN_HEIGHT; x++)
        {
            
            a = map(x, 0, SCREEN_WIDTH, a_lower_bound, a_upper_bound);
            b = map(y, 0, SCREEN_HEIGHT, b_lower_bound, b_upper_bound);

            real_number = a;
            imaginary_number = b;
            
            for(int i = 0; i < iterations; i++)
            {

                next_a = a*a - b*b;
                next_b = 2*a*b;

                a = next_a + real_number;
                b = next_b + imaginary_number;
                
                if(a*a + b*b > 4) 
                {
                    double hue = i%359;
                    double brightness = 1;
                    hsb_to_rgb(color, hue, 1, brightness);
  
                    break;
                }
                else
                {
                    color[0] = 0; 
                    color[1] = 0; 
                    color[2] = 0; 
                }
            }
            SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }

    }
}
__m256d _map(__m256d _number, double ub, double lb, double n_ub, double n_lb)
{
 
    __m256d _ub, _lb, _n_ub, _n_lb, _p, _range, _n_number;
 
    _ub = _mm256_set1_pd(ub);
    _lb = _mm256_set1_pd(lb);
    _n_ub = _mm256_set1_pd(n_ub);
    _n_lb = _mm256_set1_pd(n_lb);
 
    _p = _mm256_div_pd(_number, _mm256_sub_pd(_ub, _lb));
    _range = _mm256_sub_pd(_n_ub, _n_lb);
    _n_number = _mm256_mul_pd(_range, _p);

    return _mm256_add_pd(_n_lb, _n_number);

}


void draw_mandelbrot_set_SIMD(int iterations, double a_lower_bound, double a_upper_bound,
                                         double b_lower_bound, double b_upper_bound)

{
    int x, y, i, j, k;
    int color[3];
    double hue[4];
    double *escape_time;
    __m256d _x, _y, _a, _b, _real, _imaginary, _next_a, _next_b, _escape_time;
    __m256d _two = _mm256_set1_pd(2.0);
    
    for (y = 0; y < SCREEN_HEIGHT; y++)
    {
        _y = _mm256_set1_pd(y);
        for (x = 0; x < SCREEN_WIDTH; x += 4)
        {
            _x = _mm256_set_pd(x+3, x+2, x+1, x);

            _a = _map(_x, SCREEN_WIDTH, 0, a_upper_bound, a_lower_bound);
            _b = _map(_y, SCREEN_HEIGHT, 0, b_upper_bound, b_lower_bound);

            _real = _a;
            _imaginary = _b;

            int lock[4] = {0, 0, 0, 0};
            double brightness[4] = {1, 1, 1, 1};

            for (i = 0; i < iterations; i++)
            {
                _next_a = _mm256_sub_pd(_mm256_mul_pd(_a, _a), _mm256_mul_pd(_b, _b));
                _next_b = _mm256_mul_pd(_two, _mm256_mul_pd(_a, _b));
                
                _a = _mm256_add_pd(_next_a, _real);
                _b = _mm256_add_pd(_next_b, _imaginary);

                _escape_time = _mm256_add_pd(_mm256_mul_pd(_a, _a), _mm256_mul_pd(_b, _b));
                
                escape_time = (double*)&_escape_time;

                for(k = 0; k < 4; k++)
                {
                    if (escape_time[k] > 4 && lock[k] == 0)
                    {
                        hue[k] = i%359;
                        lock[k] = 1;
                    }
                    else if (escape_time[k] < 4 && i >= iterations-1) 
                    {
                        brightness[k] = 0;
                    }
                }

                if (lock[0] == 1 && lock[1] == 1 && lock[2] == 1 && lock[3] == 1) { break; }
            }

            for (j = 0; j < 4; j++)
            {
               hsb_to_rgb(color, hue[j], 1, brightness[j]);
               SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], 255);
               SDL_RenderDrawPoint(renderer, x+j, y);
            }
        }
    }
}






