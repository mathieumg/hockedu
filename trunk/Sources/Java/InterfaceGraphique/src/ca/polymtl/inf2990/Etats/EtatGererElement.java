package ca.polymtl.inf2990.Etats;

import ca.polymtl.inf2990.Fenetre;
import ca.polymtl.inf2990.Jeu.ElementVisitable;
import ca.polymtl.inf2990.Panneaux.PanneauAbstrait;
import ca.polymtl.inf2990.Panneaux.PanneauGestionElement;

/**
 * État lorsqu'on a un panneau de gestion d'élément.
 * @author Gabriel Couvrette
 *
 */
public class EtatGererElement extends EtatAbstrait
{
	ElementVisitable element_;
	
	/**
	 * Utile pour le patron visiteur.
	 * @param element
	 */
	public void chargerElement(ElementVisitable element)
	{
		element_=element;
		((PanneauGestionElement)obtenirPanneau()).chargerElement(element);
		Fenetre.rafraichirFenetre();
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
		panneau.rafraichir();
	}

	@Override
	public Etats obtenirNom() {
		return Etats.MODIFIER_ELEMENT;
	}

	@Override
	protected PanneauAbstrait creerPanneau() {
		return new PanneauGestionElement("",element_);
	}

}
