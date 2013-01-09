package ca.polymtl.inf2990.Etats;

import ca.polymtl.inf2990.Fenetre;
import ca.polymtl.inf2990.Lancement;
import ca.polymtl.inf2990.Jeu.JoueurAbstrait;
import ca.polymtl.inf2990.Jeu.OperationsRadioJNI;
import ca.polymtl.inf2990.Panneaux.PanneauAbstrait;
import ca.polymtl.inf2990.Panneaux.PanneauCanevas;

/**
 * État lorsque l'application est en mode partie rapide.
 * 
 * @author Charles Étienne Lalonde
 * 
 */
public class EtatPartieRapide extends EtatAbstrait {
	private JoueurAbstrait joueur_;

	@Override
	public void initialiser() {

	}

	@Override
	public void terminer() {
		Fenetre.obtenirInstance().terminerRadio();
		Lancement.desactiverMinuterie();
	}

	@Override
	public void mettreAuRepos() {
		Lancement.desactiverMinuterie();
	}

	@Override
	public Etats obtenirNom() {
		return Etats.MODE_PARTIE_RAPIDE;
	}

	@Override
	public void reveiller() {
		Lancement.activerMinuterie();
		OperationsRadioJNI.arreterMusiqueRadio();
		Fenetre.obtenirInstance().afficherRadio();
	}

	@Override
	protected PanneauAbstrait creerPanneau() {
		return PanneauCanevas.obtenirInstance();
	}

	@Override
	public PanneauAbstrait obtenirPanneau() {
		return PanneauCanevas.obtenirInstance();
	}

	public void changerJoueur(final JoueurAbstrait joueur) {
		joueur_ = joueur;
	}

	public JoueurAbstrait obtenirJoueur() {
		return joueur_;
	}
}
