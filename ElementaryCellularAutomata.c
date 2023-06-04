#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <SDL2/SDL.h>

void generate_rules(int rules[256][8]);
void first_cell_array(int* cell_array, int cell_array_len);
void cell_automata(int* cell_array, int* next_cell_array, int* rule, int cell_array_len);
void draw_cell_array(int* cell_array, int cell_size, int cell_array_len, int rule_number);
void draw_cells(int cell_size, int cell_array_len, int rule_number, int y);
void generate_random_seed(int* cell_array, int cell_array_len);

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Color gray = {100, 100, 100};

const int width = 1024;
const int height = 540;
const int X_center = width / 2;
const int extra_cells = 200;


int main()
{   
    int y = 0;    
    int draw = 0;
    int i = 0; 
    int cell_size = 1;
    int cell_array_len = (width / cell_size) + extra_cells;
    int cell_array[cell_array_len];

    generate_random_seed(cell_array, cell_array_len);

    int good_rules[] = {
                       0, 18, 22, 26, 30, 45, 57, 60, 73, 75,
                       82, 86, 89, 90, 99, 101, 102, 110, 124,
                       126, 129, 135, 137, 146, 149, 150, 153, 
                       154, 161, 165, 167, 169, 181, 182, 193,
                       195, 210, 218, 225, 0
                       };

    int rule_number = 0; 

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Video Initialization Error: ", SDL_GetError());
    }
    
    else
    {
        printf("Hello World!\n");
    }

    window = SDL_CreateWindow("Elementary Cellular Automata", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // | SDL_RENDERER_PRESENTVSYNC );

    int running = 1;
    while (running == 1)
    {
        

        SDL_Event event;

        while (SDL_PollEvent(&event))
         {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_RETURN)
                {
                    draw = 1;
                }
            }
         }
        
         SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
         SDL_RenderClear(renderer);
         //usleep(10000); 
         //if(y < (height / cell_size)) {y ++; }
         //else {y = 0;}
         //draw_cells(cell_size, cell_array_len, rule_number, y);
         //draw_text(rule_number);
         
         
         if (draw == 1) 
         {
            /*
             if(i < 39) { i ++; }
             else { i = 39; }
             usleep(1000000); 
             rule_number = good_rules[i];
             printf("%i\n", rule_number);
            */

             draw_cell_array(cell_array, cell_size, cell_array_len, rule_number);
 
             if (rule_number < 255) {rule_number ++;}
             else {rule_number = 255;}
             usleep(1000000); 
         }
         
         

         if (rule_number < 255) {rule_number ++;}
         else {rule_number = 255;}

         SDL_RenderPresent(renderer);
         SDL_Delay(100);

    }
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void first_cell_array(int* cell_array, int cell_array_len)
{
    int i;
    for(i = 0; i < cell_array_len; i++)
    {
        if(i == (cell_array_len / 2) - 1) { cell_array[i] = 1; }
        else { cell_array[i] = 0; } 
    }
}

void generate_random_seed(int* cell_array, int cell_array_len)
{
    srand(time(0));
    int random_number;

    for (int i = 0; i < cell_array_len; i++)
    {
        random_number = (rand() % 100) + 1;

        if (random_number <= 51) { cell_array[i] = 1; }
        else { cell_array[i] = 0; } 
        //printf("%d", cell_array[i]);
        
    }

}

void generate_rule(int* rule, int rule_number)
{
    int i;
    int rn = rule_number;
    
    for(i = 0; i < 8; i++)
    {
        if(rn % 2 == 1) { rule[7-i] = 1; }
        else if(rn % 2 == 0) { rule[7-i] = 0; }

        rn = rn / 2;
    }
}

void cell_automata(int* cell_array, int* next_cell_array, int* rule, int cell_array_len)
{   
    int i, j, x, y, z;
    int temp_var;
    
    //Rule
    for(i = 0; i < cell_array_len; i ++)
    {
        x = cell_array[(i - 1) % cell_array_len];     
        y = cell_array[i % cell_array_len];     
        z = cell_array[(i + 1) % cell_array_len];

        //Checking the cells 
        if((x == 1) && (y == 1) && (z == 1)) { next_cell_array[i] = rule[0]; }
        else if((x == 1) && (y == 1) && (z == 0)) { next_cell_array[i] = rule[1]; }
        else if((x == 1) && (y == 0) && (z == 1)) { next_cell_array[i] = rule[2]; }
        else if((x == 1) && (y == 0) && (z == 0)) { next_cell_array[i] = rule[3]; }
        else if((x == 0) && (y == 1) && (z == 1)) { next_cell_array[i] = rule[4]; }
        else if((x == 0) && (y == 1) && (z == 0)) { next_cell_array[i] = rule[5]; }
        else if((x == 0) && (y == 0) && (z == 1)) { next_cell_array[i] = rule[6]; }
        else if((x == 0) && (y == 0) && (z == 0)) { next_cell_array[i] = rule[7]; }
        else { next_cell_array[i] = 0; }
    }

    //Swapping cell_array and next_cell_array values
    for(j = 0; j < cell_array_len; j ++)
    {

        temp_var = cell_array[j];
        cell_array[j] = next_cell_array[j];
        next_cell_array[j] = temp_var;
    }
    
}

void draw_cell_array(int* cell_array, int cell_size, int cell_array_len, int rule_number)
{
    int i, j; 

    //Array logic variables
    int rule[8]; 
    //int cell_array[cell_array_len];
    int next_cell_array[cell_array_len];

    //Drawing array variables
    SDL_Rect cell_rect;
    cell_rect.x;
    cell_rect.y = 0;
    cell_rect.w = cell_size;
    cell_rect.h = cell_size;

    //Generating rule array
    generate_rule(rule, rule_number);
    
    //Seeding first array
    //first_cell_array(cell_array, cell_array_len);
    
    //Drawing to the window 
    
    //SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

    int fps = 60;
    int desiredDelta = 700/fps; 
   
    for(i = 1; i <= (height / cell_size); i++)
    {
        int startLoop = SDL_GetTicks();

        for(j = 0; j < cell_array_len; j++)
        {
            cell_rect.x = (cell_size * j) - extra_cells/2;
            if(cell_array[j] == 1)
            {
                SDL_RenderFillRect(renderer, &cell_rect);
            }
        }
        cell_rect.y = i * cell_size;
        cell_automata(cell_array, next_cell_array, rule, cell_array_len);
        /* 
         SDL_RenderPresent(renderer); 
        
         int delta = SDL_GetTicks() - startLoop;
         if (delta < desiredDelta)
         {
            SDL_Delay(desiredDelta - delta);
         }
         */
    }
}

void draw_cells(int cell_size, int cell_array_len, int rule_number, int y)
{
    int i; 

    //Array logic variables
    int rule[8]; 
    int cell_array[cell_array_len];
    int next_cell_array[cell_array_len];

    //Drawing array variables
    SDL_Rect cell_rect;
    cell_rect.x;
    cell_rect.y = y;
    cell_rect.w = cell_size;
    cell_rect.h = cell_size;

    //Generating rule array
    generate_rule(rule, rule_number);
    
    //Seeding first array
    first_cell_array(cell_array, cell_array_len);
    
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    for(i = 0; i < cell_array_len; i++)
    {
        cell_rect.x = (cell_size * i) - extra_cells/2;
        if (cell_array[i] == 1)
        {
            SDL_RenderFillRect(renderer, &cell_rect);
        }
    }
    cell_automata(cell_array, next_cell_array, rule, cell_array_len);
}































