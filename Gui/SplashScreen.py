import pygame, sys, os
import Configuration

class SplashScreen:
  def __init__(self, targetsHit):
    gameOverFont = Configuration.BASICFONT
    gameSurf = Configuration.BASICFONT.render('Targets Hit: ', True, Configuration.WHITE)
    overSurf = Configuration.BASICFONT.render(str(targetsHit), True, Configuration.WHITE)
    gameRect = gameSurf.get_rect()
    overRect = overSurf.get_rect()
    gameRect.midtop = (Configuration.WINDOWWIDTH / 2, 10)
    overRect.midtop = (Configuration.WINDOWWIDTH / 2, gameRect.height + 10 + 25)

    Configuration.DISPLAYSURF.blit(gameSurf, gameRect)
    Configuration.DISPLAYSURF.blit(overSurf, overRect)
    pygame.display.update()
    pygame.time.wait(5000)
