package ca.polymtl.inf2990.Jeu;

import java.util.ArrayList;

import javax.swing.JTextField;

/**
 * Fonctions JNI utile pour la radio.
 * @author Gabriel Couvrette
 *
 */
public class OperationsRadioJNI {		
	//méthode "Play" qui demande à C++ de jouer la chanson.
	public static native void jouerMusiqueRadio();
	
	//méthode "Stop" qui demande à c++ d'arrêter de lire la chanson et de se replacer au début.
	public static native void arreterMusiqueRadio();
	
	//méthode "pause" qui met la chanson courante en pause.
	public static native void pauserMusiqueRadio();
	
	//méthode qui change de chanson
	public static native void avancerChansonRadio();
	public static native void reculerChansonRadio();
	
	//méthode qui change le volume (entre 0 et 100%)
	public static native void placerVolumeRadio(int pourcentageVolume);
	
	//retourne la liste des canaux
	public static native ArrayList<Object> obtenirCanaux();
	
	//retourne les informations sur le canal selon le nom.
	public static native CanalRadio obtenirCanal(String nomCanal);
	
	//supprime un canal au complet.
	public static native void supprimerCanal(String nomCanal);

	//ajoute/override le canal au fichier xml des canaux.
	public static native void genererCanal(CanalRadio canal_);

	//dit a FMOD de jouer le canal donne en parametres.
	public static native void chargerCanal(String selectedItem);

	//retourne le canal qui joue en ce moment.
	public static native String obtenirCanalCourant();
}
