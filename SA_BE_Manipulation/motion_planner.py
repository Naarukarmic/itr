class MotionPlanner:
    def __init__ (self, robot, ps):
        self.robot = robot
        self.ps = ps

    def solveBiRRT (self, maxIter = float("inf")):
        deltaQ = 0.01
        self.ps.prepareSolveStepByStep()
        finished = False

        # In the framework of the course, we restrict ourselves to 2 connected components.
        nbCC = self.ps.numberConnectedComponents()
        if nbCC != 2:
            raise Exception("There should be 2 connected components.")

        iter = 0
        while True:
            #### RRT begin
            ## Check whether direct path exists
            for i in self.ps.nodesConnectedComponent(0):
                if self.ps.numberConnectedComponents() == 2 : 
                    for j in self.ps.nodesConnectedComponent(1):
                        valid, path, msg = self.ps.directPath(i, j, True)
                        if valid:
                            self.ps.addEdgeToRoadmap(i, j, path, True)
                            break
            # Generate a random config 
            q_rand = self.robot.shootRandomConfig()
            # For each CC, find the nearest config and extend it
            if self.ps.numberConnectedComponents() == 2:
                for i in range(2):
                    q, plen = self.ps.getNearestConfig(q_rand, i)
                    valid, path, msg = self.ps.directPath(q, q_rand, True)
                    plen = self.ps.pathLength(path)
                    conf = self.ps.configAtParam(path, plen)
                    
                    self.ps.addConfigToRoadmap(conf)
                    self.ps.addEdgeToRoadmap(q, conf, path, True)
            #### RRT end
            ## Check if the problem is solved.
            nbCC = self.ps.numberConnectedComponents()
            if nbCC == 1:
                # Problem solved
                finished = True
                break
            iter = iter + 1
            if iter > maxIter:
                break
        if finished:
                self.ps.finishSolveStepByStep()
                return self.ps.numberPaths() - 1

    def solvePRM (self):
        self.ps.prepareSolveStepByStep()
        #### PRM begin
        #### PRM end
        self.ps.finishSolveStepByStep()
