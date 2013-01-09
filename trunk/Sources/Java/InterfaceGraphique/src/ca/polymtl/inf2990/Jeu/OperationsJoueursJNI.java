package ca.polymtl.inf2990.Jeu;

import java.awt.event.KeyEvent;
import java.util.ArrayList;

/**
 * Classe contenant les appels JNI utiles pour l'interface.
 * @author Gabriel Couvrette
 *
 */
public class OperationsJoueursJNI
{
	public static native JoueurAbstrait obtenirJoueur(String nom);
	public static native void ajouterJoueur(JoueurAbstrait joueur);
	public static native ArrayList<Object> obtenirJoueurs();
	public static native void supprimerJoueur(String nom);
	public static native void modifierTouches(int haut, int bas, int gauche, int droite);
	public static native int[] obtenirTouches();
	public static native void creerNouveauTournoi(Tournoi tournoi);
}
