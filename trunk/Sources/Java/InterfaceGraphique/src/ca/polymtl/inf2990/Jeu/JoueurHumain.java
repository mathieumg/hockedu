package ca.polymtl.inf2990.Jeu;

import javax.swing.JPanel;

import ca.polymtl.inf2990.Panneaux.PanneauAbstrait;
import ca.polymtl.inf2990.Panneaux.VisiteurGenererPanneau;

/**
 * Classe repr�sente un joueur control� par un humain dans le mode jeu
 * 
 * @author Michael Ferris
 * @date 17-02-2012
 */
public class JoueurHumain extends JoueurAbstrait implements ElementVisitable
{

	/**
	 * Constructeur pas param�tres du JoueurHumain
	 * 
	 * @param nom Nom du joueur
	 */
	public JoueurHumain(final String nom)
	{
		super(nom, TypeJoueur.HUMAIN);
		// TODO Auto-generated constructor stub
	}
	
	/**
	 * Utile pour le visiteur qui g�n�re le panneau de gestion d'�l�ments.
	 */
	@Override
	public PanneauAbstrait accepter(VisiteurGenererPanneau visiteur) {
		return visiteur.visiter(this);		
	}

}
