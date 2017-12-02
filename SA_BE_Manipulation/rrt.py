from hpp.corbaserver.practicals.ur5 import Robot
from hpp.corbaserver import ProblemSolver
from hpp.gepetto import ViewerFactory, PathPlayer

robot = Robot ('ur5')
ps = ProblemSolver (robot)

vf = ViewerFactory (ps)

vf.loadObstacleModel ("hpp_practicals","ur_benchmark/obstacles","obstacles")
vf.loadObstacleModel ("hpp_practicals","ur_benchmark/table","table")
vf.loadObstacleModel ("hpp_practicals","ur_benchmark/wall","wall")

r = vf.createViewer ()

q1 = [0, -1.57, 1.57, 0, 0, 0]; q2 = [0.2, -1.57, -1.8, 0, 0.8, 0]
q3 = [1.57, -1.57, -1.8, 0, 0.8, 0]

r (q2)
r (q3)

ps.setInitialConfig (q2)
ps.addGoalConfig (q3)

from motion_planner import MotionPlanner
m = MotionPlanner (robot, ps)
pathId = m.solveBiRRT (maxIter = 1000)
print pathId
pp = PathPlayer (robot.client, r)
# pp (pathId)
