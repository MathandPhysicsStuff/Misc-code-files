import pygame
import sys
from math import sin, cos, pi, pow, sqrt
import random

pygame.init()
clock = pygame.time.Clock()

width = 600
height = 600
black = (0, 0, 0)
white = (255, 255, 255)

screen = pygame.display.set_mode((width, height))

shift_x = width // 2
shift_y = height 
theta = pi / 2
length = 150
x = shift_x
y = height

def draw_tree(x, y, length, theta):

    x2 = x - length * cos(theta) 
    y2 = y - length * sin(theta)

    pygame.draw.line(screen, white, (x, y), (x2, y2), 1) 

    #theta += 0.5
   
    if length > 1:

        draw_tree(x2, y2, length * (0.69), theta + (pi/7))
        draw_tree(x2, y2, length * (0.69), theta - (pi/4))


def main():

    active = True
    while active:

        screen.fill(black)

        draw_tree(x, y, length, theta)

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                active = False


        pygame.display.update()
        clock.tick(60)

    pygame.quit()
    sys.exit()

if __name__ == "__main__":
    main()




