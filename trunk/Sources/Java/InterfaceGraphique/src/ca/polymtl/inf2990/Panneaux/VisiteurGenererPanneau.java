package ca.polymtl.inf2990.Panneaux;

import javax.swing.JPanel;

import ca.polymtl.inf2990.Jeu.CanalRadio;
import ca.polymtl.inf2990.Jeu.ElementVisitable;
import ca.polymtl.inf2990.Jeu.JoueurHumain;
import ca.polymtl.inf2990.Jeu.JoueurVirtuel;
import ca.polymtl.inf2990.Jeu.Tournoi;

/**
 * Visiteur qui sert à créer le bon panneau selon l'élément donné.
 * @author Gabriel Couvrette
 *
 */
public class VisiteurGenererPanneau {
	
	/**
	 * Visite un element visitable.
	 * @param element L'element a visiter.
	 * @return Le panneau qui lui est associé.
	 */
	public PanneauAbstrait visiterElement(ElementVisitable element)
	{
		return element.accepter(this);
	}
	/**
	 * Action du visiteur qui genere un panneau pour le joueur virtuel.
	 * @param joueur Le joueur a utiliser pour generer le panneau
	 * @return le panneau a utiliser.
	 */
	public PanneauAbstrait visiter(JoueurVirtuel joueur)
	{
		return new SousPanneauJoueurVirtuel(joueur);	
	}
	
	/**
	 * Action du visiteur qui genere un panneau pour le joueur humain. (ATTENTION! NE FAIT RIEN POUR L'INSTANT!)
	 * @param joueur Le joueur humain 
	 * @return
	 */
	public PanneauAbstrait visiter(JoueurHumain joueur)
	{
		//return new SousPanneauJoueurVirtuel(JoueurVirtuel joueur);	
		return null; // on a pas a traiter ce cas pour l'instant.
	}
	
	
	
	/**
	 * Action du visiteur qui genere un panneau pour un tournoi.
	 * @param tournoi Le tournoi a modifier.
	 * @return
	 */
	public PanneauAbstrait visiter(Tournoi tournoi)
	{
		
		return new SousPanneauTournoi(tournoi);
	}
}
