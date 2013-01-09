package ca.polymtl.inf2990;

import java.awt.Canvas;
import java.awt.CardLayout;
import java.awt.Color;
import java.awt.Cursor;
import java.awt.Dialog;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.GraphicsEnvironment;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.HashMap;
import java.util.Map;

import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPopupMenu;
import javax.swing.SwingUtilities;
import javax.swing.ToolTipManager;
import javax.swing.UIManager;
import javax.swing.UnsupportedLookAndFeelException;
import javax.swing.WindowConstants;

import ca.polymtl.inf2990.BarresOutils.BarreDeMenus;
import ca.polymtl.inf2990.BarresOutils.Commandes;
import ca.polymtl.inf2990.BarresOutils.EvenementsSelectionneurFichier;
import ca.polymtl.inf2990.Etats.EtatMenuPrincipal;
import ca.polymtl.inf2990.Etats.EtatModeEdition;
import ca.polymtl.inf2990.Etats.Etats;
import ca.polymtl.inf2990.Etats.GestionnaireEtats;
import ca.polymtl.inf2990.Jeu.OperationsRadioJNI;
import ca.polymtl.inf2990.Panneaux.PanneauAbstrait;
import ca.polymtl.inf2990.Panneaux.PanneauCanevas;
import ca.polymtl.inf2990.Panneaux.PanneauModificationTouches;
import ca.polymtl.inf2990.Panneaux.PanneauRadio;
import de.muntjak.tinylookandfeel.Theme;
import de.muntjak.tinylookandfeel.ThemeDescription;
import de.muntjak.tinylookandfeel.TinyLookAndFeel;

/**
 * Encapsulation de la fenêtre et de ses événements. Plutôt que d'avoir un
 * JFrame à manipuler dans la classe de lancement, on manipule un objet Fenetre.
 * 
 * @author Charles Étienne Lalonde
 * @date 11-02-2012
 */
public class Fenetre extends JFrame
{
	/**
	 * Donne le focus au canevas.
	 */
	public void giveFocusToCanevas()
	{
		canvas_.requestFocus();
	}

	/**
	 * Change l'état de la radio entre affichée et cachée.
	 */
	public void triggerRadio()
	{
		if (fenetreRadio_ == null)
		{
			construireRadio();
		}
		if (fenetreRadio_.isVisible())
		{
			fenetreRadio_.setVisible(false);
		}
		else
		{
			fenetreRadio_.setVisible(true);
		}
		fenetreRadio_.setBounds(getLocationOnScreen().x + getWidth(), getLocationOnScreen().y, 434, 90);
	}

	/**
	 * Affiche la radio, la construit si elle n'existe pas.
	 */
	public void afficherRadio()
	{
		if (fenetreRadio_ == null)
		{
			construireRadio();
		}
		if (fenetreRadio_.isVisible())
		{
			return;
		}

		//fenetreRadio_.setAutoRequestFocus(false); Valide seulement en java 1.7.
		fenetreRadio_.setVisible(true);
		fenetreRadio_.setBounds(getLocationOnScreen().x + getWidth(), getLocationOnScreen().y, 434, 90);
		fenetreRadio_.setResizable(false);
		panneauRadio_.rafraichir();
		requestFocus();
		obtenirCanevas().requestFocus();

	}

	/**
	 * Cache le panneau contenant la radio.
	 */
	public void cacherRadio()
	{
		if (!fenetreRadio_.isVisible())
		{
			return;
		}
		fenetreRadio_.setVisible(false);
	}

	/**
	 * Initialise la fenetre de la radio et ses composantes.
	 */
	private void construireRadio()
	{
		fenetreRadio_ = new JFrame();
		panneauRadio_ = new PanneauRadio();
		fenetreRadio_.setContentPane(panneauRadio_);
	}

	/**
	 * Charge la librairie TinyLookAndFeel, qui gère le thème visuel des
	 * panneaux Java.
	 */
	public static void chargerTinyLookAndFeel()
	{
		// Changement du look & feel
		try
		{
			UIManager.setLookAndFeel("de.muntjak.tinylookandfeel.TinyLookAndFeel");
		}
		catch (Exception ex)
		{
			ex.printStackTrace();
		}
		Toolkit.getDefaultToolkit().setDynamicLayout(true);
		System.setProperty("sun.awt.noerasebackground", "true");
		JFrame.setDefaultLookAndFeelDecorated(true);
		JDialog.setDefaultLookAndFeelDecorated(true);
		String nom = ((TinyLookAndFeel) UIManager.getLookAndFeel()).getDescription();
		for (ThemeDescription theme : Theme.getAvailableThemes())
		{
			if (theme.getName().equals(nom))
			{

			}
		}
	}

	/**
	 * Choisi un des thèmes de la librairie TinyLAF à utiliser.
	 * 
	 * @param nom Le nom du thème.
	 */
	public void changerLookAndFeel(final String nom)
	{
		for (ThemeDescription theme : Theme.getAvailableThemes())
		{
			if (theme.getName().equals(nom))
			{
				Theme.loadTheme(theme);
				// Réinstaller le Tiny Look and Feel
				try
				{
					UIManager.setLookAndFeel(new TinyLookAndFeel());
				}
				catch (UnsupportedLookAndFeelException e)
				{
					// TODO Auto-generated catch block
					e.printStackTrace();
				}

				// Mise à jour de toutes les composantes de la fenêtre.
				SwingUtilities.updateComponentTreeUI(getRootPane());

				// Rafraichissement de la fenêtre
				rafraichirFenetre();
				break;
			}
		}
	}

	/**
	 * Méthode qui affiche la fenêtre d'édition des touches.
	 */
	public void afficherPanneauModificationTouches()
	{
		int larg = 500;
		int haut = 400;
		if (fenetreTouches == null)
		{
			fenetreTouches = new JDialog(new JFrame(), "Modification des touches", Dialog.ModalityType.APPLICATION_MODAL);
			fenetreTouches.setBounds(getX() + (getWidth() / 2) - (larg / 2), getY() + (getHeight() / 2) - (haut / 2), larg, haut);
			PanneauModificationTouches p = new PanneauModificationTouches();
			p.creer();
			fenetreTouches.setContentPane(p);
		}

		fenetreTouches.setBounds(getX() + (getWidth() / 2) - (larg / 2), getY() + (getHeight() / 2) - (haut / 2), larg, haut);
		fenetreTouches.pack();
		((PanneauModificationTouches) fenetreTouches.getContentPane()).activerPanneau();
		fenetreTouches.setVisible(true);
	}

	/**
	 * Cette méthode cache la fenêtre de modification des touches.
	 */
	public void cacherPanneauModificationTouches()
	{
		if (fenetreTouches != null)
		{
			fenetreTouches.setVisible(false);
		}
	}

	private static Fenetre		instance_;					// une référence à
	// la fenêtre.

	private static final long	serialVersionUID	= 1L;

	/**
	 * Méthode qui retourne une instance de la fenêtre (Singleton)
	 * 
	 * @return Une référence à la fenêtre d'affichage.
	 */
	public static Fenetre obtenirInstance()
	{
		if (instance_ == null)// si la fenêtre n'existe pas, on la créée avant
								// de la retourner.
		{
			instance_ = new Fenetre("INF2990");
			instance_.add(PanneauCanevas.obtenirInstance(), "Canevas");
			GestionnaireEtats.obtenirInstance().changerEtat(new EtatMenuPrincipal());

			instance_.panneaux_.put(Etats.MODE_EDITION, PanneauCanevas.obtenirInstance());

			rafraichirFenetre();
		}
		return instance_;
	}

	// Map qui va contenir les panneaux créés.
	private final Map<Etats, PanneauAbstrait>	panneaux_;

	/**
	 * Méthode qui retourne le panneau d'affichage relié à un état particulier.
	 * 
	 * @param etat L'état lié au panneau requis.
	 * @return La panneau lié à l'état passé en paramètres
	 */
	public PanneauAbstrait obtenirPanneau(final Etats etat)
	{
		return panneaux_.get(etat);
	}

	/**
	 * Méthode qui demande à la fenêtre de se rafraîchir. (Utile après avoir
	 * ajouté/supprimé un composant dans la fenêtre.)
	 */
	public static void rafraichirFenetre()
	{
		if (instance_ != null)
		{
			GestionnaireEtats.obtenirInstance().obtenirEtat().rafraichir();
			final Dimension grandeur = instance_.getSize();
			instance_.setPreferredSize(grandeur);
			instance_.repaint();
			instance_.getJMenuBar().repaint();
			instance_.pack();
		}
	}

	/**
	 * Fonction qui doit être appelée lorsque la fenêtre est redimensionnée afin
	 * d'ajuster les paramètres de la machine à états d'OpenGL pour correspondre
	 * aux nouvelles dimensions de la fenêtre.
	 * 
	 * @param largeur La nouvelle largeur de la fenêtre.
	 * @param hauteur La nouvelle hauteur de la fenêtre.
	 */
	public static native void redimensionnerFenetre(int largeur, int hauteur);

	/**
	 * Méthode qui demande une confirmation d'action si nécéssaire. (Ouvre une
	 * fenêtre de confirmation)<br>
	 * Doit être appelée si on veut une confirmation de la part de l'usager
	 * avant d'exécuter réellement l'action.
	 * 
	 * @param action L'Action que l'usager désire accomplir.
	 * @return Si oui ou non, on doit exécuter l'action. (La décision de
	 *         l'usager)
	 */
	public static boolean validerAction(final String action)
	{
		final Commandes commande = Commandes.valueOf(action);
		int optionChoisie = -1;
		switch (commande)
		{
			case SUPPRIMER:
				optionChoisie = JOptionPane.showOptionDialog(instance_, "Voulez-vous vraiment supprimer la sélection?", "Confirmation", JOptionPane.YES_NO_OPTION, JOptionPane.INFORMATION_MESSAGE, null, new String[] { "Oui", "Non" }, "Non");
				break;
			case EDITEUR_NOUVEAU:
				// if(EvenementsSelectionneurFichier.obtenirDernierFichierSelectionne()
				// != null)
				// {
				optionChoisie = JOptionPane.showOptionDialog(instance_, "Si vous créez une nouvelle surface, tout changement à la surface actuelle sera perdu. Voulez-vous continuer?", "Confirmation", JOptionPane.YES_NO_OPTION, JOptionPane.INFORMATION_MESSAGE, null, new String[] { "Oui", "Non" }, "Non");
				if (optionChoisie == 0)
				{
					EvenementsSelectionneurFichier.reinitialiserDernierFichierSelectionne();
					((EtatModeEdition) GestionnaireEtats.obtenirInstance().obtenirEtat()).modifierSelection(commande);
				}
				// }
				break;
			case REINITIALISER_PARTIE:
				optionChoisie = JOptionPane.showOptionDialog(instance_, "Si vous réinitialiser la partie tous les points seront remis a zéro. Voulez-vous continuer?", "Confirmation", JOptionPane.YES_NO_OPTION, JOptionPane.INFORMATION_MESSAGE, null, new String[] { "Oui", "Non" }, "Non");
				break;
		}
		return optionChoisie == 0;
	}

	// Référence vers le menu interactif du haut de la fenêtre.
	private final BarreDeMenus	barreMenus_;

	// Le Canvas AWT qui contiendra le contexte OpenGL.
	private Canvas				canvas_;

	// Fenetre de modification des touches.
	private JDialog				fenetreTouches;

	private JFrame				fenetreRadio_;

	private PanneauAbstrait		panneauRadio_;

	private boolean				openGLinit_	= false;

	/**
	 * Le constructeur de fenêtre. Est appelée une seule fois, car c'est un
	 * singleton.
	 * 
	 * @param titre Le titre à donner à la fenêtre. S'affiche dans la barre de
	 *            titre de la fenêtre.
	 */
	private Fenetre(final String titre)
	{
		super(titre);
		JPopupMenu.setDefaultLightWeightPopupEnabled(false);
		ToolTipManager.sharedInstance().setLightWeightPopupEnabled(false);

		getContentPane().setLayout(new CardLayout());
		setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);

		// Création et ajout de la police.
		GraphicsEnvironment ge = GraphicsEnvironment.getLocalGraphicsEnvironment();

		try
		{
			ge.registerFont(Font.createFont(Font.TRUETYPE_FONT, getClass().getResourceAsStream("/ca/polymtl/inf2990/polices/game_over.ttf")));
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}

		// fenetre_.setCursor(new Cursor(Cursor.HAND_CURSOR));

		addWindowListener(new WindowAdapter() {
			@Override
			public void windowClosing(final WindowEvent e)
			{
				final int optionChoisie = JOptionPane.showOptionDialog(getParent(), "Voulez-vous quitter ou aller au menu principal?", "Confirmation", JOptionPane.YES_NO_OPTION, JOptionPane.INFORMATION_MESSAGE, null, new String[] { "Quitter", "Menu Principal", "Annuler" }, "Annuler");
				if (optionChoisie == 0)
				{
					OpenGL.libererOpenGL(Fenetre.obtenirInstance().obtenirCanevas());
					System.exit(0);
				}
				else if (optionChoisie == 1)
				{
					GestionnaireEtats.obtenirInstance().changerEtat(new EtatMenuPrincipal());
				}
			}
		});

		/*
		 * addMouseMotionListener(new MouseMotionAdapter() { private final int
		 * ZONE_DETECTION_X = 10; private final int ZONE_DETECTION_Y = 10;
		 * 
		 * @Override public void mouseMoved(final MouseEvent e) {
		 * SwingUtilities.invokeLater(new Runnable() {
		 * 
		 * @Override public void run() { int posX = e.getX(); int posY =
		 * e.getY(); System.out.println("X: " + posX + ", Y: " + posY); Cursor
		 * curseur = getCursor();
		 * 
		 * if (posX <= ZONE_DETECTION_X && posY <= ZONE_DETECTION_Y) //
		 * Détection en haut à gauche { curseur = new
		 * Cursor(Cursor.NW_RESIZE_CURSOR); } else if (posX >= (getWidth() -
		 * ZONE_DETECTION_X) && posY <= ZONE_DETECTION_Y) // Détection en haut à
		 * droite { curseur = new Cursor(Cursor.NE_RESIZE_CURSOR); } else if
		 * (posX <= ZONE_DETECTION_X && posY >= (getHeight() -
		 * ZONE_DETECTION_Y)) // Détection en bas à gauche { curseur = new
		 * Cursor(Cursor.SW_RESIZE_CURSOR); } else if (posX >= (getWidth() -
		 * ZONE_DETECTION_X) && posY >= (getHeight() - ZONE_DETECTION_Y)) //
		 * Détection en bas à droite { curseur = new
		 * Cursor(Cursor.SE_RESIZE_CURSOR); } else if (posX <= ZONE_DETECTION_X
		 * || posX >= (getWidth() - ZONE_DETECTION_X)) // Gauche ou droite {
		 * curseur = new Cursor(Cursor.E_RESIZE_CURSOR); } else if (posY <=
		 * ZONE_DETECTION_Y || posY >= (getHeight() - ZONE_DETECTION_Y)) // Haut
		 * ou bas { curseur = new Cursor(Cursor.S_RESIZE_CURSOR); }
		 * 
		 * setCursor(curseur); } }); } });
		 */
		// Construction de la barre de menu
		barreMenus_ = new BarreDeMenus();
		barreMenus_.ajouterEcouteurAction(GestionnaireEvenements.obtenirInstance());
		barreMenus_.ajouterEcouteurAction(new ActionListener() {

			@Override
			public void actionPerformed(final ActionEvent e)
			{
				Fenetre.obtenirInstance().requestFocus();
				Fenetre.obtenirInstance().obtenirCanevas().requestFocus();
			}
		});
		setJMenuBar(barreMenus_);

		setMinimumSize(new Dimension(800, /* getHeight() + 50 */700));

		panneaux_ = new HashMap<Etats, PanneauAbstrait>(Etats.values().length);
		ajouterEvenements();
	}

	/**
	 * Méthode qui s'occupe d'ajouter tout les écouteurs aux différents
	 * composants contenues dans la fenêtre.<BR>
	 * Doit seulement être appelée par le constructeur.
	 */
	private void ajouterEvenements()
	{
	}

	/**
	 * Demande à la facade JNI de demander à openGL de repeindre l'affichage.
	 */
	public void dessinerOpenGL()
	{
		OpenGL.dessinerOpenGL(canvas_);
	}

	@Override
	public void pack()
	{
		super.pack();
		if (!openGLinit_)
		{
			openGLinit_ = true;
			OpenGL.initialiserOpenGL(canvas_);
		}
	}

	/**
	 * Retourne le Canevas sur lequel OpenGL est appelé à se dessiner.
	 * 
	 * @return Le canevas openGL
	 */
	public Canvas obtenirCanevas()
	{
		if (canvas_ == null)
		{
			// Canvas AWT qui contiendra le contexte OpenGL.
			canvas_ = new Canvas();

			canvas_.setBackground(Color.GRAY);
			canvas_.setPreferredSize(new Dimension(500, 500));
			canvas_.requestFocus();

			canvas_.addComponentListener(new ComponentAdapter() {
				@Override
				public void componentResized(final ComponentEvent evenement)
				{
					redimensionnerFenetre(canvas_.getWidth(), canvas_.getHeight());
				}
			});

			addComponentListener(new ComponentAdapter() {
				@Override
				public void componentResized(final ComponentEvent evenement)
				{
					redimensionnerFenetre(canvas_.getWidth(), canvas_.getHeight());
					rafraichirFenetre();
				}
			});

			canvas_.addMouseWheelListener(GestionnaireEvenements.obtenirInstance());
			canvas_.addMouseMotionListener(GestionnaireEvenements.obtenirInstance());

			// Ajout d'un gestionnaire d'événements de souris.
			canvas_.addMouseListener(GestionnaireEvenements.obtenirInstance());

			// Ajout d'un gestionnaire d'événements de clavier.
			canvas_.addKeyListener(GestionnaireEvenements.obtenirInstance());
		}
		return canvas_;
	}

	/**
	 * Méthode qui ajoute un panneauAbstrait à la fenêtre selon une contrainte
	 * d'état.
	 * 
	 * @param element Le panneau à ajouter à la liste de panneaux.
	 * @param contrainte L'état dans lequel on va afficher ce panneau.
	 */
	public void ajouterElement(final PanneauAbstrait element, final Etats contrainte)
	{
		if (!panneaux_.containsValue(element))
		{
			add(element, contrainte.toString());
		}
		panneaux_.put(contrainte, element);
	}

	/**
	 * Méthode qui demande à la fenêtre de changer l'état courant, et ainsi
	 * changer le panneau présentement affiché.
	 * 
	 * @param etat L'état dans lequel placer la fenêtre. Selon l'enum Etats.**;
	 */
	public void changerEtat(final Etats etat)
	{
		if (etat == Etats.MODE_PARTIE_RAPIDE || etat == Etats.MODE_EDITION || etat == Etats.MODE_TOURNOI)
		{
			((CardLayout) getContentPane().getLayout()).show(getContentPane(), "Canevas");
			obtenirCanevas().requestFocus();
		}
		else if (etat == Etats.MODE_OPTION_PARTIE_RAPIDE || etat == Etats.MODE_OPTIONS || etat == Etats.MODE_OPTION_TOURNOI || etat == Etats.CHOIX_JOUEUR || etat == Etats.MODE_JOUER)
		{
			((CardLayout) getContentPane().getLayout()).show(getContentPane(), Etats.MENU_PRINCIPAL.toString());
		}
		else
		{
			((CardLayout) getContentPane().getLayout()).show(getContentPane(), etat.toString());
		}
		barreMenus_.changerMenuCourant(etat);
		rafraichirFenetre();

	}

	@Override
	public void setCursor(final Cursor curseur)
	{
		obtenirCanevas().setCursor(curseur);
	}

	/**
	 * Change le mode entre fenêtre ou plein écran
	 * 
	 * @param estPleinEcran
	 */
	/*
	 * public void doitEtreEnPleinEcran(boolean estPleinEcran) {
	 * setUndecorated(estPleinEcran); if(estPleinEcran) {
	 * setSize(java.awt.Toolkit.getDefaultToolkit().getScreenSize()); } else {
	 * setSize(new Dimension(800,600)); } rafraichirFenetre();
	 * 
	 * }
	 */
	/**
	 * Retourne les dimensions du canevas. Utile pour garder le ratio de
	 * proportion entre openGL et l'affichage Java.
	 * 
	 * @return les dimensions du canevas.
	 */
	public Dimension obtenirTailleCanevas()
	{
		return canvas_.getSize();
	}

	/**
	 * Ferme la radio, ainsi que la musique qu'elle joue.
	 */
	public void terminerRadio()
	{
		if (fenetreRadio_ == null)
		{
			return;
		}
		else
		{
			OperationsRadioJNI.arreterMusiqueRadio();
		}
		cacherRadio();
	}
}
