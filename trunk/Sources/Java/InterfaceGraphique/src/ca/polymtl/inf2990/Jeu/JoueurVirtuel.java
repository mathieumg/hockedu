package ca.polymtl.inf2990.Jeu;

import javax.swing.JPanel;

import ca.polymtl.inf2990.Panneaux.PanneauAbstrait;
import ca.polymtl.inf2990.Panneaux.VisiteurGenererPanneau;

/**
 * Classe qui représente un joueur virtuel dans le mode jeu
 * 
 * @author Michael Ferris
 * @date 17-02-2012
 */
public class JoueurVirtuel extends JoueurAbstrait implements ElementVisitable
{

	/**
	 * Constructeur pas paramètres du JoueurVirtuel
	 * 
	 * @param nom Nom du joueur Virtuel
	 * @param vitesse vitesse du joueur Virtuel
	 * @param probabiliteEchec probabiliteEchec du joueur Virtuel
	 */
	public JoueurVirtuel(final String nom, final int vitesse, final int probabiliteEchec)
	{
		super(nom, TypeJoueur.VIRTUEL);
		vitesse_ = vitesse;
		probabiliteEchec_ = probabiliteEchec;
	}

	/**
	 * Retourne la probabilite que l'AI rate son coup.
	 * 
	 * @return probabiliteEchec_
	 */
	public int obtenirProbabiliteEchec()
	{
		return probabiliteEchec_;
	}

	/**
	 * Retorune la probabilite de succes.
	 * 
	 * @return probabilite de succes (100% - probabiliteEchec_)
	 */
	public int obtenirProbabiliteSucces()
	{
		return 100 - probabiliteEchec_;
	}

	/**
	 * @param probabiliteEchec_ la probabiliteEchec_ à modifier
	 */
	public void modifierProbabiliteEchec(final int probabiliteEchec)
	{
		probabiliteEchec_ = probabiliteEchec;
	}

	/**
	 * Retourne la vitesse du joueur virtuel.
	 * @return vitesse_
	 */
	public int obtenirVitesse()
	{
		return vitesse_;
	}

	/**
	 * @param vitesse_ la vitesse_ à modifier
	 */
	public void modifierVitesse(final int vitesse)
	{
		vitesse_ = vitesse;
	}

	@Override
	public PanneauAbstrait accepter(final VisiteurGenererPanneau visiteur)
	{
		return visiteur.visiter(this);

	}

	private int	probabiliteEchec_;
	private int	vitesse_;

}
