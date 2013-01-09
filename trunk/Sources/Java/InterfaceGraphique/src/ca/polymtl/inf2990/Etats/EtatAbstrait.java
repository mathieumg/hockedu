package ca.polymtl.inf2990.Etats;

import ca.polymtl.inf2990.Fenetre;
import ca.polymtl.inf2990.Panneaux.PanneauAbstrait;

/**
 * Classe abstraite représentant un État d'affichage de panneaux.
 * @author Charles Étienne Lalonde
 *
 */
public abstract class EtatAbstrait
{
	protected PanneauAbstrait		panneau;
	protected static EtatAbstrait	instance_;

	/**
	 * Crée un état abstrait.
	 */
	public EtatAbstrait()
	{
		panneau = Fenetre.obtenirInstance().obtenirPanneau(obtenirNom());
		if (panneau == null)
		{
			Fenetre.obtenirInstance().ajouterElement(obtenirPanneau(), obtenirNom());
		}
		initialiser();
	}

	/**
	 * Méthode appelée à la construction de l'état.
	 */
	public abstract void initialiser();

	/**
	 * Méthode appelée quand on enlève l'état de la liste des états.
	 */
	public abstract void terminer();

	/**
	 * Méthode appelée lorsqu'on quitte l'état.
	 */
	public abstract void mettreAuRepos();

	/**
	 * Méthode appelée lorsqu'on entre dans l'état.
	 */
	public abstract void reveiller();

	/**
	 * Méthode appelée pour rafraichir le panneau de l'état.
	 */
	public void rafraichir()
	{
		obtenirPanneau().rafraichir();
	}

	/**
	 * Retourne le nom de cet état.
	 * @return Le nom, selon l'enum Etats.
	 */
	public abstract Etats obtenirNom();

	protected abstract PanneauAbstrait creerPanneau();

	/**
	 * Retourne le panneau associé à cet état.
	 * @return Le panneau de cet état.
	 */
	public PanneauAbstrait obtenirPanneau()
	{
		if (panneau == null)
		{
			panneau = creerPanneau();
		}
		return panneau;
	}
}
