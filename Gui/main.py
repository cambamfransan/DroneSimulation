import random, pygame, sys
from pygame.locals import *
import Configuration
import Simulation
import SplashScreen

def main():

  myGame = Simulation.DroneSimulation()
  splashScreen = SplashScreen.SplashScreen(myGame.getTargetsHit())
  
  pygame.display.quit()
  pygame.quit()
  sys.exit(0)

if __name__ == '__main__':
  main()