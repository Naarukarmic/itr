package isae.lem;

import java.util.List;

import fr.isae.WoPANets.analysis.nc.maths.Calcul;
import fr.isae.WoPANets.analysis.nc.maths.RateLatency;
import fr.isae.WoPANets.analysis.nc.maths.ShapedTokenBucket;
import fr.isae.WoPANets.analysis.nc.model.NCFlow;
import fr.isae.WoPANets.analysis.nc.model.NCUtil;

public class Service {
	
	private RateLatency service;
	private ShapedTokenBucket arrival;
	
	
	public Service(double sr_c, List<NCFlow> l, double delay) {
		service = new RateLatency(sr_c);
		arrival = new ShapedTokenBucket(0);
		
		for(NCFlow ncFlow: l) {
			ShapedTokenBucket arr2 = new ShapedTokenBucket(8.0 * NCUtil.getMaxPayloadSize(ncFlow) / NCUtil.getPeriod(ncFlow),
					8.0 * NCUtil.getMaxPayloadSize(ncFlow));
			arr2 = ShapedTokenBucket.shiftLeft(arr2, delay);
			System.out.println(delay);
			arrival = ShapedTokenBucket.add(arrival, arr2);
		}
	}
//  Arrival rate and b
//	8.0 * NCUtil.getMaxPayloadSize(ncFlow) / NCUtil.getPeriod(ncFlow)
//	8.0 * NCUtil.getMaxPayloadSize(ncFlow))
	
	
	public double getDelay() {
		double delay = Calcul.getMaxHorizontalDeviation(arrival, service);
		return delay;
	}
	
	public double getBacklog() {
		double backlog = Calcul.getMaxVerticalDeviation(arrival, service);
		return backlog;
	}

}
