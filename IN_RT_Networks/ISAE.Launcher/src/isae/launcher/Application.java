package isae.launcher;

import org.eclipse.equinox.app.IApplication;
import org.eclipse.equinox.app.IApplicationContext;

import fr.isae.wpn.util.Analysis;

public class Application implements IApplication {
	@Override
	public Object start(IApplicationContext context) throws Exception {
		// This is a path to a WoPANets archive
		String path2project = "/home/ingenieurs18/p.lemarquand/Documents/CaseStudyCT/Z_ESE.zip";
		Analysis an = new Analysis(path2project);
		
		// Here I launch the delays analysis, using the engine "ISAE LEM" and the option 1
		an.launchSimpleMainAnalysis("ISAE LEM", 1);
		return IApplication.EXIT_OK;
	}
	
	@Override
	public void	stop() {
		// nothing to do
	}
}