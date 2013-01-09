package ca.polymtl.inf2990.Jeu;

/**
 * Classe abstraite qui représente un joueur dans le mode jeu
 * 
 * @author Michael Ferris
 * @date 17-02-2012
 */

public abstract class JoueurAbstrait
{
	/**
	 * Les deux types de joueurs possibles.
	 * @author Michael Ferris
	 *
	 */
	public enum TypeJoueur
	{
		HUMAIN, VIRTUEL
	};

	/**
	 * Constructeur pas paramètres du JoueurAbstrait
	 * 
	 * @param nom Nom du joueur
	 */
	public JoueurAbstrait(final String nom, final TypeJoueur type)
	{
		nom_ = nom;
		type_ = type;
	}

	/**
	 * Cette méthode demande au C++ de sélectionner l'objet à la position
	 * courante de la souris et affiche le menu contextuel ensuite.
	 * 
	 * @return Le nom du joueur
	 */
	public String obtenirNom()
	{
		return nom_;
	}

	// Nom du joueur
	private final String		nom_;

	private final TypeJoueur	type_;

	/**
	 * Retourne le type du joueur.
	 * @return Le type du joueur.
	 */
	public TypeJoueur obtenirType()
	{
		return type_;
	}

	/**
	 * Retourne le type comme un int. Utile pour l'interface JNI, car C++ aime mieux recevoir un int qu'une valeur d'enum.
	 * @return L'index du type (0=HUMAIN, 1=VIRTUEL)
	 */
	public int obtenirTypeNumeral()
	{
		return type_.ordinal();
	}

	/**
	 * Supprime le joueur. (En fait, ne fait rien du tout pour l'instant)
	 */
	public void supprimerJoueur()
	{

	}

	/**
	 * Retourne le nom du joueur sous forme de string.
	 */
	@Override
	public String toString()
	{
		return obtenirNom();
	}

	/**
	 * Méthode native qui envoie les infos du joueur à JNI.
	 */
	public native void envoyerJNI();
}
