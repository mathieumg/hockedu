package ca.polymtl.inf2990.Etats;

import ca.polymtl.inf2990.Fenetre;
import ca.polymtl.inf2990.Panneaux.PanneauAbstrait;

/**
 * Classe abstraite repr�sentant un �tat d'affichage de panneaux.
 * @author Charles �tienne Lalonde
 *
 */
public abstract class EtatAbstrait
{
	protected PanneauAbstrait		panneau;
	protected static EtatAbstrait	instance_;

	/**
	 * Cr�e un �tat abstrait.
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
	 * M�thode appel�e � la construction de l'�tat.
	 */
	public abstract void initialiser();

	/**
	 * M�thode appel�e quand on enl�ve l'�tat de la liste des �tats.
	 */
	public abstract void terminer();

	/**
	 * M�thode appel�e lorsqu'on quitte l'�tat.
	 */
	public abstract void mettreAuRepos();

	/**
	 * M�thode appel�e lorsqu'on entre dans l'�tat.
	 */
	public abstract void reveiller();

	/**
	 * M�thode appel�e pour rafraichir le panneau de l'�tat.
	 */
	public void rafraichir()
	{
		obtenirPanneau().rafraichir();
	}

	/**
	 * Retourne le nom de cet �tat.
	 * @return Le nom, selon l'enum Etats.
	 */
	public abstract Etats obtenirNom();

	protected abstract PanneauAbstrait creerPanneau();

	/**
	 * Retourne le panneau associ� � cet �tat.
	 * @return Le panneau de cet �tat.
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
