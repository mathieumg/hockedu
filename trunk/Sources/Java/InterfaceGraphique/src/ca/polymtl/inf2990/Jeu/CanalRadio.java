package ca.polymtl.inf2990.Jeu;

import java.util.ArrayList;
import java.util.Map;

import javax.swing.JPanel;

import ca.polymtl.inf2990.Panneaux.PanneauAbstrait;
import ca.polymtl.inf2990.Panneaux.VisiteurGenererPanneau;

/**
 * Repr�sente un canal de radio.
 * @author Gabriel Couvrette
 *
 */
public class CanalRadio
{
	private String nomDuCanal_;
	//ArrayList<String> : liste des noms de chansons.
	private ArrayList<Object> listeDesChansons_;
	
	/**
	 * Cr�e un canal de radio.
	 */
	public CanalRadio()
	{
		listeDesChansons_=new ArrayList<Object>();
	}
	
	/**
	 * Assigne un nom au canal.
	 * @param nom Le nom � assigner.
	 */
	public void assignerNomDuCanal(String nom)
	{
		nomDuCanal_=nom;
	}
	
	/**
	 * Assigne une liste de chansons au canal.
	 * @param liste La liste de chanson contenue dans le canal.
	 */
	public void assignerListeDesChansons(ArrayList<Object> liste)
	{
		listeDesChansons_=liste;
	}
	
	/**
	 * Retorune le nom du canal.
	 * @return Le nom du canal.
	 */
	public String obtenirNomDuCanal()
	{
		return nomDuCanal_;
	}
	
	/**
	 * Retourne la liste des chansons.
	 * @return La liste des chansons
	 */
	public ArrayList<Object> obtenirListeDesChansons()
	{
		return listeDesChansons_;
	}

	/**
	 * Ajoute une chanson � la liste.
	 * @param path Le chemin vers la chanson sur le disque.
	 */
	public void ajouterChanson(String path)
	{
		listeDesChansons_.add(path);
	}
	
	/**
	 * Retire une chanson de la liste.
	 * @param path Le chemin vers la chanson � supprimer de la liste.
	 */
	public void retirerChanson(String path)
	{
		for(int i=0;i<listeDesChansons_.size();++i)
		{
			if(listeDesChansons_.get(i).equals(path.replace("\\", "/")) || listeDesChansons_.get(i).equals(path))
			{
				listeDesChansons_.remove(i);
				return;
			}
		}
	}
}
