package ca.polymtl.inf2990.Jeu;

/**
 * Classe qui représente un objet modificateur envoyer au modele pour charger
 * les objets avec les bons attributs
 * 
 * @author Samuel Ledoux
 * @date 17-02-2012
 */

public class ModificateurProprieteNoeud
{
	/**
	 * Constructeur par défaut
	 * 
	 */
	public ModificateurProprieteNoeud()
	{
		modifierPositionX(0.0);
		modifierPositionY(0.0);
		coefRebondBandes_ = new double[8];
		for (int i = 0; i < 8; ++i)
		{
			coefRebondBandes_[i] = 0;
		}
	}

	/**
	 * Constructeur par parametres.
	 * 
	 * @param coefFriction_
	 * @param coefRebond_
	 * @param bonusAccel_
	 * @param position_
	 * @param echelle_
	 * @param rotation_
	 * @param proprietesModifiees_
	 */
	public ModificateurProprieteNoeud(final double positionX_, final double positionY_, final double coefFriction_, final double coefRebond_, final double bonusAccel_, final double echelle_, final int rotation_, final double zoneEditionX, final double zoneEditionY, final double[] coefsRebondBandes)
	{
		this.coefFriction_ = coefFriction_;
		this.coefRebond_ = coefRebond_;
		this.bonusAccel_ = bonusAccel_;
		this.positionX_ = positionX_;
		this.positionY_ = positionY_;
		this.echelle_ = echelle_;
		this.rotation_ = rotation_;
		zoneEditionX_ = zoneEditionX;
		zoneEditionY_ = zoneEditionY;
		coefRebondBandes_ = coefsRebondBandes;
	}

	/**
	 * Accesseur du coefficient de friction
	 * 
	 * @return coefFriction_
	 */
	public double obtenirCoefFriction()
	{
		return coefFriction_;
	}

	/**
	 * Mutateur de coefFriction_
	 * 
	 * @param coefFriction_ : le coefficient a assigner
	 */
	public void modifierCoefFriction(final double coefFriction)
	{
		coefFriction_ = coefFriction;
	}

	/**
	 * Accesseur du coefficient de rebond
	 * 
	 * @return coefRebond_
	 */
	public double obtenirCoefRebond()
	{
		return coefRebond_;
	}

	/**
	 * Mutateur du coefficient de rebond
	 * 
	 * @param coefRebond_ : le coefficient de rebond a assigner
	 */
	public void modifierCoefRebond(final double coefRebond)
	{
		coefRebond_ = coefRebond;
	}

	/**
	 * Accesseur de bonus d'acceleration
	 * 
	 * @return the bonusAccel_
	 */
	public double obtenirBonusAccel()
	{
		return bonusAccel_;
	}

	/**
	 * Mutateur de bonus d'acceleration
	 * 
	 * @param bonusAccel_ : bonus a assigner
	 */
	public void modifierBonusAccel(final double bonusAccel)
	{
		bonusAccel_ = bonusAccel;
	}

	/**
	 * Accesseur de position
	 * 
	 * @return position_
	 */
	public double obtenirPositionX()
	{
		return positionX_;
	}

	public double obtenirPositionY()
	{
		return positionY_;
	}

	/**
	 * Mutateur de position
	 * 
	 * @param positionX : la position a assigner
	 */
	public void modifierPositionX(final double positionX)
	{
		positionX_ = positionX;
	}

	/**
	 * Mutateur de position
	 * 
	 * @param positionY : la position a assigner
	 */
	public void modifierPositionY(final double positionY)
	{
		positionY_ = positionY;
	}

	/**
	 * Accesseur de echelle
	 * 
	 * @return echelle_
	 */
	public double obtenirEchelle()
	{
		return echelle_;
	}

	/**
	 * Mutateur de echelle
	 * 
	 * @param echelle_ : l'echelle a assigner
	 */
	public void modifierEchelle(final double echelle)
	{
		echelle_ = echelle;
	}

	/**
	 * Accesseur de rotation
	 * 
	 * @return rotation_
	 */
	public int obtenirRotation()
	{
		return rotation_;
	}

	/**
	 * Mutateur de rotation
	 * 
	 * @param rotation_ : la rotation a assigner
	 */
	public void modifierRotation(final int rotation)
	{
		rotation_ = rotation;
	}

	/**
	 * Mutateur de largeurZoneEdition
	 * 
	 * @param largeur: largeur a assigner
	 */
	public void modifierZoneEditionX(final double valeur)
	{
		zoneEditionX_ = valeur;
	}

	/**
	 * Mutateur de longueurZoneEdition
	 * 
	 * @param longueur: longueur a modifier
	 */
	public void modifierZoneEditionY(final double valeur)
	{
		zoneEditionY_ = valeur;
	}

	/**
	 * Accesseur de zoneEditionX
	 * 
	 * @return zoneEditionX_
	 */
	public double obtenirZoneEditionX()
	{
		return zoneEditionX_;
	}

	/**
	 * Accesseur de zoneEditionY
	 * 
	 * @return zoneEditionY_
	 */
	public double obtenirZoneEditionY()
	{
		return zoneEditionY_;
	}

	/**
	 * Accesseur des coefRebond pour les bandes
	 * 
	 * @return coefRebondBande[laquelle]
	 */
	public double obtenirCoefRebondBandes(final int laquelle)
	{
		return coefRebondBandes_[laquelle];
	}

	/**
	 * Mutateur des coef rebond
	 * 
	 * @param laquelle : bande a modifier
	 * @param valeur : valeur a assigner
	 */
	public void modifierCoefRebondBandes(final int laquelle, final double valeur)
	{
		coefRebondBandes_[laquelle] = valeur;
	}

	public native void envoyerModificateurPropriete();

	public final static int	NOMBRE_BANDES	= 8;

	private double			coefFriction_;

	private double			coefRebond_;

	private double			bonusAccel_;

	private double			positionX_;

	private double			positionY_;

	private double			echelle_;

	private int				rotation_;

	private double			zoneEditionX_;

	private double			zoneEditionY_;

	private final double[]	coefRebondBandes_;

	private boolean[]		proprietesModifiees_;

}
