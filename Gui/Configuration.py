import pygame
import sys
import json

if len(sys.argv) == 1:
  print("Please give a file path to the config")
  exit(1)

with open(sys.argv[1], 'r') as content_file:
  content = content_file.read()

config = json.loads(content)

if "Size" not in config:
  print("Please define a size")
  exit()
if "x" not in config["Size"]:
  print("Please define an x for Size")
  exit()
if "y" not in config["Size"]:
  print("Please define an y for Size")  
  exit()

if "HomeLocation" not in config:
  print("Please define a HomeLocation")  
  exit()
if "x" not in config["HomeLocation"]:
  print("Please define an x for HomeLocation")
  exit()
if "y" not in config["HomeLocation"]:
  print("Please define an y for HomeLocation")  
  exit()

if "TargetDeck" not in config and "TargetCount" not in config:
  print("Please define either Targets or TargetCount")  
  exit()

# TODO protect the user...

CELLS_X = config["Size"]["x"]
CELLS_Y = config["Size"]["y"]

FPS = 10
WINDOWWIDTH = 640*3
WINDOWHEIGHT = 480*2
CELLWIDTH = int(WINDOWWIDTH / CELLS_X)
CELLHEIGHT = int(WINDOWHEIGHT / CELLS_Y)
WINDOWWIDTH = CELLS_X * CELLWIDTH + 1
WINDOWHEIGHT = CELLS_Y * CELLHEIGHT + 1

#             R    G    B
WHITE     = (255, 255, 255)
BLACK     = (  0,   0,   0)
RED       = (255,   0,   0)
GREEN     = (  0, 255,   0)
DARKGREEN = (  0, 155,   0)
DARKGRAY  = ( 40,  40,  40)
LIGHTGRAY  = ( 140,  140,  140)
ORANGE    = (255, 140,   0)
YELLOW    = (255, 255,   0)
BGCOLOR = BLACK

global FPSCLOCK, DISPLAYSURF, BASICFONT

pygame.init()
FPSCLOCK = pygame.time.Clock()
DISPLAYSURF = pygame.display.set_mode((WINDOWWIDTH, WINDOWHEIGHT))
BASICFONT = pygame.font.Font('freesansbold.ttf', 18)
pygame.display.set_caption('DroneSimulation')