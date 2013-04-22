package ca.polymtl.inf2990;

import java.awt.EventQueue;
import java.lang.reflect.InvocationTargetException;
import java.util.Timer;
import java.util.TimerTask;

import javax.swing.SwingUtilities;

/**
 * Cette classe crée une fenêtre Java ainsi qu'un contexte OpenGL dans lequel la
 * librairie C++ pourra dessiner.
 * 
 * Il ne s'agit que d'un exemple des différentes fonctionnalités qu'il est
 * possible d'implanter en Java, notamment la création d'une fenêtre, d'un
 * contexte OpenGL, d'une barre de menus et d'une barre d'outils, ainsi que la
 * gestionnaire de certains événements.
 * 
 * CETTE CLASSE NE CONSTITUE PAS UN EXEMPLE DE BONNE PROGRAMMATION. Elle est
 * monolithique et vise à être séparée en plusieurs classes, éventuellement dans
 * des paquetages différents, lorsque les véritables fonctionnalités seront
 * implantées.
 * 
 * @author Martin Bisson
 * @date 2007-08-15
 */
public class Lancement implements Runnable {
	// Le temps souhaité entre 2 affichages, en millisecondes.
	static private final int TEMPS_INTER_AFFICHAGE = 20;
	private static Timer minuterie = null;

	/**
	 * Initialisation statique de la classe qui charge la librairie C++
	 * contenant l'implantation des fonctions natives.
	 */
	static {
		System.loadLibrary("INF2990");
	}

	// ///////////////////////
	// //
	// FONCTIONS NATIVES //
	// //
	// ///////////////////////

	/**
	 * Fonction qui anime le jeu d'un certain intervalle de temps.
	 * 
	 * Elle vise à être appelée de nombreuses fois par seconde. Elle effectue
	 * les différents calculs de physique et effectue un affichage.
	 * 
	 * @param temps
	 *            L'intervalle de temps à utiliser pour les différents calculs.
	 *            On vise à faire correspondre cet invervalle de temps au temps
	 *            entre deux appels à la fonction.
	 */
	public static native void fctC_animer(float temps);

	/**
	 * Fonction qui exécute les jeux de tests unitaires C++.
	 */
	public static native int fctC_executerTests();

	/**
	 * Fonction principale de l'application qui constitue en fait son point
	 * d'entrée. Elle vérifie en premier si l'usager souhaite exécuter les test
	 * unitaires (si l'argument est testsC++). Dans l'affirmative, elle invoque
	 * l'appel des tests unitaires C++ via JNI et retourne le code d'erreur
	 * renvoyé. Sinon, la fonction d'entrée procède au traitement par défaut :
	 * elle se contente de créer un objet <code>Lancement</code> et de
	 * l'invoquer dans le thread AWT.
	 * 
	 * @param args
	 *            Les paramètres passées à l'application par la ligne de
	 *            commande. Si le premier argument (args[0]) contient la chaîne
	 *            "testsC++", les tests unitaires C++ sont invoqués et
	 *            l'application quitte (retourne) immédiatement.
	 */
	public static void main(final String args[]) {
		// Vérifier si nous exécuterons les tests unitaires...

		if (args.length >= 1 && args[0].equals("testsC++")) { // Exécution des
																// tests C++
			final int resultat = fctC_executerTests();
			if (resultat == 0) {
				System.exit(resultat);
			}
		} // Pas d'exécution des tests unitaires, crééons la fenêtre et //
			// affichons-là
		else {
			// Création de l'unique instance du contrôleur
			final Lancement instance = new Lancement();
			// Création et affichage de la fenêtre principale
			EventQueue.invokeLater(instance);
		}
	}

	/**
	 * Cette fonction active la minuterie qui appelle périodiquement une
	 * fonction afin de rafraîchir constamment la fenêtre.
	 */
	public static void activerMinuterie() {
		desactiverMinuterie();

		minuterie = new Timer();
		minuterie.scheduleAtFixedRate(new TimerTask() {
			@Override
			public void run() {
				try {
					SwingUtilities.invokeAndWait(new Runnable() {

						@Override
						public void run() {
							final float temps = TEMPS_INTER_AFFICHAGE / 1000.0f;
							fctC_animer(temps);
							Fenetre.obtenirInstance().dessinerOpenGL();

						}
					});
				} catch (final InvocationTargetException ex) {
					ex.printStackTrace();
				} catch (final InterruptedException ex) {
					ex.printStackTrace();
				}
			}
		}, 0, TEMPS_INTER_AFFICHAGE);
	}

	public static void desactiverMinuterie() {
		if (minuterie != null) {
			minuterie.cancel();
		}
		minuterie = null;
	}

	/**
	 * Fonction qui crée l'interface graphique et le prépare à recevoir des
	 * événements. Cette fonction est appelée par le thread AWT afin que les
	 * appels au GUI soit tous faits dans le même thread.
	 */
	@Override
	public void run() {
		// Chargement de la librairie pour le look & feel.
		Fenetre.chargerTinyLookAndFeel();

		// Appel de la fenetre pour l'afficher
		Fenetre.obtenirInstance();
		Fenetre.rafraichirFenetre();
		Fenetre.obtenirInstance().setVisible(true);

		// Activation de la minuterie appelée plusieurs fois par seconde pour
		// réafficher la fenêtre.
		// activerMinuterie();
	}
}
