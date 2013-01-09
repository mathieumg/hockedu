package ca.polymtl.inf2990.Etats;

import ca.polymtl.inf2990.Panneaux.PanneauAbstrait;
import ca.polymtl.inf2990.Panneaux.PanneauListeCanaux;

/**
 * État lorsque Java est en mode de liste des canaux de radio existants.
 * @author Gabriel Couvrette
 *
 */
public class EtatListeCanaux extends EtatAbstrait
{

	@Override
	public void initialiser() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void terminer() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void mettreAuRepos() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void reveiller() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public Etats obtenirNom() {
		// TODO Auto-generated method stub
		return Etats.MODE_LISTE_CANAUX;
	}

	@Override
	protected PanneauAbstrait creerPanneau() {
		return new PanneauListeCanaux("Liste des canaux");
	}


}
