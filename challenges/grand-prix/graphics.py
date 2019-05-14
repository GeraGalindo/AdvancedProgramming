import pygame
import os

f = open("initFile.txt", "r")
init_values = f.read()

redCarImage = None
greenCarImage = None
blueCarImage = None

def initRed():
    global redCarImage
    redCarImage = pygame.image.load("res/red_car.png")
    redCarImage = pygame.transform.scale(redCarImage, (30,45))
    redCarImage = redCarImage.convert_alpha()

def initGreen():
    global greenCarImage
    greenCarImage = pygame.image.load("res/green_car.png")
    greenCarImage = pygame.transform.scale(greenCarImage, (30,45))
    greenCarImage = greenCarImage.convert_alpha()

def initBlue():
    global blueCarImage
    blueCarImage = pygame.image.load("res/blue_car.png")
    blueCarImage = pygame.transform.scale(blueCarImage, (30,45))
    blueCarImage = blueCarImage.convert_alpha()

pygame.init() # PYGAME Initilization
screen = pygame.display.set_mode((900,700))

####### BEGIN CARS DEFINITION ######
initBlue()
if int(init_values[0]) == 2 or int(init_values[0]) == 3:
    initGreen()
if int(init_values[0]) == 3:
    initRed()
####### END CARS DEFINITION ########

backgroundImage = pygame.image.load("res/background.png")
backgroundImage = pygame.transform.scale(backgroundImage, (900,700))
screen.blit(backgroundImage, (0,0))

frame = pygame.time.Clock()

blueX = 60
blueY = 325
greenX = 100
greenY = 325
redX = 140
redY = 325
blueRotation = 0
redRotation = 0
greenRotation = 0

running = True

while running == True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    if os.path.exists("blue.txt"):
        f = open("blue.txt", "r")
        if f.mode == "r":
            contents = f.read().split()  ## FILE FORMAT: Car X Y Rotation
            if contents[0] == "Blue":   # Check if Blue car has written to the status file
                blueX = int(contents[1], 10)
                blueY = int(contents[2], 10)
                blueRotation = int(contents[3], 10)
                os.remove("blue.txt")

    if os.path.exists("red.txt") and int(init_values[0]) == 3:
        f = open("red.txt", "r")
        if f.mode == "r":
            contents = f.read().split()  ## FILE FORMAT: Car X Y Rotation
            if contents[0] == "Red":   # Check if Blue car has written to the status file
                redX = int(contents[1], 10)
                redY = int(contents[2], 10)
                redRotation = int(contents[3], 10)
                os.remove("red.txt")

    if os.path.exists("green.txt") and (int(init_values[0]) == 2 or int(init_values[0]) == 3):
        f = open("green.txt", "r")
        if f.mode == "r":
            contents = f.read().split()  ## FILE FORMAT: Car X Y Rotation
            if contents[0] == "Green":   # Check if Blue car has written to the status file
                greenX = int(contents[1], 10)
                greenY = int(contents[2], 10)
                greenRotation = int(contents[3], 10)
                os.remove("green.txt")

    screen.blit(backgroundImage, (0,0))
    blueCarImage = pygame.transform.rotate(blueCarImage, blueRotation)

    screen.blit(blueCarImage, (blueX, blueY))
    
    if int(init_values[0]) == 3:
        redCarImage = pygame.transform.rotate(redCarImage, redRotation)

        screen.blit(redCarImage, (redX, redY))
    
    if int(init_values[0]) == 2 or int(init_values[0]) == 3:
        greenCarImage = pygame.transform.rotate(greenCarImage, greenRotation)

        screen.blit(greenCarImage, (greenX, greenY))

    pygame.display.flip()
    frame.tick(20)
