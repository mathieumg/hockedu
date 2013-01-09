package ca.polymtl.inf2990;

import java.awt.Canvas;

public class OpenGL
{
	/**
	 * Fonction qui affiche la scène dans la fenêtre identifiée par le canvas
	 * AWT passé en paramètre.
	 * 
	 * @param canvas
	 *            Le canvas dans lequel créer le contexte OpenGL.
	 */
	public static native void dessinerOpenGL(Canvas canvas);

	/**
	 * Fonction qui initialise un contexte OpenGL dans la fenêtre identifiée par
	 * le canvas AWT passé en paramètre. Cette fonction doit être la première à
	 * être appelée, car la création de l'objet du modèle C++ s'attend à avoir
	 * un contexte OpenGL valide.
	 * 
	 * @param canvas
	 *            Le canvas dans lequel créer le contexte OpenGL.
	 */
	public static native void initialiserOpenGL(Canvas canvas);

	/**
	 * Fonction qui libère le contexte OpenGL dans la fenêtre identifiée par le
	 * canvas AWT passé en paramètre. Cette fonction doit être la dernière à
	 * être appelée, car elle libère également l'objet du modèle C++.
	 * 
	 * @param canvas
	 *            Le canvas dans lequel créer le contexte OpenGL.
	 */
	public static native void libererOpenGL(Canvas canvas);
}
