package ca.polymtl.inf2990.Etats;

import ca.polymtl.inf2990.Panneaux.PanneauAbstrait;
import ca.polymtl.inf2990.Panneaux.PanneauListeTournois;

/**
 * Classe représentant l'état d'affichage de la liste des tournois existants.
 * @author Gabriel Couvrette
 *
 */
public class EtatListeTournois extends EtatAbstrait{

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
		return Etats.MODE_LISTE_TOURNOIS;
	}

	@Override
	protected PanneauAbstrait creerPanneau() {
		return new PanneauListeTournois("Liste des tounois");
	}

}
