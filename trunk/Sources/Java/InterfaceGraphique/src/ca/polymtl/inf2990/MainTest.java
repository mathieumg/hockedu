package ca.polymtl.inf2990;

import ca.polymtl.inf2990.BarresOutils.BarreEdition;
import ca.polymtl.inf2990.Composants.*;
import ca.polymtl.inf2990.GestionnaireImages.*;

public class MainTest {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		
		
		BarreEdition barre = new BarreEdition();
		barre.setVisible(true);
		
		BoutonHockey boutonH = new BoutonHockey(GestionnaireImages.NomImage.BOUTON_NORMAL, GestionnaireImages.NomImage.BOUTON_ENFONCE, GestionnaireImages.NomImage.BOUTON_SURVOLE);
		boutonH.setVisible(true);
		
		
		/*for(int i=0; i<100; i++)
		{
			barre.toutCacher();
			try { Thread.sleep(1000); } catch (InterruptedException e) {}
			barre.toutAfficher();
			try { Thread.sleep(1000); } catch (InterruptedException e) {}
		}*/
		
	}

}
