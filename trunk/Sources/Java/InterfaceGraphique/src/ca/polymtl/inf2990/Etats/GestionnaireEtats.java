/**
 * 
 */
package ca.polymtl.inf2990.Etats;

import java.util.Stack;

import ca.polymtl.inf2990.Fenetre;

/**
 * Gestionnaire qui se charge des états de l'interface.
 * 
 * @author Charles Étienne Lalonde
 * 
 */
public class GestionnaireEtats {
	private final Stack<EtatAbstrait> pileEtats_;
	private static GestionnaireEtats instance_;

	public native void genererTerrainParDefaut();

	public native boolean validerTable();

	public native boolean changerEtat(int etat);

	private GestionnaireEtats() {
		pileEtats_ = new Stack<EtatAbstrait>();
	}

	/**
	 * Retourne une instance du gestionnaire (Singleton)
	 * 
	 * @return une instance du gestionnaire
	 */
	public static GestionnaireEtats obtenirInstance() {
		if (instance_ == null) {
			instance_ = new GestionnaireEtats();
		}
		return instance_;
	}

	/**
	 * Retourne l'état courant.
	 * 
	 * @return L'état présentement affiché.
	 */
	public EtatAbstrait obtenirEtat() {
		return pileEtats_.peek();
	}

	/**
	 * Retourne à l'état précédent. Utile pour implanter des boutons "back".
	 */
	public void allerEtatPrecedent() {

		if (pileEtats_.size() > 1) {
			pileEtats_.pop().terminer();
			pileEtats_.peek().reveiller();
			Fenetre.obtenirInstance().changerEtat(
					pileEtats_.peek().obtenirNom());
		}
	}

	/**
	 * Place l'interface dans un nouvel état et affiche le panneau associé.
	 * 
	 * @param nouvelEtat L'état dans lequel aller.
	 */
	public void changerEtat(final EtatAbstrait nouvelEtat) {
		if (nouvelEtat.obtenirNom() == Etats.MODE_OPTION_PARTIE_RAPIDE) {
			if (pileEtats_.peek().obtenirNom() == Etats.MODE_EDITION) {
				if (!validerTable()) {
					return;
				}
			} else {
				genererTerrainParDefaut();
				// A conserver car la methode applique des variables importantes
				// pour le jeu
				if (!validerTable()) {
					return;
				}
			}
		}

		if (pileEtats_.isEmpty()) {
			pileEtats_.push(nouvelEtat);
			Fenetre.obtenirInstance().changerEtat(nouvelEtat.obtenirNom());
			return;
		}

		if (nouvelEtat == pileEtats_.peek()) {
			return;
		}

		if (nouvelEtat.obtenirNom() == Etats.MODE_PARTIE_RAPIDE
				|| nouvelEtat.obtenirNom() == Etats.MODE_EDITION
				|| nouvelEtat.obtenirNom() == Etats.MODE_TOURNOI
				|| nouvelEtat.obtenirNom() == Etats.MENU_PRINCIPAL) {
			while (!pileEtats_.isEmpty()) {
				pileEtats_.pop().terminer();
			}
		}

		if (!pileEtats_.isEmpty()) {
			if (pileEtats_.contains(nouvelEtat)) {
				while (!(pileEtats_.peek() == nouvelEtat)) {
					pileEtats_.pop().terminer();
				}
				pileEtats_.peek().reveiller();
				Fenetre.obtenirInstance().changerEtat(nouvelEtat.obtenirNom());
				if (nouvelEtat.obtenirNom() == Etats.MODE_PARTIE_RAPIDE) {
					((EtatPartieRapide) nouvelEtat).obtenirJoueur()
							.envoyerJNI();
				}
				if (!changerEtat(nouvelEtat.obtenirNom().ordinal())) {
					allerEtatPrecedent();
				}
				nouvelEtat.reveiller();
				return;
			}
			pileEtats_.peek().mettreAuRepos();
		}

		pileEtats_.push(nouvelEtat);
		Fenetre.obtenirInstance().changerEtat(nouvelEtat.obtenirNom());
		if (nouvelEtat.obtenirNom() == Etats.MODE_PARTIE_RAPIDE) {
			((EtatPartieRapide) nouvelEtat).obtenirJoueur().envoyerJNI();
		}
		if (!changerEtat(nouvelEtat.obtenirNom().ordinal())) {
			allerEtatPrecedent();
		}
		nouvelEtat.reveiller();
	}
}
