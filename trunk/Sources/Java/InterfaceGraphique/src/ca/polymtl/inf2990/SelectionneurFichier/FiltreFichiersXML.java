package ca.polymtl.inf2990.SelectionneurFichier;

import java.io.File;

import javax.swing.filechooser.FileFilter;

/**
 * Filtre utile pour seulement afficher/authoriser les fichiers de type XML dans le selectionneur de fichiers.
 * @author Charles Étienne Lalonde
 *
 */
public class FiltreFichiersXML extends FileFilter
{

	/**
	 *  Détermine si le fichier doit s'afficher ou non selon son extension.
	 *  @param f Le fichier à traiter.
	 *  @return Si le fichier a été accepté ou non.
	 */
	public boolean accept(File f)
	{
		if (f.isDirectory())
		{
			return true;
		}

		String extension = obtenirExtension(f);

		// Tests sur l'extension (non nulle, et xml)
		if (extension != null)
		{
			if (extension.equals("xml"))
			{
				return true;
			}
		}

		return false;
	}

	/**
	 *  Retourne la description du filtre.
	 *  @return La description du filtre.
	 */
	public String getDescription()
	{
		return "Fichiers XML (.xml)";
	}

	/**
	 * Obtient l'extension d'un fichier
	 * 
	 * @param f
	 *            Le fichier duquel on veut l'extension
	 * @return L'extension du fichier
	 */
	public static String obtenirExtension(File f)
	{
		String ext = null;
		String s = f.getName();
		int i = s.lastIndexOf('.');

		if (i > 0 && i < s.length() - 1)
		{
			ext = s.substring(i + 1).toLowerCase();
		}
		return ext;
	}

}
