package ca.polymtl.inf2990.Composants;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.ImageIcon;
import javax.swing.JButton;

import ca.polymtl.inf2990.GestionnaireImages;
import ca.polymtl.inf2990.GestionnaireImages.NomImage;

/**
 * Bouton utilisant les images de GestionnaireImages afin d'�tre plus beau que
 * les boutons standards de Java.
 * 
 * @author Gabriel Couvrette
 * 
 */
public class BoutonHockey extends JButton
{
	private final NomImage	enfonce_, survol_, normal_;

	/**
	 * Cr�e un bouton selon les trois ressources suivantes : une image normale,
	 * une image enfonc�e et une image survol�e. Le nom des images vient des
	 * constantes de GestionnaireImages.
	 * 
	 * @param imageStd Le nom de l'image � afficher normalement.
	 * @param imagePressed Le nom de l'image � afficher lorsque le bouton est
	 *            press�e.
	 * @param imageSurvol Le nom de l'image � afficher lorsque la souris survole
	 *            le bouton.
	 */
	public BoutonHockey(final NomImage imageStd, final NomImage imagePressed, final NomImage imageSurvol)
	{
		normal_ = imageStd;
		survol_ = imageSurvol;
		enfonce_ = imagePressed;
		setLayout(null);
		setOpaque(false);
		setSize(new Dimension(100, 50));// par defaut.
		setForeground(Color.RED);
		setFont(new Font("Game Over", Font.PLAIN, (int) (0.39408839779 * getHeight()) * 2));
		setFont(getFont().deriveFont(Font.PLAIN, (int) (0.39408839779 * getHeight())));
		setIcon(new ImageIcon(GestionnaireImages.obtenirInstance().obtenirImageSelonTaille(normal_, getWidth(), getHeight())));
		setPressedIcon(new ImageIcon(GestionnaireImages.obtenirInstance().obtenirImageSelonTaille(enfonce_, getWidth(), getHeight())));
		setBorder(null);
		setBackground(null);
		ajouterEcouteurSouris();
		setHorizontalTextPosition(JButton.CENTER);
		setVerticalTextPosition(JButton.CENTER);

		addComponentListener(new ComponentAdapter() {
			@Override
			public void componentResized(final ComponentEvent e)
			{
				if (getWidth() != 0 && getHeight() != 0)
				{
					setIcon(new ImageIcon(GestionnaireImages.obtenirInstance().obtenirImageSelonTaille(normal_, getWidth(), getHeight())));
					setPressedIcon(new ImageIcon(GestionnaireImages.obtenirInstance().obtenirImageSelonTaille(enfonce_, getWidth(), getHeight())));
					setFont(new Font("Game Over", Font.PLAIN, (int) (0.39408839779 * getHeight()) * 2));
				}
			}
		});
	}

	/**
	 * Cr�e un bouton selon les trois ressources suivantes : une image normale,
	 * une image enfonc�e et une image survol�e. <br>
	 * Le nom des images vient des constantes de GestionnaireImages. <br>
	 * De plus, ce constructeur ajoute du texte par dessus le bouton.
	 * 
	 * @param imageStd Le nom de l'image � afficher normalement.
	 * @param imagePressed Le nom de l'image � afficher lorsque le bouton est
	 *            press�e.
	 * @param imageSurvol Le nom de l'image � afficher lorsque la souris survole
	 *            le bouton.
	 * @param texte
	 */
	public BoutonHockey(final NomImage imageStd, final NomImage imagePressed, final NomImage imageSurvol, final String texte)
	{
		this(imageStd, imagePressed, imageSurvol);
		setText(texte);
	}

	@Override
	public void setSize(final int width, final int height)
	{
		super.setSize(width, height);
		setIcon(new ImageIcon(GestionnaireImages.obtenirInstance().obtenirImageSelonTaille(normal_, getWidth(), getHeight())));
		setPressedIcon(new ImageIcon(GestionnaireImages.obtenirInstance().obtenirImageSelonTaille(enfonce_, getWidth(), getHeight())));
	}

	/**
	 * Ajoute les �couteurs requis au bon fonctionnement du bouton. Devrait
	 * uniquement �tre appel� par le constructeur.
	 */
	private void ajouterEcouteurSouris()
	{
		addMouseListener(new EcouteurEffets());
	}

	/**
	 * M�thode qui dicte le foncionnement du bouton en mode survol.
	 */
	private void survolerImage()
	{
		setIcon(new ImageIcon(GestionnaireImages.obtenirInstance().obtenirImageSelonTaille(survol_, getWidth(), getHeight())));
	}

	/**
	 * M�thode qui ram�ne le bouton en mode normal.
	 */
	private void resetImage()
	{
		setIcon(new ImageIcon(GestionnaireImages.obtenirInstance().obtenirImageSelonTaille(normal_, getWidth(), getHeight())));
	}

	/**
	 * Ecouteur qui s'occupe de changer les images a afficher selon l'etat du
	 * bouton.
	 * 
	 * @author Gabriel Couvrette
	 * 
	 */
	private class EcouteurEffets extends MouseAdapter
	{
		@Override
		public void mouseEntered(final MouseEvent e)
		{
			survolerImage();
		}

		@Override
		public void mouseExited(final MouseEvent e)
		{
			resetImage();
		}

	}

}
