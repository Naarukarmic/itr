from grasp_ball import q_init, q_goal, robot, ps, graph, r, pp

success = False
trial = 0
while not success:
    paths = list ()
    print ("trial {0}".format (trial)); trial += 1
    q = robot.shootRandomConfig ()
    res, q1, err = graph.generateTargetConfig ('grasp-ball', q_init, q)
    if not res: continue
    res, msg = robot.isConfigValid (q1)
    if not res: continue
    res, pid, msg = ps.directPath (q_init, q1, True)
    paths.append (pid)
    if not res: continue
    success = True


    