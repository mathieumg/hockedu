package ca.polymtl.inf2990;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;

/**
 * Ce gestionnaire ne fait que rediriger chaque événement vers le contrôleur JNI
 * du côté de C++.
 * 
 * @author Charles Étienne Lalonde
 * 
 */
public class GestionnaireEvenements implements KeyListener, MouseListener, MouseMotionListener, MouseWheelListener, ActionListener
{
	// Les appels d'événements sont des fonctions natives.
	@Override
	public native void actionPerformed(ActionEvent e); //Événement générée par l'appui sur un bouton.

	@Override
	public native void keyPressed(KeyEvent e); //Événement générée par l'enfoncement d'une touche du clavier.

	@Override
	public native void keyReleased(KeyEvent e); //Événement générée par le relâchement d'une touche du clavier.

	@Override
	public native void keyTyped(KeyEvent e); //Événement générée lors d'une tape sur une touche du clavier..

	@Override
	public native void mouseClicked(MouseEvent e); //Événement générée quand un des boutons de la souris est tapée.

	@Override
	public native void mouseDragged(MouseEvent e); //Événement générée quand la souris est clic-déplacée.

	@Override
	public native void mouseEntered(MouseEvent e); //Événement générée quand la souris arrive par dessus un objet écouté.

	@Override
	public native void mouseExited(MouseEvent e); //Événement générée quand la souris quitte un objet écouté.

	@Override
	public native void mouseMoved(MouseEvent e); //Événement générée quand la souris est déplacée.

	@Override
	public native void mousePressed(MouseEvent e); //Événement générée quand la souris est enfoncée.

	@Override
	public native void mouseReleased(MouseEvent e); //Événement générée quand la souris est relâchée.

	@Override
	public native void mouseWheelMoved(MouseWheelEvent e); //Événement générée quand roulette de la souris est tournée.

	private static GestionnaireEvenements	instance_;

	private GestionnaireEvenements()
	{

	}

	/**
	 * Singleton : retourne une instance de cette classe.
	 * @return Une instance de GestionnaireEvenements
	 */
	public static GestionnaireEvenements obtenirInstance()
	{
		if (instance_ == null)
		{
			instance_ = new GestionnaireEvenements();
		}
		return instance_;
	}
}
