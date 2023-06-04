import pygame
import sys
from random import uniform
from math import ceil, sin, cos, tan
from collections import deque
import time

pygame.init()
clock = pygame.time.Clock()

draw_font = pygame.font.SysFont("serif", 16)

WIDTH = 800
HEIGHT = 640

X_SCREEN_CENTER = WIDTH // 2
Y_SCREEN_CENTER = HEIGHT // 2

BLACK = (0, 0, 0)
GRAY = (200, 200, 200)
WHITE = (255, 255, 255)
blue = (0, 0, 255)

screen = pygame.display.set_mode((WIDTH, HEIGHT))

def display_text():
    
    display_rect = pygame.Rect(0, 0, 160, 60) 
    text = draw_font.render("constant b = 0.08", 1, GRAY)

    screen.set_clip(display_rect)
    screen.fill(BLACK)

    screen.blit(text, (10, 10))

    screen.set_clip(None)
    
def create_main_array(trail_length, number_of_points):

    array_of_points = deque([])

    for i in range(number_of_points):

        trail_array = deque([])

        x = uniform(-0.01, 0.01)
        y = uniform(-0.01, 0.01)
        z = uniform(-0.01, 0.01)
        point = (x, y, z)

        for j in range(trail_length):

            trail_array.append(point)

        array_of_points.append(trail_array)

    return array_of_points


def draw_points(arrays, array_length, start_color):

    scaling_factor = 2
    
    largest_color = max(start_color)

    red_grad = ceil(start_color[0] / array_length)
    green_grad = ceil(start_color[1] / array_length)
    blue_grad = ceil(start_color[2] / array_length)

    grad_step = ceil(array_length / largest_color)

    for array in arrays:
        

        red = 0
        green = 0
        blue = 0

        for position in range(array_length):
            

            x0 = array[position - 1][0]
            y0 = array[position - 1][1]
            point0 = (X_SCREEN_CENTER + x0*scaling_factor, Y_SCREEN_CENTER + y0*scaling_factor)

            x1 = array[position][0]
            y1 = array[position][1]
            point1 = (X_SCREEN_CENTER + x1*scaling_factor, Y_SCREEN_CENTER + y1*scaling_factor)


            color = (red, green, blue)
            #print(color)
            
            #Trail
            if position > 0:
                pygame.draw.aaline(screen, color, point0, point1, 1)
                pygame.draw.circle(screen, color, point2, 1)
            
            x2 = array[-1][0]
            y2 = array[-1][1]
            point2 = (X_SCREEN_CENTER + x2*scaling_factor, Y_SCREEN_CENTER + y2*scaling_factor)
            
            #Head
            #pygame.draw.circle(screen, WHITE, point2, 1)
          
            if position % grad_step == 0:
                if red < start_color[0] and red + red_grad < 256:
                    red += red_grad

                if green < start_color[1] and green + green_grad < 256:
                    green += green_grad

                if blue < start_color[2] and blue + blue_grad < 256:
                    blue += blue_grad


def update_attractor(arrays, dt):
    
    array_length = len(arrays)
    a = 32.48
    b = 45.84
    c = 1.18
    d = 0.13
    e = 0.57 
    f = 14.7

    dx = 0
    
    for i in range(array_length):
        
        array = arrays[i]
        
        x = array[-1][0]
        y = array[-1][1]
        z = array[-1][2]
        
        dx = (a*(y - x) + dx*z)*dt
        dy = (b*x - x*z + f*y)*dt
        dz = (c*z + x*y - e*x**2)*dt

        x = x + dx
        y = y + dy
        z = z + dz

        point = (x, y, z)
        
        array.append(point) 
        array.popleft()
    
    return arrays
        



def main():
    
    step = 0.02
    trail_length = 50
    number_of_points = 500

    array_of_points = create_main_array(trail_length, number_of_points)
    
    previus_time = time.time()
    
    draw = False
    running = True 
    while running:

        delta_time = time.time() - previus_time
        previus_time = time.time()
        dt = delta_time*step

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

            if  event.type == pygame.KEYDOWN:
                if event.key == pygame.K_RETURN:
                    draw = True

        screen.fill(BLACK)
        
        if draw == True:
            
            #display_text()
            draw_points(array_of_points, trail_length, (255, 64, 8))
            new_array_of_points = update_attractor(array_of_points, dt)
            array_of_points = new_array_of_points

        pygame.display.update()
        #clock.tick(60)

    pygame.quit()
    sys.exit()


main()























