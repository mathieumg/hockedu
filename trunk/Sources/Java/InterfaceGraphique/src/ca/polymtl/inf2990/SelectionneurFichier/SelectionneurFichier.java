package ca.polymtl.inf2990.SelectionneurFichier;

import java.io.File;

import javax.swing.JFileChooser;
import javax.swing.JOptionPane;

import ca.polymtl.inf2990.Fenetre;

/**
 * Classe pour encapsuler le JFileChooser.
 * 
 * @author Charles Etienne Lalonde
 */
public class SelectionneurFichier extends JFileChooser
{
	private static final long	serialVersionUID	= 1L;

	/**
	 * Constructeur pour le selectionneur de fichier.
	 */
	public SelectionneurFichier()
	{
		super("./");
		setAcceptAllFileFilterUsed(false);
		addChoosableFileFilter(new FiltreFichiersXML());

	}

	@Override
	public void approveSelection()
	{
		if (getDialogType() == SAVE_DIALOG)
		{
			final File fichierSelectionne = getSelectedFile();
			if (fichierSelectionne != null && fichierSelectionne.exists())
			{
				final int valeurRetour = JOptionPane.showOptionDialog(Fenetre.obtenirInstance(), "Ce fichier existe déjà. Voulez-vous vraiment l'écraser?", "Écraser?", JOptionPane.INFORMATION_MESSAGE, JOptionPane.YES_NO_OPTION, null, new String[] { "Oui", "Non" }, "Non");
				if (valeurRetour == 1)
				{
					return;
				}
			}
			final String extensionFichierSelectionne = FiltreFichiersXML.obtenirExtension(fichierSelectionne);
			if (extensionFichierSelectionne == null || !extensionFichierSelectionne.equals("xml"))
			{
				setSelectedFile(new File(fichierSelectionne.getAbsolutePath() + ".xml"));
			}
		}
		super.approveSelection();
	}
}
