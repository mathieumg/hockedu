package ca.polymtl.inf2990.BarresOutils;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

import javax.swing.JFileChooser;

import ca.polymtl.inf2990.Fenetre;
import ca.polymtl.inf2990.SelectionneurFichier.SelectionneurFichier;

/**
 * �couteur li� au s�lectionneur de fichiers.
 * 
 * @author Charles �tienne Lalonde
 * 
 */
public class EvenementsSelectionneurFichier implements ActionListener
{
	private static File								dernierFichierSelectionne;
	private static EvenementsSelectionneurFichier	instance_;

	/**
	 * Retourne le dernier fichier s�lectionn�.
	 * 
	 * @return La valeur de dernierFichierSelectionne
	 */
	public static File obtenirDernierFichierSelectionne()
	{
		return dernierFichierSelectionne;
	}

	/**
	 * Efface la r�f�rence au dernier fichier s�lectionn�.
	 */
	public static void reinitialiserDernierFichierSelectionne()
	{
		dernierFichierSelectionne = null;
	}

	private final SelectionneurFichier	selectionneurDeFichier_;

	/**
	 * Constructeur de la classe.
	 */
	private EvenementsSelectionneurFichier()
	{
		selectionneurDeFichier_ = new SelectionneurFichier();
	}

	public static EvenementsSelectionneurFichier obtenirInstance()
	{
		if (instance_ == null)
		{
			instance_ = new EvenementsSelectionneurFichier();
		}
		return instance_;
	}

	/**
	 * Fonction pour g�rer les �v�nements d'actions relatifs au selectionneur de
	 * fichier.
	 * 
	 * @param e L'�venement d'action g�n�rer.
	 */
	@Override
	public void actionPerformed(final ActionEvent e)
	{
		int returnVal;
		final Commandes commande = Commandes.valueOf(e.getActionCommand());
		switch (commande)
		{
		// Si on cr�e un nouveau fichier.
			case EDITEUR_NOUVEAU:
				returnVal = selectionneurDeFichier_.showOpenDialog(Fenetre.obtenirInstance());
				if (returnVal == JFileChooser.APPROVE_OPTION)
				{
					dernierFichierSelectionne = selectionneurDeFichier_.getSelectedFile();
					chargerFichier(dernierFichierSelectionne.getAbsolutePath());
				}
				break;
			// Si on veut sauvegarder un fichier
			case EDITEUR_SAUVEGARDER:
				if (dernierFichierSelectionne == null)
				{
					returnVal = selectionneurDeFichier_.showSaveDialog(Fenetre.obtenirInstance());
					if (returnVal == JFileChooser.APPROVE_OPTION)
					{
						dernierFichierSelectionne = selectionneurDeFichier_.getSelectedFile();

					}
					else
					{
						return;
					}
				}
				sauvegarderFichier(dernierFichierSelectionne.getAbsolutePath());
				break;
			// Si on veut sauvegarder sous.
			case EDITEUR_SAUVEGARDER_SOUS:
				returnVal = selectionneurDeFichier_.showSaveDialog(Fenetre.obtenirInstance());
				if (returnVal == JFileChooser.APPROVE_OPTION)
				{
					dernierFichierSelectionne = selectionneurDeFichier_.getSelectedFile();
					sauvegarderFichier(dernierFichierSelectionne.getAbsolutePath());
				}
				break;
			// Si on veut charger.
			case EDITEUR_CHARGER:
				returnVal = selectionneurDeFichier_.showOpenDialog(Fenetre.obtenirInstance());
				if (returnVal == JFileChooser.APPROVE_OPTION)
				{
					dernierFichierSelectionne = selectionneurDeFichier_.getSelectedFile();
					chargerFichier(dernierFichierSelectionne.getAbsolutePath());
				}
				break;
		}

	}

	/**
	 * Charge un fichier XML selon le chemin pass� en param�tre.<br>
	 * M�thode native du JNI. Redirige l'appel vers C++.
	 * 
	 * @param nomFichier Le chemin du fichier.
	 */
	private native void chargerFichier(String nomFichier);

	/**
	 * Sauvegarde un fichier XML selon le chemin pass� en param�tre.<br>
	 * M�thode native du JNI. Redirige l'appel vers C++.
	 * 
	 * @param nomFichier Le chemin du fichier.
	 */
	private native void sauvegarderFichier(String nomFichier);
}
