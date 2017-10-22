from math import sqrt
from hpp import Transform
from hpp.corbaserver.manipulation import ConstraintGraph
from manipulation import robot, vf, ps, Ground, Box, Pokeball, PathPlayer, gripperName, ballName

vf.loadEnvironmentModel (Ground, 'ground')
vf.loadEnvironmentModel (Box, 'box')
vf.moveObstacle ('box/base_link_0', [0.3+0.04, 0, 0.04, 0, 0, 0, 1])
vf.moveObstacle ('box/base_link_1', [0.3-0.04, 0, 0.04, 0, 0, 0, 1])
vf.moveObstacle ('box/base_link_2', [0.3, 0.04, 0.04, 0, 0, 0, 1])
vf.moveObstacle ('box/base_link_3', [0.3, -0.04, 0.04, 0, 0, 0, 1])

vf.loadObjectModel (Pokeball, 'pokeball')
robot.setJointBounds ('pokeball/root_joint', [-.4,.4,-.4,.4,-.1,1.,
                                              -1.0001, 1.0001,-1.0001, 1.0001,
                                              -1.0001, 1.0001,-1.0001, 1.0001,])

r = vf.createViewer ()

q1 = [0, -1.57, 1.57, 0, 0, 0, .3, 0, 0.025, 0, 0, 0, 1]
r (q1)

## Create graph
graph = ConstraintGraph (robot, 'graph')

## Create constraint of relative position of the ball in the gripper when ball is grasped
ballInGripper = [0, .137, 0, 0.5, 0.5, -0.5, 0.5]
ps.createTransformationConstraint ('grasp', gripperName, ballName, ballInGripper, 6*[True,])

## Create nodes and edges
graph.createNode(['grasp-placement','gripper-above-ball','placement','ball-above-ground','grasp'])

graph.createEdge('placement','placement','transit',1,'placement')
graph.createEdge('placement','gripper-above-ball','approach-ball',1,'placement')
graph.createEdge('gripper-above-ball','placement','move-gripper-away',1,'placement')
graph.createEdge('gripper-above-ball','grasp-placement','grasp-ball',1,'placement')
graph.createEdge('grasp-placement','gripper-above-ball','move-gripper-up',1,'placement')

graph.createEdge('grasp','grasp','transfer',1,'grasp')
graph.createEdge('grasp','ball-above-ground','approach-ground',1,'grasp')
graph.createEdge('ball-above-ground','grasp','take-ball-away',1,'grasp')
graph.createEdge('ball-above-ground','grasp-placement','put-ball-down',1,'grasp')
graph.createEdge('grasp-placement','ball-above-ground','take-ball-up',1,'grasp')


## Create transformation constraint : ball is in horizontal plane with free rotation around z
ps.createTransformationConstraint ('placement', '', ballName, [0,0,0.025,0, 0, 0, 1], [False, False, True, True, True, False,])

##  Create complement constraint : complement of placement 
ps.createTransformationConstraint ('placement/complement', '', ballName, [0,0,0.025,0, 0, 0, 1], [True, True, False, False, False, True,])

## Create transformation constraint : gripper can only move on z axis in the box 
ps.createTransformationConstraint ('only-z-movement', '', gripperName, [0,0,0.2,0, 0, 0, 1], [True, True, False, True, True, True])

## Create transformation constraint of relative position of the gripper when ball is in box 
GripperAboveBall = [0, .237, 0, 0.5, 0.5, -0.5, 0.5]
ps.createTransformationConstraint ('gripper-ball', gripperName, ballName,  GripperAboveBall, 6*[True,])

## Create transformation constraint of relative position of the ball above ground 
BallAboveGround = [0,0,0.2,0, 0, 0, 1]
ps.createTransformationConstraint ('ball-ground', '', ballName,  BallAboveGround, [False, False, True, True, True, False])

ps.setConstantRightHandSide ('placement', True)
ps.setConstantRightHandSide('ball-ground', True)
ps.setConstantRightHandSide('gripper-ball', True)

ps.setConstantRightHandSide ('placement/complement', False)
ps.setConstantRightHandSide('only-z-movement', False)  

## Set constraints of nodes and edges
graph.setConstraints (node='placement', numConstraints = ['placement'])
graph.setConstraints (node='gripper-above-ball', numConstraints = ['placement','gripper-ball'])
graph.setConstraints (node='grasp-placement', numConstraints = ['placement','grasp'])
graph.setConstraints (node='ball-above-ground', numConstraints = ['ball-ground', 'grasp'])
graph.setConstraints (node='grasp', numConstraints = ['grasp'])

graph.setConstraints (edge='transit',numConstraints = ['placement/complement'])

graph.setConstraints (edge='approach-ball', numConstraints = ['placement/complement'])
graph.setConstraints (edge='grasp-ball', numConstraints = ['only-z-movement'])
graph.setConstraints (edge='take-ball-up', numConstraints = ['only-z-movement','grasp'])
graph.setConstraints (edge='take-ball-away', numConstraints = [])

graph.setConstraints (edge='transfer',numConstraints = [])

graph.setConstraints (edge='approach-ground', numConstraints = [])
graph.setConstraints (edge='put-ball-down', numConstraints = ['only-z-movement', 'grasp'])
graph.setConstraints (edge='move-gripper-up', numConstraints = ['placement/complement'])
graph.setConstraints (edge='move-gripper-away', numConstraints = ['placement/complement'])

res, q_init, error = graph.applyNodeConstraints ('placement', q1)
q2 = q1 [::]
q2 [7] = .2

res, q_goal, error = graph.applyNodeConstraints ('placement', q2)

ps.setInitialConfig (q_init)
ps.addGoalConfig (q_goal)
ps.selectPathValidation ("Discretized", 0.01)
ps.selectPathProjector ("Progressive", 0.1)

pp = PathPlayer (ps.client.basic, r)
