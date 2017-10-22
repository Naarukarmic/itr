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
			newConfigs = list()
			#Generate a random config 
			q_rand = self.robot.shootRandomConfig()
			#For each CC, find the nearest config 
			q_near = list()
			q_new = list()
			for i in range(nbCC):
				q_near.append(self.ps.getNearestConfig(q_rand,i)[0])
				path = self.ps.directPath(q_near[i], q_rand, False)
				q_new.append(self.ps.configAtParam(path[1],deltaQ))
				#Test if new config is valid
				link = self.ps.directPath(q_near[i],q_new[i], True)
				if(link[0]):
					self.ps.addConfigToRoadmap(q_new[i])
					self.ps.addEdgeToRoadmap(q_near[i], q_new[i], link[1], True)
				#Add q_new to newConfig
				newConfigs.append(q_new[i])
		
			## Try connecting the new nodes together
			connectLink = self.ps.directPath(newConfigs[0], newConfigs[1], True)
			if(connectLink[0]):
				self.ps.addEdgeToRoadmap(newConfigs[0], newConfigs[1],connectLink[1], True)
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
