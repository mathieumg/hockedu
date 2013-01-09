package ca.polymtl.inf2990;

import java.awt.Image;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import javax.imageio.ImageIO;
import javax.swing.ImageIcon;

/**
 * Classe qui s'ocupe de charger toutes les images utiles aux interfaces.
 * Empêche la duplication des images pour des composants répétés souvent dans l'interface.
 * Centralise toutes les ressources utiles à l'interface.
 * 
 * @author Gabriel Couvrette
 * 
 */
public class GestionnaireImages
{
	/**
	 * Le nom des images supportées par ce gestionnaire.
	 * @author Gabriel Couvrette
	 *
	 */
	public enum NomImage
	{
		BOUTON_NORMAL, BOUTON_ENFONCE, BOUTON_SURVOLE, SUPPRIMER_NORMAL,
		SUPPRIMER_SURVOLE, SUPPRIMER_ENFONCE, RETOUR_NORMAL, RETOUR_SURVOLE,
		RETOUR_ENFONCE, MENU_PRINCIPAL, BANDES, SPLASH
	};

	final static String					dossierRoot	= "/ca/polymtl/inf2990/icones/";
	final static String					paths[]		= { "bouton_normal.png",// BOUTON_NORMAL
													"bouton_enfonce.png",// BOUTON_ENFONCE
													"bouton_survole.png",// BOUTON_SURVOLE
													"supprimer_normal.png",// SUPPRIMER_NORMAL
													"supprimer_survol.png",// SUPPRIMER_SURVOLE
													"supprimer_presse.png",// SUPPRIMER_ENFONCE
													"retour_normal.png",// RETOUR_NORMAL
													"retour_survol.png",// RETOUR_SURVOLE
													"retour_presse.png",// RETOUR_ENFONCE
													"menu.png", // MENU_PRINCIPAL
													"tableBandes.jpg", // BANDES
													"splash.jpg" // SPLASH
													};

	// ////////////////////////////SINGLETON//////////////////////////////
	private static GestionnaireImages	instance_;

	/**
	 * Retourne une instance du gestionnaire de ressources. (Singleton)
	 * @return L'instance de GestionnaireImages
	 */
	public static GestionnaireImages obtenirInstance()
	{
		if (instance_ == null)
		{
			instance_ = new GestionnaireImages();
		}
		return instance_;
	}

	// ///////////////////////////////////////////////////////////////////

	Map<NomImage, Image>	images_;

	/**
	 * Constructeur privé du Gestionnaire. (Singleton)
	 * Initialise toutes les ressources nécéssaires et charge les images dans la table d'images.
	 */
	private GestionnaireImages()
	{
		images_ = new HashMap<NomImage, Image>(NomImage.values().length);

		for (NomImage imagePath : NomImage.values())
		{
			try
			{
				System.out.println();
				Image img = ImageIO.read(getClass().getResource(dossierRoot + paths[imagePath.ordinal()]));
				images_.put(imagePath, img);
			}
			catch (Exception ex)
			{
				ex.printStackTrace();
				images_.put(imagePath, null);
			}
		}
	}

	/**
	 * Retourne une image selon le nom de l'image venant de l'enum.
	 * 
	 * @param image le nom de l'image selon les valeurs de l'enum.
	 * @return L'image a retourner
	 */
	public Image obtenirImage(final NomImage image)
	{
		return images_.get(image);
	}

	/**
	 * Retourne une image de la banque de ressource, mais redimensionnée à une taille quelconque.
	 * @param nom Le nom de l'image à charger
	 * @param largeur La largeur de l'image à retourner
	 * @param hauteur La hauteur de l'image à retourner
	 * @return L'image ayant les dimansions demandées.
	 */
	public Image obtenirImageSelonTaille(final NomImage nom, final int largeur, final int hauteur)
	{
		return images_.get(nom).getScaledInstance(largeur, hauteur, Image.SCALE_SMOOTH);
	}

	/**
	 * Retourne une imageIcon selon le nom de l'image venant de l'enum.
	 * 
	 * @param image le nom de l'image selon les valeurs de l'enum.
	 * @return L'imageIcon a retourner
	 */
	public ImageIcon obtenirImageIcon(final NomImage image)
	{
		return new ImageIcon(images_.get(image));
	}

}
