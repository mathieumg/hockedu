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
 * Ce gestionnaire ne fait que rediriger chaque �v�nement vers le contr�leur JNI
 * du c�t� de C++.
 * 
 * @author Charles �tienne Lalonde
 * 
 */
public class GestionnaireEvenements implements KeyListener, MouseListener, MouseMotionListener, MouseWheelListener, ActionListener
{
	// Les appels d'�v�nements sont des fonctions natives.
	@Override
	public native void actionPerformed(ActionEvent e); //�v�nement g�n�r�e par l'appui sur un bouton.

	@Override
	public native void keyPressed(KeyEvent e); //�v�nement g�n�r�e par l'enfoncement d'une touche du clavier.

	@Override
	public native void keyReleased(KeyEvent e); //�v�nement g�n�r�e par le rel�chement d'une touche du clavier.

	@Override
	public native void keyTyped(KeyEvent e); //�v�nement g�n�r�e lors d'une tape sur une touche du clavier..

	@Override
	public native void mouseClicked(MouseEvent e); //�v�nement g�n�r�e quand un des boutons de la souris est tap�e.

	@Override
	public native void mouseDragged(MouseEvent e); //�v�nement g�n�r�e quand la souris est clic-d�plac�e.

	@Override
	public native void mouseEntered(MouseEvent e); //�v�nement g�n�r�e quand la souris arrive par dessus un objet �cout�.

	@Override
	public native void mouseExited(MouseEvent e); //�v�nement g�n�r�e quand la souris quitte un objet �cout�.

	@Override
	public native void mouseMoved(MouseEvent e); //�v�nement g�n�r�e quand la souris est d�plac�e.

	@Override
	public native void mousePressed(MouseEvent e); //�v�nement g�n�r�e quand la souris est enfonc�e.

	@Override
	public native void mouseReleased(MouseEvent e); //�v�nement g�n�r�e quand la souris est rel�ch�e.

	@Override
	public native void mouseWheelMoved(MouseWheelEvent e); //�v�nement g�n�r�e quand roulette de la souris est tourn�e.

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
