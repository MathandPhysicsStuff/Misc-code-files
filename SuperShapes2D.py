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
white = (255, 255, 255)
shift_x = width//2
shift_y = height//2
radius = 250
n1 = 1
n2 = 1
n3 = 1
m = 5
a = 1
b = 1


screen = pygame.display.set_mode((width, height))

#Functions

def return_angle(t):
 
    part1 = pow(abs((1/a) * cos((m/4)*t)), n2)
    part2 = pow(abs((1/b) * sin((m/4)*t)), n3)

    part3 = pow(part1 + part2, 1/n1)

    if part3 == 0:
        return 0

    else:
        return 1 / part3
       
def draw_SuperShape():
    
    number_of_points = 500

    points = 2*pi / number_of_points 

    for t in range(6*number_of_points):

        theta_0 = t * points
        theta_1 = (t+1) * points

        r_0 = return_angle(theta_0)
        r_1 = return_angle(theta_1)

        x1 = shift_x + (radius * r_0 * cos(theta_0))
        y1 = shift_y + (radius * r_0 * sin(theta_0))

        x2 = shift_x + (radius * r_1 * cos(theta_1))
        y2 = shift_y + (radius * r_1 * sin(theta_1))

        pygame.draw.line(screen ,blue, (x1, y1), (x2, y2), 1)


def main():

    active = True
    while active:

        screen.fill(dark_blue)

        draw_SuperShape()

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                active = False

        pygame.display.update()
        clock.tick(60)
    
    pygame.quit()
    sys.exit() 

if __name__ == "__main__":
    main()
