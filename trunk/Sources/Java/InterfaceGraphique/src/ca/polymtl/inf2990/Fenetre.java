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
 * Encapsulation de la fen�tre et de ses �v�nements. Plut�t que d'avoir un
 * JFrame � manipuler dans la classe de lancement, on manipule un objet Fenetre.
 * 
 * @author Charles �tienne Lalonde
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
	 * Change l'�tat de la radio entre affich�e et cach�e.
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
	 * Charge la librairie TinyLookAndFeel, qui g�re le th�me visuel des
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
	 * Choisi un des th�mes de la librairie TinyLAF � utiliser.
	 * 
	 * @param nom Le nom du th�me.
	 */
	public void changerLookAndFeel(final String nom)
	{
		for (ThemeDescription theme : Theme.getAvailableThemes())
		{
			if (theme.getName().equals(nom))
			{
				Theme.loadTheme(theme);
				// R�installer le Tiny Look and Feel
				try
				{
					UIManager.setLookAndFeel(new TinyLookAndFeel());
				}
				catch (UnsupportedLookAndFeelException e)
				{
					// TODO Auto-generated catch block
					e.printStackTrace();
				}

				// Mise � jour de toutes les composantes de la fen�tre.
				SwingUtilities.updateComponentTreeUI(getRootPane());

				// Rafraichissement de la fen�tre
				rafraichirFenetre();
				break;
			}
		}
	}

	/**
	 * M�thode qui affiche la fen�tre d'�dition des touches.
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
	 * Cette m�thode cache la fen�tre de modification des touches.
	 */
	public void cacherPanneauModificationTouches()
	{
		if (fenetreTouches != null)
		{
			fenetreTouches.setVisible(false);
		}
	}

	private static Fenetre		instance_;					// une r�f�rence �
	// la fen�tre.

	private static final long	serialVersionUID	= 1L;

	/**
	 * M�thode qui retourne une instance de la fen�tre (Singleton)
	 * 
	 * @return Une r�f�rence � la fen�tre d'affichage.
	 */
	public static Fenetre obtenirInstance()
	{
		if (instance_ == null)// si la fen�tre n'existe pas, on la cr��e avant
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

	// Map qui va contenir les panneaux cr��s.
	private final Map<Etats, PanneauAbstrait>	panneaux_;

	/**
	 * M�thode qui retourne le panneau d'affichage reli� � un �tat particulier.
	 * 
	 * @param etat L'�tat li� au panneau requis.
	 * @return La panneau li� � l'�tat pass� en param�tres
	 */
	public PanneauAbstrait obtenirPanneau(final Etats etat)
	{
		return panneaux_.get(etat);
	}

	/**
	 * M�thode qui demande � la fen�tre de se rafra�chir. (Utile apr�s avoir
	 * ajout�/supprim� un composant dans la fen�tre.)
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
	 * Fonction qui doit �tre appel�e lorsque la fen�tre est redimensionn�e afin
	 * d'ajuster les param�tres de la machine � �tats d'OpenGL pour correspondre
	 * aux nouvelles dimensions de la fen�tre.
	 * 
	 * @param largeur La nouvelle largeur de la fen�tre.
	 * @param hauteur La nouvelle hauteur de la fen�tre.
	 */
	public static native void redimensionnerFenetre(int largeur, int hauteur);

	/**
	 * M�thode qui demande une confirmation d'action si n�c�ssaire. (Ouvre une
	 * fen�tre de confirmation)<br>
	 * Doit �tre appel�e si on veut une confirmation de la part de l'usager
	 * avant d'ex�cuter r�ellement l'action.
	 * 
	 * @param action L'Action que l'usager d�sire accomplir.
	 * @return Si oui ou non, on doit ex�cuter l'action. (La d�cision de
	 *         l'usager)
	 */
	public static boolean validerAction(final String action)
	{
		final Commandes commande = Commandes.valueOf(action);
		int optionChoisie = -1;
		switch (commande)
		{
			case SUPPRIMER:
				optionChoisie = JOptionPane.showOptionDialog(instance_, "Voulez-vous vraiment supprimer la s�lection?", "Confirmation", JOptionPane.YES_NO_OPTION, JOptionPane.INFORMATION_MESSAGE, null, new String[] { "Oui", "Non" }, "Non");
				break;
			case EDITEUR_NOUVEAU:
				// if(EvenementsSelectionneurFichier.obtenirDernierFichierSelectionne()
				// != null)
				// {
				optionChoisie = JOptionPane.showOptionDialog(instance_, "Si vous cr�ez une nouvelle surface, tout changement � la surface actuelle sera perdu. Voulez-vous continuer?", "Confirmation", JOptionPane.YES_NO_OPTION, JOptionPane.INFORMATION_MESSAGE, null, new String[] { "Oui", "Non" }, "Non");
				if (optionChoisie == 0)
				{
					EvenementsSelectionneurFichier.reinitialiserDernierFichierSelectionne();
					((EtatModeEdition) GestionnaireEtats.obtenirInstance().obtenirEtat()).modifierSelection(commande);
				}
				// }
				break;
			case REINITIALISER_PARTIE:
				optionChoisie = JOptionPane.showOptionDialog(instance_, "Si vous r�initialiser la partie tous les points seront remis a z�ro. Voulez-vous continuer?", "Confirmation", JOptionPane.YES_NO_OPTION, JOptionPane.INFORMATION_MESSAGE, null, new String[] { "Oui", "Non" }, "Non");
				break;
		}
		return optionChoisie == 0;
	}

	// R�f�rence vers le menu interactif du haut de la fen�tre.
	private final BarreDeMenus	barreMenus_;

	// Le Canvas AWT qui contiendra le contexte OpenGL.
	private Canvas				canvas_;

	// Fenetre de modification des touches.
	private JDialog				fenetreTouches;

	private JFrame				fenetreRadio_;

	private PanneauAbstrait		panneauRadio_;

	private boolean				openGLinit_	= false;

	/**
	 * Le constructeur de fen�tre. Est appel�e une seule fois, car c'est un
	 * singleton.
	 * 
	 * @param titre Le titre � donner � la fen�tre. S'affiche dans la barre de
	 *            titre de la fen�tre.
	 */
	private Fenetre(final String titre)
	{
		super(titre);
		JPopupMenu.setDefaultLightWeightPopupEnabled(false);
		ToolTipManager.sharedInstance().setLightWeightPopupEnabled(false);

		getContentPane().setLayout(new CardLayout());
		setDefaultCloseOperation(WindowConstants.DO_NOTHING_ON_CLOSE);

		// Cr�ation et ajout de la police.
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
		 * D�tection en haut � gauche { curseur = new
		 * Cursor(Cursor.NW_RESIZE_CURSOR); } else if (posX >= (getWidth() -
		 * ZONE_DETECTION_X) && posY <= ZONE_DETECTION_Y) // D�tection en haut �
		 * droite { curseur = new Cursor(Cursor.NE_RESIZE_CURSOR); } else if
		 * (posX <= ZONE_DETECTION_X && posY >= (getHeight() -
		 * ZONE_DETECTION_Y)) // D�tection en bas � gauche { curseur = new
		 * Cursor(Cursor.SW_RESIZE_CURSOR); } else if (posX >= (getWidth() -
		 * ZONE_DETECTION_X) && posY >= (getHeight() - ZONE_DETECTION_Y)) //
		 * D�tection en bas � droite { curseur = new
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
	 * M�thode qui s'occupe d'ajouter tout les �couteurs aux diff�rents
	 * composants contenues dans la fen�tre.<BR>
	 * Doit seulement �tre appel�e par le constructeur.
	 */
	private void ajouterEvenements()
	{
	}

	/**
	 * Demande � la facade JNI de demander � openGL de repeindre l'affichage.
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
	 * Retourne le Canevas sur lequel OpenGL est appel� � se dessiner.
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

			// Ajout d'un gestionnaire d'�v�nements de souris.
			canvas_.addMouseListener(GestionnaireEvenements.obtenirInstance());

			// Ajout d'un gestionnaire d'�v�nements de clavier.
			canvas_.addKeyListener(GestionnaireEvenements.obtenirInstance());
		}
		return canvas_;
	}

	/**
	 * M�thode qui ajoute un panneauAbstrait � la fen�tre selon une contrainte
	 * d'�tat.
	 * 
	 * @param element Le panneau � ajouter � la liste de panneaux.
	 * @param contrainte L'�tat dans lequel on va afficher ce panneau.
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
	 * M�thode qui demande � la fen�tre de changer l'�tat courant, et ainsi
	 * changer le panneau pr�sentement affich�.
	 * 
	 * @param etat L'�tat dans lequel placer la fen�tre. Selon l'enum Etats.**;
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
	 * Change le mode entre fen�tre ou plein �cran
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
