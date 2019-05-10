import pygame

pygame.init() # PYGAME Initilization
screen = pygame.display.set_mode((900,700))

####### BEGIN CARS DEFINITION ######
redCarImage = pygame.image.load("res/red_car.png")
redCarImage = pygame.transform.scale(redCarImage, (30,45))
redCarImage = redCarImage.convert_alpha()

greenCarImage = pygame.image.load("res/green_car.png")
greenCarImage = pygame.transform.scale(greenCarImage, (30,45))
greenCarImage = greenCarImage.convert_alpha()

blueCarImage = pygame.image.load("res/blue_car.png")
blueCarImage = pygame.transform.scale(blueCarImage, (30,45))
blueCarImage = blueCarImage.convert_alpha()
####### END CARS DEFINITION ########

backgroundImage = pygame.image.load("res/background.png")
backgroundImage = pygame.transform.scale(backgroundImage, (900,700))
screen.blit(backgroundImage, (0,0))

frame = pygame.time.Clock()

redX = 60
redY = 325
greenX = 100
greenY = 325
blueX = 140
blueY = 325

running = True

while running == True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    pressedKey = pygame.key.get_pressed()

    if pressedKey[pygame.K_SPACE] == 1:
        redY -= 2
        greenY -= 2
        blueY -= 2

    screen.blit(backgroundImage, (0,0))
    screen.blit(redCarImage, (redX,redY))
    screen.blit(greenCarImage, (greenX,greenY))
    screen.blit(blueCarImage, (blueX,blueY))
    print "Red: ", redX, redY
    print "Green: ", greenX, greenY
    print "Blue: ", blueX, blueY
    pygame.display.flip()
    frame.tick(30)
