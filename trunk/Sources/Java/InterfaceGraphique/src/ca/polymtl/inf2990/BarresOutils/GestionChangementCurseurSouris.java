package ca.polymtl.inf2990.BarresOutils;

import java.awt.Component;
import java.awt.Cursor;
import java.awt.Point;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.ImageIcon;
import javax.swing.JFrame;

import ca.polymtl.inf2990.Fenetre;

/**
 * Gestionnaire qui change le curseur (pointeur de la souris) selon l'état
 * courant. (Sélection, déplacement...)
 * 
 * @author Charles Étienne Lalonde
 * 
 */
public class GestionChangementCurseurSouris implements ActionListener
{
	private static GestionChangementCurseurSouris	instance_;

	private void GestionChangementCurseurSouris()
	{

	}

	/**
	 * Patron singleton;
	 * @return Une instance de cette classe.
	 */
	public static GestionChangementCurseurSouris obtenirInstance()
	{
		if (instance_ == null)
		{
			instance_ = new GestionChangementCurseurSouris();
		}
		return instance_;
	}

	@Override
	public void actionPerformed(final ActionEvent e)
	{
		Cursor curseur = ConstantesOutils.CURSEURS[Commandes.valueOf(e.getActionCommand()).obtenirId()];
		if (curseur == null)
		{
			try
			{
				// Création d'un curseur
				curseur = Toolkit.getDefaultToolkit().createCustomCursor(new ImageIcon(getClass().getResource("/ca/polymtl/inf2990/icones/" + ConstantesOutils.IMAGES[Commandes.valueOf(e.getActionCommand()).obtenirId()])).getImage(), new Point(0, 0), e.getActionCommand());
			}
			catch (Exception ex)
			{
				ex.printStackTrace();
			}
		}

		Fenetre.obtenirInstance().obtenirCanevas().setCursor(curseur);
	}

	/**
	 * retourne le parent du composant passé en paramètres.
	 * 
	 * @param composant Le composant.
	 * @return Son parent.
	 */
	public JFrame obtenirJFrameParent(Component composant)
	{
		while (!(composant instanceof JFrame))
		{
			composant = composant.getParent();
		}
		return (JFrame) composant;
	}
}
