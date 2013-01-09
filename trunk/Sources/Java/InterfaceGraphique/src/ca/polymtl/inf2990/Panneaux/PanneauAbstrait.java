/**
 * 
 */
package ca.polymtl.inf2990.Panneaux;

import javax.swing.JPanel;

/**
 * Interface pour d�finir les panneaux qui seront utilis�s dans les diff�rents �tats.
 * 
 * @author Charles �tienne Lalonde
 * 
 */
public abstract class PanneauAbstrait extends JPanel
{
	/**
	 * Constructeur par d�faut pour cr�er le panneau.
	 */
	public PanneauAbstrait()
	{
		creer();
	}
	
	/**
	 * M�thode appel�e par le constructeur par d�faut pour cr�er le panneau.
	 */
	protected abstract void creer();

	/**
	 * M�thode � appeler pour rafra�chir l'affichage du panneau.
	 */
	public abstract void rafraichir();
}
