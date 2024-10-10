import pygame
import math

# Initialize Pygame
pygame.init()

# Set up the display
WIDTH, HEIGHT = 800, 600
win = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Ray Casting Game")

# Colors
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)
RED = (255, 0, 0)

# Player settings
player_x, player_y = 100, 100
player_angle = 0
MOVE_SPEED = 5
ROTATE_SPEED = 0.1

# Map settings
MAP_SIZE = 8
TILE_SIZE = 64
game_map = [
    [1,1,1,1,1,1,1,1],
    [1,0,0,0,0,0,0,1],
    [1,0,1,1,0,1,0,1],
    [1,0,1,0,0,1,0,1],
    [1,0,1,0,0,0,0,1],
    [1,0,1,0,1,1,0,1],
    [1,0,0,0,0,0,0,1],
    [1,1,1,1,1,1,1,1]
]

# Ray casting settings
FOV = math.pi / 3
HALF_FOV = FOV / 2
CASTED_RAYS = 120
STEP_ANGLE = FOV / CASTED_RAYS
MAX_DEPTH = 800

def cast_rays():
    start_angle = player_angle - HALF_FOV

    for ray in range(CASTED_RAYS):
        for depth in range(MAX_DEPTH):
            target_x = player_x - math.sin(start_angle) * depth
            target_y = player_y + math.cos(start_angle) * depth

            col = int(target_x / TILE_SIZE)
            row = int(target_y / TILE_SIZE)

            if game_map[row][col] == 1:
                color = 255 / (1 + depth * depth * 0.0001)
                depth *= math.cos(player_angle - start_angle)
                wall_height = 21000 / (depth + 0.0001)

                if wall_height > HEIGHT: wall_height = HEIGHT

                pygame.draw.rect(win, (color, color, color), (ray * WIDTH / CASTED_RAYS, (HEIGHT / 2) - wall_height / 2, WIDTH / CASTED_RAYS, wall_height))
                break

        start_angle += STEP_ANGLE

running = True
clock = pygame.time.Clock()

while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Movement
    keys = pygame.key.get_pressed()
    if keys[pygame.K_LEFT]: player_angle -= ROTATE_SPEED
    if keys[pygame.K_RIGHT]: player_angle += ROTATE_SPEED
    if keys[pygame.K_UP]:
        player_x += -math.sin(player_angle) * MOVE_SPEED
        player_y += math.cos(player_angle) * MOVE_SPEED
    if keys[pygame.K_DOWN]:
        player_x -= -math.sin(player_angle) * MOVE_SPEED
        player_y -= math.cos(player_angle) * MOVE_SPEED

    win.fill(BLACK)

    # Draw floor
    pygame.draw.rect(win, (100, 100, 100), (0, HEIGHT / 2, WIDTH, HEIGHT / 2))

    # Cast rays and draw walls
    cast_rays()

    pygame.display.flip()
    clock.tick(30)

pygame.quit()