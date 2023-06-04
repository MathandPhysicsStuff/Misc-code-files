import pygame
import sys
from math import sin, cos, pi

pygame.init()
clock = pygame.time.Clock()

#Variables and constance

width = 600
height = 600
dark_blue = (20, 20, 40)
blue = (150, 150, 255)
shift_x = width//2
shift_y = height//2
a = 250
b = 250
n = 0.1

screen = pygame.display.set_mode((width, height))

#Functions

def sgn(tf):
    
    if tf < 0:
        return -1
 
    if tf == 0:
        return 0
 
    if tf > 0:
        return 1




def draw_super_ellipse(n):
    
    t = 0 

    while t < 2*pi:

        x1 = shift_x + (abs(cos(t))**(2/n)) * a * sgn(cos(t))
        y1 = shift_y + (abs(sin(t))**(2/n)) * b * sgn(sin(t))

        t += 0.01

        x2 = shift_x + (abs(cos(t))**(2/n)) * a * sgn(cos(t))
        y2 = shift_x + (abs(sin(t))**(2/n)) * a * sgn(sin(t))

        #pygame.draw.circle(screen, blue, (x1, y1), 1)
        pygame.draw.line(screen, blue, (x1,y1), (x2, y2), 1)


def main(n):

    active = True
    while active:

        n += 0.01

        screen.fill(dark_blue)

        draw_super_ellipse(n)

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                active = False

        pygame.display.update()
        clock.tick(60)
    
    pygame.quit()
    sys.exit() 

if __name__ == "__main__":
    main(n)
