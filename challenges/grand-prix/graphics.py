import pygame

pygame.init() # PYGAME Initilization
screen = pygame.display.set_mode((900,700))

redCarImage = pygame.image.load("res/red_car.png")
redCarImage = pygame.transform.scale(redCarImage, (30,45))
redCarImage = redCarImage.convert_alpha()

backgroundImage = pygame.image.load("res/background.png")
backgroundImage = pygame.transform.scale(backgroundImage, (900,700))
screen.blit(backgroundImage, (0,0))

frame = pygame.time.Clock()

x = 50
y = 650

finished = False

while finished == False:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            finished = True

    pressedKey = pygame.key.get_pressed()

    if pressedKey[pygame.K_SPACE] == 1:
        y -= 5

    screen.blit(backgroundImage, (0,0))
    screen.blit(redCarImage, (x,y))
    pygame.display.flip()
    frame.tick(30)
