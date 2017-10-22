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

## Create constraint of relative position of the ball in the gripper when ball
## is grasped
ballInGripper = [0, .137, 0, 0.5, 0.5, -0.5, 0.5]
ps.createTransformationConstraint ('grasp', gripperName, ballName,
                                   ballInGripper, 6*[True,])

## Create nodes and edges
#  Warning the order of the nodes is important. When checking in which node
#  a configuration lies, node constraints will be checked in the order of node
#  creation.
graph.createNode (['grasp-placement', 'gripper-above-ball', 'placement', 'ball-above-ground', 'grasp'])
graph.createEdge ('placement', 'placement', 'transit', 1, 'placement')
graph.createEdge ('grasp', 'grasp', 'transfer', 1, 'grasp')
graph.createEdge ('placement', 'gripper-above-ball', 'approach-ball', 1, 'placement')
graph.createEdge ('gripper-above-ball', 'placement', 'move-gripper-away', 1, 'placement')
graph.createEdge ('gripper-above-ball', 'grasp-placement', 'grasp-ball', 1, 'placement')
graph.createEdge ('grasp-placement', 'gripper-above-ball', 'move-gripper-up', 1, 'placement')
graph.createEdge ('grasp-placement', 'ball-above-ground', 'take-ball-up', 1, 'grasp')
graph.createEdge ('ball-above-ground', 'grasp-placement', 'put-ball-down', 1, 'grasp')
graph.createEdge ('ball-above-ground', 'grasp', 'take-ball-away', 1, 'grasp')
graph.createEdge ('grasp', 'ball-above-ground', 'approach-ground', 1, 'grasp')

## Create transformation constraint : ball is in horizontal plane with free
## rotation around z

ps.createTransformationConstraint ('placement', '', ballName, [0,0,0.025,0, 0, 0, 1], [False, False, True, True, True, False,])
#  Create complement constraint
ps.createTransformationConstraint ('placement/complement', '', ballName, [0,0,0.025,0, 0, 0, 1], [True, True, False, False, False, True,])

ps.createTransformationConstraint ('above-b', gripperName, ballName, [0, 0, 0, 0.5, 0.5, -0.5, 0.5], [False, False, True, True, True, True,])
ps.createTransformationConstraint ('above-a', gripperName, '', [0.2,0,0,0, 0, 0, 0], [True, True, False, False, False, False,])

ps.setConstantRightHandSide ('placement', True)
ps.setConstantRightHandSide ('placement/complement', False)

## Set constraints of nodes and edges
graph.setConstraints (node='placement', numConstraints = ['placement'])
graph.setConstraints (node='grasp', numConstraints = ['grasp'])
graph.setConstraints (edge='transit', numConstraints = ['placement/complement'])
graph.setConstraints (edge='grasp-ball', numConstraints = ['placement/complement'])
# These edges are in node placement
graph.setConstraints (node='gripper-above-ball', numConstraints = ['above-b'])
graph.setConstraints (edge='approach-ball', numConstraints = [])
graph.setConstraints (edge='move-gripper-away', numConstraints = [])
# These edges are in node 'grasp'
graph.setConstraints (node='ball-above-ground', numConstraints = ['grasp', 'above-a'])
graph.setConstraints (edge='transfer',     numConstraints = [])
graph.setConstraints (edge='put-ball-down', numConstraints = [])
graph.setConstraints (edge='take-ball-up', numConstraints = [])
graph.setConstraints (node='grasp-placement', numConstraints = ['grasp', 'placement'])

res, q_init, error = graph.applyNodeConstraints ('placement', q1)
q2 = q1 [::]
q2 [7] = .2

res, q_goal, error = graph.applyNodeConstraints ('placement', q2)

ps.setInitialConfig (q_init)
ps.addGoalConfig (q_goal)
ps.selectPathValidation ("Discretized", 0.01)
ps.selectPathProjector ("Progressive", 0.1)

pp = PathPlayer (ps.client.basic, r)

