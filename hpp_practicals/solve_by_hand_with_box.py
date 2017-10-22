from grasp_ball import q_init, q_goal, robot, ps, graph, r, pp

success = False
trial = 0
path = []
state_dict = {
    0: 'approach-ball',
    1: 'grasp-ball',
    2: 'take-ball-up',
    3: 'take-ball-away',
    4: 'approach-ground',
    5: 'put-ball-down',
    6: 'move-gripper-up',
    7: 'move-gripper-away',
}

for state in sorted(state_dict):
    while not success:
        paths = list ()
        print ("trial {0}".format (trial)); trial += 1
        q = robot.shootRandomConfig ()
        res, q1, err = graph.generateTargetConfig (state_dict[state], q_init, q)
        if not res: continue
        res, msg = robot.isConfigValid (q1)
        if not res: continue
        res, pid, msg = ps.directPath (q_init, q1, True)
        paths.append (pid)
        if not res: continue
        path += paths
        q_init = q1
        success = True
    success = False

for step in path:
    pp(step)
