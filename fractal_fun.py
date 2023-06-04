import pygame
import sys
from math import fabs

pygame.init()
clock = pygame.time.Clock()

#Variables and constance
width = 600
height = 600
#Colors 
black = (0, 0, 0)
light_blue = (100, 100, 225)

screen = pygame.display.set_mode((width, height))
pygame.display.set_caption("Fun with fractals")

#Math variables
x = width // 2
y = height // 2
r = 100


def draw_circles(x, y, r):

    pygame.draw.circle(screen, light_blue, (x, y), r, 1)

    if r > 1:
        

        """
        draw_circles(x + r, y, r//2)
        draw_circles(x - r, y, r//2)
        draw_circles(x , y - r, r//2)
        draw_circles(x , y - r, r//2)
        """

        draw_circles(x, y-r, r // 2)
        draw_circles(x, y+r, r // 2)
        draw_circles(x+r, y+r, r // 2)
        draw_circles(x-r, y+r, r // 2)

a = [width//2 - 50, height - 1]
b = [width//2 + 50, height - 1]
c = [(a[0] + b[0])/2, height - (b[0] - a[0])]

def triangles(a, b, c):
        
    pygame.draw.polygon(screen, light_blue, [a, b, c], 1) 
    
    if (b[0] - a[0]) > 1:
        a2 = [width/4 + a[0]/2 + 100, height/2 + a[1]/2]
        b2 = [width/4 + b[0]/2 + 100, height/2 + b[1]/2]
        c2 = [width/4 + c[0]/2 + 100, height/2 + c[1]/2]
        triangles(a2, b2, c2)

def main():
    
    active = True
    while active:
        
        screen.fill(black)

        draw_circles(x, y, r)

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                active = False

        pygame.display.update()
        clock.tick(60)

    pygame.quit()
    sys.exit()

if __name__ == "__main__":
    main()
