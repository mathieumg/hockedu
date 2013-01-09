package ca.polymtl.inf2990.Jeu;


import java.awt.Dimension;
import java.awt.Point;
import java.util.ArrayList;

import ca.polymtl.inf2990.Panneaux.PanneauAbstrait;
import ca.polymtl.inf2990.Panneaux.VisiteurGenererPanneau;

/**
 * Classe contenant les informations utiles d'un tournoi.
 * @author Vincent Lemire / Michael Ferris / Gabriel Couvrette
 *
 */
public class Tournoi implements ElementVisitable
{

	private ArrayList<Object>	joueurs_;
	private String				nom_;
	private String				terrain_;
	private ArrayList<Object> vainqueurs_; //la liste de tous les vainqueurs du tournoi.
	private ArrayList<Object> pointsTable_; //la liste des points de la table.
	


	/**
	 * Donne à l'objet la liste des points de la table sur laquelle on va jouer.
	 * @param listePoints
	 */
	public void modifierPointsTable(ArrayList<Object> listePoints)
	{
		pointsTable_=listePoints;
	}
	
	/**
	 * Retourne la liste des points de la table.
	 * @return Les points de la table.
	 */
	public ArrayList<Object> obtenirPointsTable()
	{
		return pointsTable_;
	}
	
	/**
	 * Retourne la liste de tous les joueurs ayant deja remporte ce tournoi.
	 * @return La liste du nom de tous les joueurs ayant gagne ce tournoi (String).
	 */
	public ArrayList<Object> obtenirVainqueurs()
	{
		return vainqueurs_;
	}
	
	/**
	 * Associe une liste de gagnants à ce tournoi.
	 * @param gagnants Les anciens gagnants.
	 */
	public void modifierVainqueurs(ArrayList<Object> gagnants)
	{
		vainqueurs_=gagnants;
	}
	
	/**
	 * Mémorise le chemin du terrain de jeu à utiliser pour le tournoi.
	 * @param terrain Le chemin (Path) menant vers le tournoi.
	 */
	public void modifierTerrain(final String terrain)
	{
		terrain_ = terrain;
	}

	/**
	 * Retourne le chemin vers le terrain de jeu.
	 * @return Le chemin où trouver le fichier du terrain.
	 */
	public String obtenirTerrain()
	{
		return terrain_;
	}

	/**
	 * retourne le nom du tournoi.
	 * @return
	 */
	public String obtenirNom()
	{
		return nom_;
	}

	/**
	 * Donne un nom au tournoi.
	 * @param nom Le nom du tournoi.
	 */
	public void setNom(String nom)
	{
		nom_ = nom;
	}

	/**
	 * Utile pour le visiteur de population de panneaux.
	 */
	@Override
	public PanneauAbstrait accepter(final VisiteurGenererPanneau visiteur)
	{
		return visiteur.visiter(this);
	}

	/**
	 * Charge la liste des joueurs participant au tournoi.
	 * @param joueurs Les joueurs à placer dans le tournoi.
	 */
	public void chargerJoueurs(final ArrayList<Object> joueurs)
	{
		joueurs_ = joueurs;
	}

	/**
	 * Retourne la liste des joueurs participant au tournoi.
	 * @return Les joueurs du tournoi.
	 */
	public ArrayList<Object> obtenirListeDesJoueurs()
	{
		return joueurs_;
	}

	
	/**
	 * Retourne le nombre de joueurs initialement dans le tournoi.
	 * @return Le nombre de joueurs dans le tournoi.
	 */
	public int obtenirNombreDeJoueurs()
	{
		return joueurs_.size();
	}
}
