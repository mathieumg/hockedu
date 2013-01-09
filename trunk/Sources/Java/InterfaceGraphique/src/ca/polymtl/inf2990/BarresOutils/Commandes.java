package ca.polymtl.inf2990.BarresOutils;

/**
 * Énumération de toutes les commandes pouvant êtres exécutées par nos
 * boutons/menus etc...
 * 
 * @author Charles Étienne Lalonde
 * 
 */
public enum Commandes
{
	CAMERA(0), EDITEUR_CHARGER(13), EDITEUR_DEPLACER(6), EDITEUR_ECHELLE(4),
	EDITEUR_NOUVEAU(11), EDITEUR_ROTATION(3), EDITEUR_SAUVEGARDER(12),
	EDITEUR_SAUVEGARDER_SOUS(15), EDITEUR_SELECTION(5),
	INSERER_ACCELERATEUR(9), INSERER_BUT(10), INSERER_MURET(7),
	INSERER_PORTAIL(8), QUITTER(14), SUPPRIMER(16), ZOOM_ELASTIQUE(1),
	ZOOM_PROPORTIONNEL(2), INSERER_MAILLET(17), INSERER_RONDELLE(18),
	DUPLIQUER(19), ALLER_MODE_EDITION(20), ALLER_MODE_JEU(21),
	MODIFIER_TOUCHE(22), REINITIALISER_PARTIE(23), PAUSE_JEU(24), REPLAY(25),
	CAMERA_FIXE(26), CAMERA_ORBITE(27), CAMERA_LIBRE(28), ORBIT(29),
	CAMERA_SPLIT(30);

	final int	jobid;

	/**
	 * Construit une commande.
	 * 
	 * @param id Id de la commande.
	 */
	private Commandes(final int id)
	{
		jobid = id;
	}

	/**
	 * retourne l'identifiant de la commande.
	 * 
	 * @return La valeur d'id.
	 */
	public int obtenirId()
	{
		return jobid;
	}

	public boolean activerQuandObjetSelectionne()
	{
		return (this == EDITEUR_ECHELLE || this == EDITEUR_ROTATION || this == DUPLIQUER || this == SUPPRIMER || this == EDITEUR_DEPLACER);
	}

	public boolean desactiverQuandObjetSelectionne()
	{
		return false;
	}

	/**
	 * Dis si on est en mode camera ou non.
	 * 
	 * @param etatCameraActuel La commande de camera
	 * @return un booleen
	 */
	public boolean activerPourEtatCamera(final Commandes etatCameraActuel)
	{

		if (etatCameraActuel == CAMERA_FIXE)
		{
			return this != CAMERA && this != ORBIT;
		}

		return true;
	}

	public boolean estCommandeParDefaut()
	{
		return this == ORBIT;
	}

	public boolean desactiverSiPointButSelectionne()
	{
		return this == SUPPRIMER || this == DUPLIQUER || this == EDITEUR_ROTATION;
	}

	public boolean desactiverSiPointPasButSelectionne()
	{
		return this == SUPPRIMER || this == DUPLIQUER || this == EDITEUR_ROTATION || this == EDITEUR_ECHELLE;
	}
}
