import random, pygame, os
import Configuration
import State

class DroneSimulation:
  def __init__(self):
    self.state = State.State(True)
    self.draw()
    while not self.state.done:
      self.state.solveState()
      self.draw()
      self.state.writeState()
      Configuration.FPSCLOCK.tick(Configuration.FPS)

  def getTargetsHit(self):
    return len(self.state.targets["Hit"])

  def draw(self):
    for i in range(self.state.size["x"]):
      for j in range(self.state.size["y"]):
        rect = pygame.Rect(i* Configuration.CELLWIDTH , j * Configuration.CELLHEIGHT, Configuration.CELLWIDTH, Configuration.CELLHEIGHT)
        pygame.draw.rect(Configuration.DISPLAYSURF, Configuration.BLACK, rect)

    for i in range(self.state.size["x"] + 1):
      pygame.draw.line(Configuration.DISPLAYSURF, Configuration.LIGHTGRAY, (i*Configuration.CELLWIDTH, 0), (i*Configuration.CELLWIDTH, Configuration.WINDOWHEIGHT))

    for j in range(self.state.size["y"] + 1):
      pygame.draw.line(Configuration.DISPLAYSURF, Configuration.LIGHTGRAY, (0, j*Configuration.CELLHEIGHT), (Configuration.WINDOWWIDTH, j*Configuration.CELLHEIGHT))

    for target in self.state.targets["Left"]:
      rect = pygame.Rect(target["x"]* Configuration.CELLWIDTH, target["y"] * Configuration.CELLHEIGHT, Configuration.CELLWIDTH, Configuration.CELLHEIGHT)
      pygame.draw.rect(Configuration.DISPLAYSURF, Configuration.RED, rect)

    for drone in self.state.drones:
      rect = pygame.Rect(drone["Location"]["x"]* Configuration.CELLWIDTH, drone["Location"]["y"] * Configuration.CELLHEIGHT, Configuration.CELLWIDTH, Configuration.CELLHEIGHT)
      pygame.draw.rect(Configuration.DISPLAYSURF, Configuration.GREEN, rect)

    rect = pygame.Rect(self.state.homeLocation["x"]* Configuration.CELLWIDTH, self.state.homeLocation["y"] * Configuration.CELLHEIGHT, Configuration.CELLWIDTH, Configuration.CELLHEIGHT)
    pygame.draw.rect(Configuration.DISPLAYSURF, Configuration.ORANGE, rect)

    pygame.display.update()
    
