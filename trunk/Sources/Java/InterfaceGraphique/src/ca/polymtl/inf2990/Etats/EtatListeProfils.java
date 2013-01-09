package ca.polymtl.inf2990.Etats;

import ca.polymtl.inf2990.Panneaux.PanneauAbstrait;
import ca.polymtl.inf2990.Panneaux.PanneauListeProfils;

/**
 * État lorsque le panneau de liste les profils de joueurs virtuels.
 * @author Gabriel Couvrette
 *
 */
public class EtatListeProfils extends EtatAbstrait
{

	@Override
	public void initialiser() {
		//rien. dans le constructeur deja.
	}

	@Override
	public void terminer() {		
	}

	@Override
	public void mettreAuRepos() {	
	}

	@Override
	public void reveiller() {
		panneau.rafraichir();
	}

	public Etats obtenirNom() {
		return Etats.LISTE_PROFILS;
	}

	@Override
	protected PanneauAbstrait creerPanneau() {
		return new PanneauListeProfils("Liste des profils");
	}

}
