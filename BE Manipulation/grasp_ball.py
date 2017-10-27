from math import sqrt
from hpp import Transform
from hpp.corbaserver.manipulation import ConstraintGraph
from manipulation import robot, vf, ps, Ground, Box, Pokeball, PathPlayer, gripperName, ballName

vf.loadEnvironmentModel (Ground, 'ground')
vf.loadObjectModel (Pokeball, 'pokeball')
robot.setJointBounds ('pokeball/root_joint', [-.4,.4,-.4,.4,-.1,2.,
                                              -1.0001, 1.0001,-1.0001, 1.0001,
                                              -1.0001, 1.0001,-1.0001, 1.0001,])

r = vf.createViewer ()

q1 = [0, -1.57, 1.57, 0, 0, 0, .3, 0, 0.025, 0, 0, 0, 1]
r (q1)

## Create constraint graph
graph = ConstraintGraph (robot, 'graph')

## Create constraint of relative position of the ball in the gripper when ball
## is grasped
ballInGripper = [0, .137, 0, 0.5, 0.5, -0.5, 0.5]
ps.createTransformationConstraint ('grasp', gripperName, ballName,
                                   ballInGripper, 6*[True,])

## Create nodes and edges
#  Warning the order of the nodes is important. When checking in which node
#  a configuration lies, node constraints will be checked in the order of node
#  creation.
graph.createNode (['grasp', 'placement'])
graph.createEdge ('placement', 'placement', 'transit', 1, 'placement')
graph.createEdge ('grasp', 'grasp', 'transfer', 1, 'grasp')
graph.createEdge ('placement', 'grasp', 'grasp-ball', 1, 'placement')
graph.createEdge ('grasp', 'placement', 'release-ball', 1, 'grasp')

## Create transformation constraint : ball is in horizontal plane with free
## rotation around z

ps.createTransformationConstraint ('placement', '', ballName, [0,0,0.025,0, 0, 0, 1], [False, False, True, True, True, False,])
#  Create complement constraint
ps.createTransformationConstraint ('placement/complement', '', ballName, [0,0,0.025,0, 0, 0, 1], [True, True, False, False, False, True,])

ps.setConstantRightHandSide ('placement', True)
ps.setConstantRightHandSide ('placement/complement', False)

## Set constraints of nodes and edges
graph.setConstraints (node='placement', numConstraints = ['placement'])
graph.setConstraints (node='grasp', numConstraints = ['grasp'])
graph.setConstraints (edge='transit',
                      numConstraints = ['placement/complement'])
graph.setConstraints (edge='grasp-ball',
                      numConstraints = ['placement/complement'])
# These edges are in node 'grasp'
graph.setConstraints (edge='transfer',     numConstraints = [])
graph.setConstraints (edge='release-ball', numConstraints = [])

## Project initial configuration on state 'placement'
res, q_init, error = graph.applyNodeConstraints ('placement', q1)
q2 = q1 [::]
q2 [7] = .2

## Project goal configuration on state 'placement'
res, q_goal, error = graph.applyNodeConstraints ('placement', q2)

## Define manipulation planning problem
ps.setInitialConfig (q_init)
ps.addGoalConfig (q_goal)
ps.selectPathValidation ("Dichotomy", 0)
ps.selectPathProjector ("Progressive", 0.1)

pp = PathPlayer (ps.client.basic, r)

## Build relative position of the ball with respect to the gripper
for i in range (100):
  q = robot.shootRandomConfig ()
  res,q3,err = graph.generateTargetConfig ('grasp-ball', q_init, q)
  if res and robot.isConfigValid (q3): break;

if res:
  robot.setCurrentConfig (q3)
  gripperPose = Transform (robot.getJointPosition (gripperName))
  ballPose = Transform (robot.getJointPosition (ballName))
  gripperGraspsBall = gripperPose.inverse () * ballPose
  gripperAboveBall = Transform (gripperGraspsBall)
  gripperAboveBall.translation [2] += .1
