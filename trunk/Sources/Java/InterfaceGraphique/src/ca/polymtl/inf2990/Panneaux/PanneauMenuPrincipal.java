package ca.polymtl.inf2990.Panneaux;

import java.awt.CardLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Graphics;
import java.awt.GridLayout;
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;

import javax.swing.JPanel;

import ca.polymtl.inf2990.Fenetre;
import ca.polymtl.inf2990.GestionnaireImages;
import ca.polymtl.inf2990.GestionnaireImages.NomImage;
import ca.polymtl.inf2990.OpenGL;
import ca.polymtl.inf2990.Composants.BoutonHockey;
import ca.polymtl.inf2990.Etats.EtatJouer;
import ca.polymtl.inf2990.Etats.EtatModeEdition;
import ca.polymtl.inf2990.Etats.EtatOptions;
import ca.polymtl.inf2990.Etats.Etats;
import ca.polymtl.inf2990.Etats.GestionnaireEtats;

/**
 * Panneau représentant le menu d'accueil.
 * 
 * @author Charles Étienne Lalonde
 * 
 */
public class PanneauMenuPrincipal extends PanneauAbstrait
{
	private Image						imgFond;
	private JPanel						panneauCartes_;
	private JPanel						panneauBoutons_;
	private static PanneauMenuPrincipal	instance_;

	private CardLayout					layout_;

	private PanneauMenuPrincipal()
	{
	}

	/**
	 * Singleton
	 * @return Retourne une instance du panneau.
	 */
	public static PanneauMenuPrincipal obtenirInstance()
	{
		if (instance_ == null)
		{
			instance_ = new PanneauMenuPrincipal();
		}
		return instance_;
	}

	@Override
	protected void creer()
	{
		setLayout(null);
		setSize(Fenetre.obtenirInstance().getWidth() - 16, Fenetre.obtenirInstance().getHeight() - 40);
		imgFond = GestionnaireImages.obtenirInstance().obtenirImageSelonTaille(NomImage.MENU_PRINCIPAL, Fenetre.obtenirInstance().getWidth() - 16, Fenetre.obtenirInstance().getHeight() - 40);

		layout_ = new CardLayout();
		panneauCartes_ = new JPanel();
		panneauBoutons_ = new JPanel();
		panneauBoutons_.setSize(getSize());
		panneauCartes_.setLayout(layout_);
		panneauCartes_.setBounds((int) (0.045703125 * getWidth()), (int) (0.2907569444 * getHeight()), (int) (0.270703125 * getWidth()), (int) (0.655875 * getHeight()));
		panneauCartes_.setBackground(Color.WHITE);
		panneauCartes_.setOpaque(false);
		panneauBoutons_.setLayout(new GridLayout(4, 1, 0, (int) (0.050673611 * getHeight())));
		panneauBoutons_.setBackground(Color.WHITE);
		panneauBoutons_.setOpaque(false);

		BoutonHockey boutonEdition = new BoutonHockey(NomImage.BOUTON_NORMAL, NomImage.BOUTON_ENFONCE, NomImage.BOUTON_SURVOLE, "Mode Édition");
		boutonEdition.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(final ActionEvent arg0)
			{
				GestionnaireEtats.obtenirInstance().changerEtat(new EtatModeEdition());
			}

		});
		panneauBoutons_.add(boutonEdition);

		BoutonHockey boutonPartieRapide = new BoutonHockey(NomImage.BOUTON_NORMAL, NomImage.BOUTON_ENFONCE, NomImage.BOUTON_SURVOLE, "Jouer");
		boutonPartieRapide.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(final ActionEvent arg0)
			{
				GestionnaireEtats.obtenirInstance().changerEtat(new EtatJouer());
			}
		});
		panneauBoutons_.add(boutonPartieRapide);

		// Jouer
		panneauCartes_.add(new PanneauJouer(), Etats.MODE_JOUER.toString());
		// Partie rapide
		panneauCartes_.add(new PanneauPartieRapide(), Etats.MODE_OPTION_PARTIE_RAPIDE.toString());
		// Tournois
		panneauCartes_.add(new PanneauOptionsTournoi(), Etats.MODE_OPTION_TOURNOI.toString());

		BoutonHockey boutonOptions = new BoutonHockey(NomImage.BOUTON_NORMAL, NomImage.BOUTON_ENFONCE, NomImage.BOUTON_SURVOLE, "Options");
		boutonOptions.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(final ActionEvent arg0)
			{
				GestionnaireEtats.obtenirInstance().changerEtat(new EtatOptions());
			}
		});
		panneauBoutons_.add(boutonOptions);

		panneauCartes_.add(new PanneauOptions(), Etats.MODE_OPTIONS.toString());

		BoutonHockey boutonQuitter = new BoutonHockey(NomImage.BOUTON_NORMAL, NomImage.BOUTON_ENFONCE, NomImage.BOUTON_SURVOLE, "Quitter");
		boutonQuitter.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(final ActionEvent e)
			{
				OpenGL.libererOpenGL(Fenetre.obtenirInstance().obtenirCanevas());
				System.exit(0);
			}
		});

		panneauBoutons_.add(boutonQuitter);

		panneauBoutons_.addComponentListener(new ComponentAdapter() {
			@Override
			public void componentResized(final ComponentEvent e)
			{
				panneauBoutons_.setLayout(new GridLayout(panneauBoutons_.getComponentCount(), 1, 0, (int) (0.050673611 * getHeight())));
			}
		});

		panneauCartes_.add(panneauBoutons_, Etats.MENU_PRINCIPAL.toString());

		addComponentListener(new ComponentAdapter() {
			@Override
			public void componentResized(final ComponentEvent e)
			{
				imgFond = GestionnaireImages.obtenirInstance().obtenirImageSelonTaille(NomImage.MENU_PRINCIPAL, getWidth(), getHeight());
				panneauCartes_.setBounds((int) (0.045703125 * getWidth()), (int) (0.2907569444 * getHeight()), (int) (0.270703125 * getWidth()), (int) (0.655875 * getHeight()));
				for (Component c : panneauCartes_.getComponents())
				{
					c.getComponentListeners()[0].componentResized(e);
				}
				panneauBoutons_.setLayout(new GridLayout(panneauBoutons_.getComponentCount(), 1, 0, (int) (0.050673611 * getHeight())));
			}
		});

		add(panneauCartes_);
		layout_.show(panneauCartes_, Etats.MENU_PRINCIPAL.toString());
	}

	/**
	 * Affiche un etat.
	 * @param etat l'état à afficher.
	 */
	public void afficher(final Etats etat)
	{
		layout_.show(panneauCartes_, etat.toString());
		Fenetre.rafraichirFenetre();
	}

	@Override
	public void rafraichir()
	{
		getComponentListeners()[0].componentResized(null);
	}

	@Override
	public void paintComponent(final Graphics g)
	{
		g.drawImage(imgFond, 0, 0, null);
	}
}
