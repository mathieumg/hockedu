package ca.polymtl.inf2990;

import java.awt.Canvas;

public class OpenGL
{
	/**
	 * Fonction qui affiche la sc�ne dans la fen�tre identifi�e par le canvas
	 * AWT pass� en param�tre.
	 * 
	 * @param canvas
	 *            Le canvas dans lequel cr�er le contexte OpenGL.
	 */
	public static native void dessinerOpenGL(Canvas canvas);

	/**
	 * Fonction qui initialise un contexte OpenGL dans la fen�tre identifi�e par
	 * le canvas AWT pass� en param�tre. Cette fonction doit �tre la premi�re �
	 * �tre appel�e, car la cr�ation de l'objet du mod�le C++ s'attend � avoir
	 * un contexte OpenGL valide.
	 * 
	 * @param canvas
	 *            Le canvas dans lequel cr�er le contexte OpenGL.
	 */
	public static native void initialiserOpenGL(Canvas canvas);

	/**
	 * Fonction qui lib�re le contexte OpenGL dans la fen�tre identifi�e par le
	 * canvas AWT pass� en param�tre. Cette fonction doit �tre la derni�re �
	 * �tre appel�e, car elle lib�re �galement l'objet du mod�le C++.
	 * 
	 * @param canvas
	 *            Le canvas dans lequel cr�er le contexte OpenGL.
	 */
	public static native void libererOpenGL(Canvas canvas);
}
