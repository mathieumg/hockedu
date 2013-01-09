package ca.polymtl.inf2990.Jeu;

import java.io.File;
import java.io.FileFilter;
import java.util.ArrayList;


/**
 * Focntions JNI utile pour les tournois.
 * @author Gabriel Couvrette
 *
 */
public class OperationTournoiJNI {
	
	/**
	 * Retourne la liste des tournois existants.
	 * @return La liste des tournois existants.
	 */
 public static ArrayList<String> obtenirListeTournois()
 {
	 ArrayList<String> nomsDesTournois=new ArrayList<String>();
	 File racine=new File("tournoi/");
	 File[] listeFiles=racine.listFiles(new FileFilter() {
		
		@Override
		public boolean accept(File pathname) {
			return pathname.getName().endsWith(".xml");
		}
	});
	 
	 for(int i=0;i<listeFiles.length;++i)
	 {
		 nomsDesTournois.add(listeFiles[i].getName().substring(0, listeFiles[i].getName().length()-4));
	 }
	 
	 return nomsDesTournois;
 }
 
 
 /**
  * retourne un objet de type Tournoi contenant les infos sur le tournoi, selon le nom passé en paramètre.
  * @param nomTournoi Le nom du tournoi.
  * @return Les informations sur ce tournoi.
  */
 public static native Object obtenirTournoi(String nomTournoi);
 
 /**
  * Supprime un tournoi.
  * @param nomTournoi Le nom du tournoi à supprimer.
  */
 public static native void supprimerTournoi(String nomTournoi);
  
}
