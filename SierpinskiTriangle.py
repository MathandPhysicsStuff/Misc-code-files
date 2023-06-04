import pygame 
import sys
import time


pygame.init()
clock = pygame.time.Clock()


"""Constance"""
WIDTH = 600
HEIGHT = 600
GRAY = (30, 30, 30)
LIGHT_BLUE = (75, 75, 150)

screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Sierpinski Triangle")

points = [(50, HEIGHT - 50), #A
          (WIDTH/2, 50), #B
          (WIDTH - 50, HEIGHT - 50)] #C

limit = 9


def sierpinski(points, limit):

    if limit <= 0:

        pygame.draw.line(screen, LIGHT_BLUE, points[0], points[1], 1)
        pygame.draw.line(screen, LIGHT_BLUE, points[1], points[2], 1)
        pygame.draw.line(screen, LIGHT_BLUE, points[0], points[2], 1)

    else:

        a = points[0]
        b = points[1]
        c = points[2]

        ab = ((a[0] + b[0]) / 2, (a[1] + b[1]) / 2) 
        bc = ((b[0] + c[0]) / 2, (b[1] + c[1]) / 2) 
        ac = ((a[0] + c[0]) / 2, (a[1] + c[1]) / 2)

        sierpinski([a, ab, ac], limit - 1)
        sierpinski([ab, b, bc], limit - 1)
        sierpinski([ac, bc, c], limit - 1)


def main(limit):
    
    temp = 0

    active = True
    while active:
        
        time.sleep(0.5)

        
        if limit <= 0:
            temp = 1
            limit += temp
            
        elif 0 < limit < 9:
            temp = temp
            limit += temp

        else:
            limit >= 9
            temp = -1
            limit += temp

        screen.fill(GRAY)

        sierpinski(points, limit)

        pygame.display.update()
        clock.tick(60)

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                active = False


    pygame.quit()
    sys.exit()


if __name__ == "__main__":
    main(limit)




































