package ca.polymtl.inf2990.Etats;

import ca.polymtl.inf2990.Jeu.Tournoi;
import ca.polymtl.inf2990.Panneaux.PanneauAbstrait;
import ca.polymtl.inf2990.Panneaux.PanneauGestionElement;
import ca.polymtl.inf2990.Panneaux.PanneauGestionTournoi;

/**
 * État lorsqu'on est en mode de gestion de tournoi.
 * @author Gabriel Couvrette
 *
 */
public class EtatGererTournoi extends EtatAbstrait
{
	Tournoi tournoi_;
	/**
	 * Construit l'état en prenant une valeur de tournoi.
	 * @param tournoi Le tournoi à gérer dans ce panneau.
	 */
	public EtatGererTournoi(Tournoi tournoi) {
		tournoi_=tournoi;
	}

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
		((PanneauGestionTournoi)obtenirPanneau()).chargerTournoi(tournoi_);		
	}

	@Override
	public Etats obtenirNom() {
		// TODO Auto-generated method stub
		return Etats.GERER_TOURNOI;
	}

	@Override
	protected PanneauAbstrait creerPanneau() {
		// TODO Auto-generated method stub
		return new PanneauGestionTournoi();
	}

}
