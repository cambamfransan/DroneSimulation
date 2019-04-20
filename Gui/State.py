import Configuration
import random, json, os

# class Drone:
#   def __init__(self, bat, home, vf, target):
#     self.batteryLifeLeft = bat
#     self.loc = home
#     self.valueFunction = vf
#     self.target = target

class State:
  def __init__(self, useConfig):
    if useConfig:
      self.size = {}
      self.size["x"] = Configuration.config["Size"]["x"]
      self.size["y"] = Configuration.config["Size"]["y"]
      self.homeLocation = {}
      self.homeLocation["x"] = Configuration.config["HomeLocation"]["x"]
      self.homeLocation["y"] = Configuration.config["HomeLocation"]["y"]
      self.targets = {}
      self.done = False
      
      if "TargetDeck" in Configuration.config:
        self.targets["Left"] = Configuration.config["TargetDeck"]
        
      if "TargetCount" in Configuration.config:
        self.targetCount = Configuration.config["TargetCount"]
      else:
        self.targetCount = len(self.targets["Left"])
  
      if "Left" not in self.targets:
        self.targets["Left"] = []
        for i in range(self.targetCount):
          self.targets["Left"].append({
            "x": random.randint(0,self.size["x"] - 1), 
            "y": random.randint(0,self.size["y"] - 1)})
      
      self.valueFunctions = Configuration.config["ValueFunction"]
      self.diffPercentage = Configuration.config["DiffPercentage"]
      v = 0
      self.drones = []
      for i in Configuration.config["Drones"]:
        self.drones.append({"batteryLifeLeft": i, "Location": self.homeLocation,"ValueFunction": self.valueFunctions[v]["ValueFunctions"]})
        if len(self.drones) / len(Configuration.config["Drones"]) > self.valueFunctions[v]["Percentage"]:
          v = v + 1

      self.map = []
      for i in range(self.size["x"]):
        toAdd = []
        for j in range(self.size["y"]):
          toAdd.append(0)
        self.map.append(toAdd)
      for t in self.targets["Left"]:
        self.map[t["x"]][t["y"]] += 1 

    else:
      self.readInState()

  def readInState(self):
    with open("./State.txt", 'r') as content_file:
      content = content_file.read()

    config = json.loads(content)
    self.drones = []
    for i in config["Drones"]:
      self.drones.append(i)

    self.targets["Left"] = config["TargetDeck"]["Left"]
    if "Hit" in config["TargetDeck"]:
      self.targets["Hit"] = config["TargetDeck"]["Hit"]
    if "Done" in config:
      self.done = config["Done"]
    else:
      self.done = False
    self.map = config["Map"]

  def writeState(self):
    data = {}
    data["Size"] = self.size
    data["HomeLocation"] = self.homeLocation
    data["TargetDeck"] = self.targets
    data["Drones"] = self.drones
    data["ValueFunctions"] = self.valueFunctions
    data["DiffPercentage"] = self.diffPercentage
    data["Done"] = self.done
    data["Map"] = self.map
    with open('./State.txt', 'w') as outfile:  
      json.dump(data, outfile)

  def solveState(self):
    self.writeState()
    dirname = os.path.dirname(os.path.abspath(__file__))
    cmd = os.path.join(dirname, 'Drones')
    os.system(cmd + " solveState State.txt")
    self.readInState()
