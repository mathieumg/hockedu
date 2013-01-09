/**
 * 
 */
package ca.polymtl.inf2990.Panneaux;

import javax.swing.JPanel;

/**
 * Interface pour définir les panneaux qui seront utilisés dans les différents états.
 * 
 * @author Charles Étienne Lalonde
 * 
 */
public abstract class PanneauAbstrait extends JPanel
{
	/**
	 * Constructeur par défaut pour créer le panneau.
	 */
	public PanneauAbstrait()
	{
		creer();
	}
	
	/**
	 * Méthode appelée par le constructeur par défaut pour créer le panneau.
	 */
	protected abstract void creer();

	/**
	 * Méthode à appeler pour rafraîchir l'affichage du panneau.
	 */
	public abstract void rafraichir();
}
