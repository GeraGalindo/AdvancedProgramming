import pygame
import os

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

    if os.path.exists("status.txt"):
        f = open("status.txt", "r")
        if f.mode == "r":
            contents = f.read().split()  ## FILE FORMAT: Car X Y Rotation
            if contents[0] == "Blue":   # Check if Blue car has written to the status file
                print("El Azul")
                carX = int(contents[1], 10)
                carY = int(contents[2], 10)
                rotation = int(contents[3], 10)
                blueCarImage = pygame.transform.rotate(blueCarImage, rotation)
                screen.blit(backgroundImage, (0,0))
                screen.blit(blueCarImage, (carX,carY))
                print(carX, carY)
                print rotation
                os.remove("status.txt")
                continue

            if contents[0] == "Red":    # Check if Red car has written to the status file
                print("El Rojo")
            if contents[0] == "Green":   # Check if Green car has written to the status file
                print("El Verde")



    # screen.blit(backgroundImage, (0,0))
    # screen.blit(redCarImage, (redX,redY))
    # screen.blit(greenCarImage, (greenX,greenY))

    # print "#####  Coordinates  ######"
    # print "Red: ", redX, redY
    # print "Green: ", greenX, greenY
    # print "Blue: ", blueX, blueY
    # print "##########################"
    pygame.display.flip()
    frame.tick(30)
