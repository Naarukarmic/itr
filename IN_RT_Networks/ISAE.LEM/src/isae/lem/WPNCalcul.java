package isae.lem;

import java.util.HashMap;
import java.util.List;

import fr.isae.WoPANets.analysis.nc.model.NCFlow;
import fr.isae.WoPANets.analysis.nc.model.NCNetwork;
import fr.isae.WoPANets.analysis.nc.model.NCNode;
import fr.isae.WoPANets.analysis.nc.model.NCUtil;
import fr.isae.WoPANets.analysis.nc.model.NodeType;
import fr.isae.WoPANets.analysis.wrapper.AbstractAFDXEngine;

public class WPNCalcul extends AbstractAFDXEngine {

	@Override
	public String getName() {
		return "ISAE LEM";
	}

	@Override
	public String getDescription() {
		return "ISAE engine created by Pierre Lemarquand";
	}

	@Override
	public boolean computeDelaysBacklog(NCNetwork network, 
			HashMap<String, HashMap<Integer, Double>> backlogs,
			HashMap<String, HashMap<String, Double>> delays) {

		// Building HashMap from only the nodes of interest
		HashMap<NCNode, Double> nodes = new HashMap<NCNode, Double>();
		for(NCFlow ncFlow: network.getFlows()) {
			for(NCNode ncTarget: ncFlow.getPathes().keySet()) {
				for(NCNode nc: ncFlow.getPathes().get(ncTarget)) {
					if(nc.getNodeType() == NodeType.SWITCH_OUT_PORT || nc.getNodeType() == NodeType.END_SYSTEM_OUT_PORT)
					nodes.putIfAbsent(nc, 0.0);
				}
			}
		}
		
		while(hashtrue(nodes)) {
			for(NCFlow ncFlow: network.getFlows()) { // For each flow

				HashMap<String, Double> flowDelays = new HashMap<>();
				delays.put(ncFlow.getName(), flowDelays);
				HashMap<Integer, Double> switchBacklogs = new HashMap<>();

				for(NCNode ncTarget: ncFlow.getPathes().keySet()) { // for each target
					double delay = 0;

					for(NCNode nc: ncFlow.getPathes().get(ncTarget)) {
						if(nc.getNodeType() == NodeType.SWITCH_OUT_PORT && isCalc(network, nodes, nc)) {
							backlogs.put(nc.getName(), switchBacklogs);
							Service loc = new Service(NCUtil.getTransmissionCapacity(nc),
									NCUtil.getFlowsPassingThroughNode(network, nc),
									prevDelays(nodes, ncFlow, ncTarget));

							delay += loc.getDelay();
							nodes.replace(nc, delay);
							switchBacklogs.put(ncTarget.getPortNum(), loc.getBacklog()/8);
							System.out.println("Entered SW");
						}

						else if(nc.getNodeType() == NodeType.END_SYSTEM_OUT_PORT && isCalc(network, nodes, nc)) {
							Service loc = new Service(NCUtil.getTransmissionCapacity(nc),
									NCUtil.getFlowsPassingThroughNode(network, nc),
									prevDelays(nodes, ncFlow, ncTarget));

							delay += loc.getDelay();
							nodes.replace(nc, delay);
							System.out.println("Entered ES");
						}
					}
					flowDelays.put(ncTarget.getName(), delay*1e6);
				}
			}
		}
		System.out.println("Delays:");
		System.out.println(delays);
		System.out.println("Backlogs:");
		System.out.println(backlogs);
		return true;
	}

	// Test if all Nodes delays have been computed
	boolean hashtrue(HashMap<NCNode, Double> nodes) {
		boolean res = false;
		for(double i: nodes.values()) {
			if(i == 0.0) res = true;
		}
		return res;
	}

	// Allow propagation
	boolean isCalc(NCNetwork network, HashMap<NCNode, Double> nodes, NCNode node) {
		boolean res = true;
		if(nodes.get(node) == 0.0) {
			List<NCFlow> lf = NCUtil.getFlowsPassingThroughNode(network, node);
			for(NCFlow flow: lf) {
				List<NCNode> l = flow.getPathes().get(node);
				if(l != null) {
					for(NCNode n: l) {
						if(nodes.get(n) == 0.0) res = false;
					}
				}
			}
		} else res = false;
		return res;
	}
	
	// Sum previous delays
	double prevDelays(HashMap<NCNode, Double> nodes, NCFlow flow, NCNode node) {
		double res = 0.0;
		List<NCNode> l = flow.getPathes().get(node);
		if(l != null) {
			for(NCNode n: l) {
				res += nodes.getOrDefault(n, 0.0);
			}
		}
		return res;
	}
}

