package ca.polymtl.inf2990;

import java.awt.EventQueue;
import java.lang.reflect.InvocationTargetException;
import java.util.Timer;
import java.util.TimerTask;

import javax.swing.SwingUtilities;

/**
 * Cette classe cr�e une fen�tre Java ainsi qu'un contexte OpenGL dans lequel la
 * librairie C++ pourra dessiner.
 * 
 * Il ne s'agit que d'un exemple des diff�rentes fonctionnalit�s qu'il est
 * possible d'implanter en Java, notamment la cr�ation d'une fen�tre, d'un
 * contexte OpenGL, d'une barre de menus et d'une barre d'outils, ainsi que la
 * gestionnaire de certains �v�nements.
 * 
 * CETTE CLASSE NE CONSTITUE PAS UN EXEMPLE DE BONNE PROGRAMMATION. Elle est
 * monolithique et vise � �tre s�par�e en plusieurs classes, �ventuellement dans
 * des paquetages diff�rents, lorsque les v�ritables fonctionnalit�s seront
 * implant�es.
 * 
 * @author Martin Bisson
 * @date 2007-08-15
 */
public class Lancement implements Runnable {
	// Le temps souhait� entre 2 affichages, en millisecondes.
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
	 * Elle vise � �tre appel�e de nombreuses fois par seconde. Elle effectue
	 * les diff�rents calculs de physique et effectue un affichage.
	 * 
	 * @param temps
	 *            L'intervalle de temps � utiliser pour les diff�rents calculs.
	 *            On vise � faire correspondre cet invervalle de temps au temps
	 *            entre deux appels � la fonction.
	 */
	public static native void fctC_animer(float temps);

	/**
	 * Fonction qui ex�cute les jeux de tests unitaires C++.
	 */
	public static native int fctC_executerTests();

	/**
	 * Fonction principale de l'application qui constitue en fait son point
	 * d'entr�e. Elle v�rifie en premier si l'usager souhaite ex�cuter les test
	 * unitaires (si l'argument est testsC++). Dans l'affirmative, elle invoque
	 * l'appel des tests unitaires C++ via JNI et retourne le code d'erreur
	 * renvoy�. Sinon, la fonction d'entr�e proc�de au traitement par d�faut :
	 * elle se contente de cr�er un objet <code>Lancement</code> et de
	 * l'invoquer dans le thread AWT.
	 * 
	 * @param args
	 *            Les param�tres pass�es � l'application par la ligne de
	 *            commande. Si le premier argument (args[0]) contient la cha�ne
	 *            "testsC++", les tests unitaires C++ sont invoqu�s et
	 *            l'application quitte (retourne) imm�diatement.
	 */
	public static void main(final String args[]) {
		// V�rifier si nous ex�cuterons les tests unitaires...

		if (args.length >= 1 && args[0].equals("testsC++")) { // Ex�cution des
																// tests C++
			final int resultat = fctC_executerTests();
			if (resultat == 0) {
				System.exit(resultat);
			}
		} // Pas d'ex�cution des tests unitaires, cr��ons la fen�tre et //
			// affichons-l�
		else {
			// Cr�ation de l'unique instance du contr�leur
			final Lancement instance = new Lancement();
			// Cr�ation et affichage de la fen�tre principale
			EventQueue.invokeLater(instance);
		}
	}

	/**
	 * Cette fonction active la minuterie qui appelle p�riodiquement une
	 * fonction afin de rafra�chir constamment la fen�tre.
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
	 * Fonction qui cr�e l'interface graphique et le pr�pare � recevoir des
	 * �v�nements. Cette fonction est appel�e par le thread AWT afin que les
	 * appels au GUI soit tous faits dans le m�me thread.
	 */
	@Override
	public void run() {
		// Chargement de la librairie pour le look & feel.
		Fenetre.chargerTinyLookAndFeel();

		// Appel de la fenetre pour l'afficher
		Fenetre.obtenirInstance();
		Fenetre.rafraichirFenetre();
		Fenetre.obtenirInstance().setVisible(true);

		// Activation de la minuterie appel�e plusieurs fois par seconde pour
		// r�afficher la fen�tre.
		// activerMinuterie();
	}
}
