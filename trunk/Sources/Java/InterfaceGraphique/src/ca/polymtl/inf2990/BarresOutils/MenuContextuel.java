package ca.polymtl.inf2990.BarresOutils;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Enumeration;

import javax.swing.AbstractButton;
import javax.swing.ButtonGroup;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.JRadioButtonMenuItem;
import javax.swing.KeyStroke;

/**
 * Un menu contextuel. (Menu qui s'affiche au clic-droit.) affichant diff�rents
 * options selon son �tat.
 * 
 * @author Charles �tienne Lalonde
 * 
 */
public class MenuContextuel extends JPopupMenu
{
	private static final long	serialVersionUID	= 1L;	;

	private final ButtonGroup	groupeBoutonsNormaux_;
	private final ButtonGroup	groupeBoutonsRadio_;
	private final ButtonGroup	groupeBoutonsCamera_;

	private JMenu				menuEdition;
	private JMenu				menuInserer_;
	private JMenu				menuVue_;

	private final BarreEdition	fenetreProprietes_;

	// Cree une barre de menu.
	// Recoit en parametres : le gestionnaire d'evenements qui va gerer les
	// commandes de la barre.
	public MenuContextuel()
	{
		// Initialisation des groupes de boutons.
		groupeBoutonsRadio_ = new ButtonGroup();
		groupeBoutonsNormaux_ = new ButtonGroup();
		groupeBoutonsCamera_ = new ButtonGroup();

		fenetreProprietes_ = new BarreEdition();

		creerMenuContextuel();
	}

	/**
	 * Ajoute un �couteur pour g�rer les actions dur les boutons de la barre.
	 * 
	 * @param ecouteur L'�couteur � relier.
	 */
	public void ajouterEcouteurAction(final ActionListener ecouteur)
	{
		AbstractButton bouton;
		Enumeration<AbstractButton> boutons = groupeBoutonsRadio_.getElements();
		while (boutons.hasMoreElements())
		{
			bouton = boutons.nextElement();
			bouton.addActionListener(GestionChangementCurseurSouris.obtenirInstance());
			bouton.addActionListener(ecouteur);
		}

		boutons = groupeBoutonsNormaux_.getElements();
		while (boutons.hasMoreElements())
		{
			bouton = boutons.nextElement();
			bouton.addActionListener(ecouteur);
		}
	}

	/**
	 * Change l'�tat du menu contextuel selon la pr�sence d'un objet ou non sous
	 * la souris.
	 * 
	 * @param objetSelectionnableTrouve La pr�sence d'un objet.
	 */
	public void changerObjetSelectionnablePositionSouris(final boolean objetSelectionnableTrouve)
	{
		if (objetSelectionnableTrouve)
		{
			menuInserer_.setEnabled(false);
			menuVue_.setEnabled(false);
		}
		else
		{
			menuInserer_.setEnabled(true);
			menuVue_.setEnabled(true);
		}
	}

	/**
	 * Cr�e le menu � afficher dans l'�tat Menu_Editeur
	 * 
	 * @return Le menu concret contenant tout ses sous-menus et actions.
	 */
	public void creerMenuContextuel()
	{

		// Cr�ation de la barre "Vue"
		menuVue_ = new JMenu("Vue");

		// ajout des items dans le menu vue.
		menuVue_.add(creerElementRadioMenu(Commandes.CAMERA_FIXE));
		menuVue_.add(creerElementRadioMenu(Commandes.CAMERA_ORBITE));
		menuVue_.add(creerElementRadioMenu(Commandes.CAMERA_LIBRE));
		menuVue_.addSeparator();
		menuVue_.add(creerElementRadioMenu(Commandes.CAMERA));
		menuVue_.add(creerElementRadioMenu(Commandes.ORBIT));
		menuVue_.add(creerElementRadioMenu(Commandes.ZOOM_PROPORTIONNEL));

		// Cr�ation du menu "�dition"
		menuEdition = new JMenu("�dition");

		// ajouter les actions
		menuEdition.add(creerElementRadioMenu(Commandes.EDITEUR_SELECTION));
		menuEdition.add(creerElementRadioMenu(Commandes.EDITEUR_DEPLACER));
		menuEdition.add(creerElementRadioMenu(Commandes.EDITEUR_ROTATION));
		menuEdition.add(creerElementRadioMenu(Commandes.EDITEUR_ECHELLE));
		menuEdition.add(creerElementBoutonMenu(Commandes.SUPPRIMER));
		menuEdition.add(creerElementBoutonMenu(Commandes.DUPLIQUER));

		// Ajout de l'option "Modifier les propri�t�s..."
		JMenuItem editionProprietes = new JMenuItem("Modifier les propri�t�s...");
		editionProprietes.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(final ActionEvent arg0)
			{
				fenetreProprietes_.setVisible(true);
			}

		});
		menuEdition.add(editionProprietes);

		// Cr�ation du menu "ins�rer"
		menuInserer_ = new JMenu("Ins�rer");

		// ajouter les items dans la barre.
		menuInserer_.add(creerElementRadioMenu(Commandes.INSERER_MURET));
		menuInserer_.add(creerElementRadioMenu(Commandes.INSERER_PORTAIL));
		menuInserer_.add(creerElementRadioMenu(Commandes.INSERER_ACCELERATEUR));
		menuInserer_.add(creerElementRadioMenu(Commandes.INSERER_MAILLET));
		menuInserer_.add(creerElementRadioMenu(Commandes.INSERER_RONDELLE));

		// Ajout du menu �dition
		add(menuEdition);

		// Ajout du menu d'insertion.
		add(menuInserer_);

		// Ajout du menu vue
		add(menuVue_);
	}

	/**
	 * Cr�e un bouton de menu standard selon la commande qu'elle doit ex�cuter.
	 * 
	 * @param commande La commande reli�e au bouton.
	 * @return L'Item d�ment cr��.
	 */
	public JMenuItem creerElementBoutonMenu(final Commandes commande)
	{
		final JMenuItem elementMenu = new JMenuItem(ConstantesOutils.NOMS_MENUS[commande.obtenirId()]);
		elementMenu.setActionCommand(commande.toString());
		elementMenu.setToolTipText(ConstantesOutils.TOOLTIPS[commande.obtenirId()]);
		try
		{
			elementMenu.setAccelerator(KeyStroke.getKeyStroke(ConstantesOutils.RACCOURCIS[commande.obtenirId()][0]));
		}
		catch (final ArrayIndexOutOfBoundsException aioobe)
		{

		}

		if (commande.activerQuandObjetSelectionne())
		{
			elementMenu.setEnabled(false);
		}
		groupeBoutonsNormaux_.add(elementMenu);

		return elementMenu;
	}

	/**
	 * Cr�e un �l�ment de type bouton radio (<input type=radio name=a>Bouton)
	 * selon la commande pass�e en param�tres.
	 * 
	 * @param commande La commande reli�e au bouton.
	 * @return L'item � mettre dans le menu.
	 */
	public JMenuItem creerElementRadioMenu(final Commandes commande)
	{
		final JRadioButtonMenuItem elementMenu = new JRadioButtonMenuItem(ConstantesOutils.NOMS_MENUS[commande.obtenirId()]);
		elementMenu.setActionCommand(commande.toString());
		elementMenu.setToolTipText(ConstantesOutils.TOOLTIPS[commande.obtenirId()]);

		try
		{
			elementMenu.setAccelerator(KeyStroke.getKeyStroke(ConstantesOutils.RACCOURCIS[commande.obtenirId()][0]));
		}
		catch (final ArrayIndexOutOfBoundsException aioobe)
		{

		}

		if (commande.activerQuandObjetSelectionne())
		{
			elementMenu.setEnabled(false);
		}
		if (commande == Commandes.CAMERA_FIXE || commande == Commandes.CAMERA_LIBRE || commande == Commandes.CAMERA_ORBITE)
		{
			groupeBoutonsCamera_.add(elementMenu);
		}
		else
		{
			groupeBoutonsRadio_.add(elementMenu);
		}
		return elementMenu;
	}

	/**
	 * Ajuste les s�lection dans les menus afin de s�lectionner l'item reli� �
	 * la commande pass�e en param�tres.
	 * 
	 * @param commande La commande � s�lectionner
	 */
	public void modifierSelection(final Commandes commande)
	{
		Enumeration<AbstractButton> boutons = groupeBoutonsCamera_.getElements();
		AbstractButton boutonASelectionner = null;
		while (boutons.hasMoreElements())
		{
			// Si la commande du prochain bouton est celle de la commande qu'on
			// cherche, quitte la boucle
			if ((boutonASelectionner = boutons.nextElement()).getActionCommand().equals(commande.toString()))
			{
				break;
			}
			else
			{
				boutonASelectionner = null;
			}
		}

		if (boutonASelectionner != null)
		{
			boutonASelectionner.getModel().setSelected(true);
			return;
		}

		boutons = groupeBoutonsRadio_.getElements();
		boutonASelectionner = null;
		while (boutons.hasMoreElements())
		{
			// Si la commande du prochain bouton est celle de la commande qu'on
			// cherche, quitte la boucle
			if ((boutonASelectionner = boutons.nextElement()).getActionCommand().equals(commande.toString()) || (Commandes.valueOf((boutonASelectionner).getActionCommand()).estCommandeParDefaut() && commande == Commandes.EDITEUR_NOUVEAU))
			{
				break;
			}
			else
			{
				boutonASelectionner = null;
			}
		}

		if (boutonASelectionner != null)
		{
			boutonASelectionner.getModel().setSelected(true);
		}
		else if (commande != Commandes.DUPLIQUER && commande != Commandes.SUPPRIMER)
		{
			groupeBoutonsRadio_.clearSelection();
		}
	}

	/**
	 * Rafraichis les boutons � des s�lections par d�faut.
	 * 
	 * @param objetSelectionne
	 */
	public void rafraichirActivationBoutons(final boolean objetSelectionne)
	{
		activerBoutonsSelonEtatCamera();
		activerBoutonsObjetSelectionne(objetSelectionne);
	}

	/**
	 * Active les boutons disponibles selon le mode de cam�ra courant.
	 */
	public void activerBoutonsSelonEtatCamera()
	{
		Enumeration<AbstractButton> boutons = groupeBoutonsRadio_.getElements();
		AbstractButton bouton;
		while (boutons.hasMoreElements())
		{
			bouton = boutons.nextElement();
			Commandes commande = Commandes.valueOf(bouton.getActionCommand());
			bouton.setEnabled(commande.activerPourEtatCamera(Commandes.valueOf(groupeBoutonsCamera_.getSelection().getActionCommand())));
		}

		boutons = groupeBoutonsNormaux_.getElements();
		while (boutons.hasMoreElements())
		{
			bouton = boutons.nextElement();
			Commandes commande = Commandes.valueOf(bouton.getActionCommand());
			bouton.setEnabled(commande.activerPourEtatCamera(Commandes.valueOf(groupeBoutonsCamera_.getSelection().getActionCommand())));
		}
	}

	public void activerBoutonsObjetSelectionne(final boolean objetSelectionne)
	{
		Enumeration<AbstractButton> boutons = groupeBoutonsRadio_.getElements();
		AbstractButton bouton;
		while (boutons.hasMoreElements())
		{
			bouton = boutons.nextElement();
			Commandes commande = Commandes.valueOf(bouton.getActionCommand());
			if (commande.activerQuandObjetSelectionne())
			{
				bouton.setEnabled(objetSelectionne);
			}
			else if (commande.desactiverQuandObjetSelectionne())
			{
				bouton.setEnabled(!objetSelectionne);
			}
		}

		boutons = groupeBoutonsNormaux_.getElements();
		while (boutons.hasMoreElements())
		{
			bouton = boutons.nextElement();
			Commandes commande = Commandes.valueOf(bouton.getActionCommand());
			if (commande.activerQuandObjetSelectionne())
			{
				bouton.setEnabled(objetSelectionne);
			}
			else if (commande.desactiverQuandObjetSelectionne())
			{
				bouton.setEnabled(!objetSelectionne);
			}
		}
	}
}
