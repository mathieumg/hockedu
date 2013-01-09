package ca.polymtl.inf2990.BarresOutils;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.Hashtable;

import javax.swing.AbstractButton;
import javax.swing.ButtonGroup;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JOptionPane;
import javax.swing.JRadioButtonMenuItem;
import javax.swing.KeyStroke;

import ca.polymtl.inf2990.Fenetre;
import ca.polymtl.inf2990.OpenGL;
import ca.polymtl.inf2990.Etats.EtatMenuPrincipal;
import ca.polymtl.inf2990.Etats.EtatModeEdition;
import ca.polymtl.inf2990.Etats.EtatOptionPartieRapide;
import ca.polymtl.inf2990.Etats.Etats;
import ca.polymtl.inf2990.Etats.GestionnaireEtats;

/**
 * Barre de menus pouvant changer d'état au cours de l'exécution, et donc
 * afficher différentes options.
 * 
 * @author Gabriel Couvrette
 * 
 */
public class BarreDeMenus extends JMenuBar
{
	private final BarreEdition	fenetreProprietes_;

	/**
	 * Cette classe est une barre concrète pouvant être utilisée comme barre de
	 * menu.
	 * 
	 * @author Gabriel Couvrette
	 * 
	 */
	private class MenuConcret
	{
		ArrayList<JMenu>	menus;	// La liste des Options du menus (Fichier,
									// edition etc...) qui eux mêmes contiennes
									// les items (nouveau, seuvegarder etc...)

		/**
		 * Crée un MenuConcret afin de pouvoir y ajouter des menus.
		 */
		public MenuConcret()
		{
			menus = new ArrayList<JMenu>();
		}

		/**
		 * Ajoute une option de menu dans la barre concrète.<br>
		 * Ex : <input type=button value=Fichier>
		 * 
		 * @param menu Le menu à ajouter.
		 */
		public void ajouterMenu(final JMenu menu)
		{
			menus.add(menu);
		}

		/**
		 * Retourne le sous-menu contenu à l'index passée en paramètres.<br>
		 * Exemple : <input type=button value=Fichier><input type=button
		 * value=Édition><input type=button value=Vue><br>
		 * Ici, index=1 retourne le composant menu "Édition".
		 * 
		 * @param index L'index du sous-menu désiré.
		 * @return Le sous menu à l'index. NULL si inexistant.
		 */
		public JMenu getMenu(final int index)
		{
			try
			{
				return menus.get(index);
			}
			catch (Exception ex)
			{
				return null;
			}
		}

		/**
		 * Retourne le nombre de sous-menus contenus dans le menu concret.
		 * 
		 * @return Le nombre de sous-menus contenus dans le menu concret.
		 */
		public int nombreDeMenus()
		{
			return menus.size();
		}
	}

	private static final long					serialVersionUID	= 1L;	;

	private final ButtonGroup					groupeBoutonsNormaux_;
	private final ButtonGroup					groupeBoutonsRadio_;
	private final ButtonGroup					groupeBoutonsCamera_;

	private final Hashtable<Etats, MenuConcret>	menus_;

	/**
	 * Construit la barre de menus à états. (Celle qui est réellement ajoutée à
	 * la fenêtre et qui contient toutes les autres.)
	 */
	public BarreDeMenus()
	{
		fenetreProprietes_ = new BarreEdition();
		// Initialisation des groupes de boutons.
		groupeBoutonsRadio_ = new ButtonGroup();
		groupeBoutonsNormaux_ = new ButtonGroup();
		groupeBoutonsCamera_ = new ButtonGroup();

		menus_ = new Hashtable<Etats, BarreDeMenus.MenuConcret>(5);
		menus_.put(Etats.MODE_EDITION, creerBarreEditeur());
		menus_.put(Etats.MODE_PARTIE_RAPIDE, creerBarreJeu());
		menus_.put(Etats.MODE_TOURNOI, creerBarreTournoi());
	}

	/**
	 * Ajoute un écouteur d'actions à la barre. Ce dernier devra gérer le clic
	 * sur les différents éléments du menu.
	 * 
	 * @param ecouteur L'écouteur à ajouter.
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

		boutons = groupeBoutonsCamera_.getElements();
		while (boutons.hasMoreElements())
		{
			bouton = boutons.nextElement();
			bouton.addActionListener(ecouteur);
		}
		repaint();
	}

	/**
	 * Cette méthode remplace le contenu de la barre pour le contenu d'un autre
	 * état.
	 * 
	 * @param nouvelEtat L'état dans lequel placer la barre. Ne fait rien si
	 *            l'état n'existe pas.
	 */
	public void changerMenuCourant(final Etats nouvelEtat)
	{
		removeAll();
		if (menus_.containsKey(nouvelEtat))
		{
			final MenuConcret menuChoisi = menus_.get(nouvelEtat);

			for (int i = 0; i < menuChoisi.nombreDeMenus(); i++)
			{
				add(menuChoisi.getMenu(i));
			}
		}
	}

	/**
	 * Crée le MenuConcret à afficher dans l'état Menu_Editeur
	 * 
	 * @return Le menu concret contenant tout ses sous-menus et actions.
	 */
	public MenuConcret creerBarreEditeur()
	{
		// Cree un menu concret qui va contenir les options pour le mode
		// editeur.
		final MenuConcret menuEditeur = new MenuConcret();

		// Cree l'option de menu "Fichier"
		final JMenu menuFichier = new JMenu("Fichier");

		// Ajoute les items au menu fichier.
		menuFichier.add(creerElementBoutonMenu(Commandes.EDITEUR_NOUVEAU));
		menuFichier.add(creerElementBoutonMenu(Commandes.EDITEUR_SAUVEGARDER));
		menuFichier.add(creerElementBoutonMenu(Commandes.EDITEUR_SAUVEGARDER_SOUS));
		menuFichier.add(creerElementBoutonMenu(Commandes.EDITEUR_CHARGER));
		menuFichier.add(creerElementBoutonMenu(Commandes.QUITTER));

		// ajout de la barre "VUE"
		final JMenu menuVue = new JMenu("Vue");

		// ajout des items dans le menu vue.
		menuVue.add(creerElementRadioMenu(Commandes.CAMERA_FIXE));
		menuVue.add(creerElementRadioMenu(Commandes.CAMERA_ORBITE));
		menuVue.add(creerElementRadioMenu(Commandes.CAMERA_LIBRE));
		menuVue.addSeparator();
		menuVue.add(creerElementRadioMenu(Commandes.CAMERA));
		menuVue.add(creerElementRadioMenu(Commandes.ORBIT));
		menuVue.add(creerElementRadioMenu(Commandes.ZOOM_PROPORTIONNEL));

		// Ajout de l'option pour le mode d'écran
		/*
		 * final JCheckBoxMenuItem optionEstPleinEcran = new
		 * JCheckBoxMenuItem("Plein écran");
		 * optionEstPleinEcran.addActionListener(new ActionListener() {
		 * 
		 * @Override public void actionPerformed(ActionEvent e) {
		 * Fenetre.obtenirInstance
		 * ().doitEtreEnPleinEcran(optionEstPleinEcran.getState());
		 * 
		 * } });
		 * 
		 * menuVue.add(optionEstPleinEcran);
		 */

		// cree le menu edition
		final JMenu menuEdition = new JMenu("Édition");

		// ajouter les actions

		menuEdition.add(creerElementRadioMenu(Commandes.EDITEUR_SELECTION));
		menuEdition.add(creerElementRadioMenu(Commandes.EDITEUR_DEPLACER));
		menuEdition.add(creerElementRadioMenu(Commandes.EDITEUR_ROTATION));
		menuEdition.add(creerElementRadioMenu(Commandes.EDITEUR_ECHELLE));
		menuEdition.add(creerElementBoutonMenu(Commandes.SUPPRIMER));
		menuEdition.add(creerElementBoutonMenu(Commandes.DUPLIQUER));
		JMenuItem afficherModifierTouches = new JMenuItem("Ajuster les touches");
		afficherModifierTouches.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(final ActionEvent e)
			{
				Fenetre.obtenirInstance().afficherPanneauModificationTouches();
			}
		});
		menuEdition.add(/* creerElementBoutonMenu(Commandes.MODIFIER_TOUCHE) */afficherModifierTouches);

		// Ajout de l'option "Modifier les propriétés..."
		JMenuItem editionProprietes = new JMenuItem("Modifier les propriétés...");
		editionProprietes.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(final ActionEvent arg0)
			{
				fenetreProprietes_.setVisible(true);
			}

		});
		menuEdition.add(editionProprietes);
		// ajout du menu "insérer"
		final JMenu menuInserer = new JMenu("Insérer");

		// ajouter les items dans la barre.
		menuInserer.add(creerElementRadioMenu(Commandes.INSERER_MURET));
		menuInserer.add(creerElementRadioMenu(Commandes.INSERER_PORTAIL));
		menuInserer.add(creerElementRadioMenu(Commandes.INSERER_ACCELERATEUR));
		// menuInserer.add(creerElementRadioMenu(Commandes.INSERER_BUT));
		menuInserer.add(creerElementRadioMenu(Commandes.INSERER_MAILLET));
		menuInserer.add(creerElementRadioMenu(Commandes.INSERER_RONDELLE));

		// Cree ses items :

		final JMenu menuMode = new JMenu("Mode");

		JMenuItem optionEdition = creerElementBoutonMenu(Commandes.ALLER_MODE_EDITION);
		optionEdition.setEnabled(false);

		JMenuItem optionJouer = creerElementBoutonMenu(Commandes.ALLER_MODE_JEU);
		optionJouer.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(final ActionEvent e)
			{
				GestionnaireEtats.obtenirInstance().changerEtat(new EtatOptionPartieRapide());
			}
		});

		menuMode.add(optionEdition);
		menuMode.add(optionJouer);

		final JMenu menuAide = new JMenu("Aide");
		final JMenuItem optionAPropos = new JMenuItem("À propos"); // / Affiche
																	// le nom de
																	// l'équipe
																	// et de ses
																	// membres.
		optionAPropos.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(final ActionEvent e)
			{
				JOptionPane.showMessageDialog(Fenetre.obtenirInstance(), "Air Hockey \nÉquipe : 2990-02 \nMembres : \nVincent Lemire \nCharles Étienne Lalonde \nGabriel Couvrette \nMichael Ferris \nMathieu Parent \nSamuel Ledoux");
			}
		});

		menuAide.add(optionAPropos);

		// Ajouter le menu "fichier" au menu concret.
		menuEditeur.ajouterMenu(menuFichier);

		// Ajout du menu d'insertion.
		menuEditeur.ajouterMenu(menuInserer);

		// Ajout du menu édition
		menuEditeur.ajouterMenu(menuEdition);

		// Ajout du menu vue
		menuEditeur.ajouterMenu(menuVue);

		// Ajout du menu mode.
		menuEditeur.ajouterMenu(menuMode);

		menuEditeur.ajouterMenu(menuAide);

		//

		return menuEditeur;
	}

	/**
	 * Crée le MenuConcret à afficher dans l'état Menu_Jeu
	 * 
	 * @return Le menu concret contenant tout ses sous-menus et actions.
	 */
	public MenuConcret creerBarreTournoi()
	{
		// Cree un menu concret qui va contenir les options pour le mode
		// editeur.
		final MenuConcret menuTournoi = new MenuConcret();

		// Cree l'option de menu "Fichier"
		final JMenu menuFichier = new JMenu("Fichier");
		// Cree ses items :
		// final JMenuItem optionNouveau = new JMenuItem("Nouveau");

		// Ajoute les items au menu fichier.
		menuFichier.add(creerElementBoutonMenu(Commandes.REINITIALISER_PARTIE));
		menuFichier.add(creerElementBoutonMenu(Commandes.PAUSE_JEU));
		menuFichier.add(creerElementBoutonMenu(Commandes.QUITTER));

		// Ajouter le menu "fichier" au menu concret.
		menuTournoi.ajouterMenu(menuFichier);

		JMenu menuOptions = new JMenu("Options");
		JMenuItem afficherModifierTouches = new JMenuItem("Ajuster les touches");
		afficherModifierTouches.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(final ActionEvent e)
			{
				Fenetre.obtenirInstance().afficherPanneauModificationTouches();
			}
		});
		menuOptions.add(/* creerElementBoutonMenu(Commandes.MODIFIER_TOUCHE) */afficherModifierTouches);
		menuOptions.add(creerElementBoutonMenu(Commandes.REPLAY));
		menuTournoi.ajouterMenu(menuOptions);

		JMenu radio = new JMenu("Radio");
		radio.addMouseListener(new MouseAdapter() {
			@Override
			public void mousePressed(final MouseEvent e)
			{
				Fenetre.obtenirInstance().triggerRadio();
			}
		});
		menuTournoi.ajouterMenu(radio);

		// ajout de la barre "VUE"
		final JMenu menuVue = new JMenu("Vue");

		// ajout des items dans le menu vue.
		menuVue.add(creerElementRadioMenu(Commandes.CAMERA_FIXE));
		menuVue.add(creerElementRadioMenu(Commandes.CAMERA_ORBITE));
		menuVue.add(creerElementRadioMenu(Commandes.CAMERA_LIBRE));
		menuVue.add(creerElementRadioMenu(Commandes.CAMERA_SPLIT));
		menuVue.addSeparator();
		menuVue.add(creerElementRadioMenu(Commandes.CAMERA));
		menuVue.add(creerElementRadioMenu(Commandes.ORBIT));

		// Ajouter le menu "vue" au menu concret.
		menuTournoi.ajouterMenu(menuVue);

		return menuTournoi;
	}

	public MenuConcret creerBarreJeu()
	{
		// Cree un menu concret qui va contenir les options pour le mode
		// editeur.
		final MenuConcret menuJeu = new MenuConcret();

		// Cree l'option de menu "Fichier"
		final JMenu menuFichier = new JMenu("Fichier");
		// Cree ses items :
		// final JMenuItem optionNouveau = new JMenuItem("Nouveau");

		// Ajoute les items au menu fichier.
		menuFichier.add(creerElementBoutonMenu(Commandes.REINITIALISER_PARTIE));
		menuFichier.add(creerElementBoutonMenu(Commandes.PAUSE_JEU));
		menuFichier.add(creerElementBoutonMenu(Commandes.QUITTER));

		// Ajouter le menu "fichier" au menu concret.
		menuJeu.ajouterMenu(menuFichier);

		JMenu menuOptions = new JMenu("Options");
		JMenuItem afficherModifierTouches = new JMenuItem("Ajuster les touches");
		afficherModifierTouches.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(final ActionEvent e)
			{
				Fenetre.obtenirInstance().afficherPanneauModificationTouches();
			}
		});
		menuOptions.add(/* creerElementBoutonMenu(Commandes.MODIFIER_TOUCHE) */afficherModifierTouches);
		menuOptions.add(creerElementBoutonMenu(Commandes.REPLAY));
		menuJeu.ajouterMenu(menuOptions);

		final JMenu menuMode = new JMenu("Mode");
		JMenuItem optionEdition = creerElementBoutonMenu(Commandes.ALLER_MODE_EDITION);
		optionEdition.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(final ActionEvent e)
			{
				GestionnaireEtats.obtenirInstance().changerEtat(new EtatModeEdition());
			}
		});

		JMenuItem optionJouer = creerElementBoutonMenu(Commandes.ALLER_MODE_JEU);
		optionJouer.setEnabled(false);

		menuMode.add(optionEdition);
		menuMode.add(optionJouer);

		menuJeu.ajouterMenu(menuMode);

		JMenu radio = new JMenu("Radio");
		radio.addMouseListener(new MouseAdapter() {
			@Override
			public void mousePressed(final MouseEvent e)
			{
				Fenetre.obtenirInstance().triggerRadio();
			}
		});
		menuJeu.ajouterMenu(radio);

		// ajout de la barre "VUE"
		final JMenu menuVue = new JMenu("Vue");

		// ajout des items dans le menu vue.
		menuVue.add(creerElementRadioMenu(Commandes.CAMERA_FIXE));
		menuVue.add(creerElementRadioMenu(Commandes.CAMERA_ORBITE));
		menuVue.add(creerElementRadioMenu(Commandes.CAMERA_LIBRE));
		menuVue.add(creerElementRadioMenu(Commandes.CAMERA_SPLIT));
		menuVue.addSeparator();
		menuVue.add(creerElementRadioMenu(Commandes.CAMERA));
		menuVue.add(creerElementRadioMenu(Commandes.ORBIT));
		menuVue.addSeparator();

		// Ajouter le menu "vue" au menu concret.
		menuJeu.ajouterMenu(menuVue);

		return menuJeu;
	}

	/**
	 * Crée un bouton de menu standard selon la commande qu'elle doit exécuter.
	 * 
	 * @param commande La commande reliée au bouton.
	 * @return L'Item dûment créé.
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

		if (commande == Commandes.QUITTER)
		{
			elementMenu.addActionListener(new ActionListener() {
				@Override
				public void actionPerformed(final ActionEvent e)
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
		}
		else if (commande == Commandes.EDITEUR_SAUVEGARDER || commande == Commandes.EDITEUR_CHARGER || commande == Commandes.EDITEUR_SAUVEGARDER_SOUS)
		{
			elementMenu.addActionListener(EvenementsSelectionneurFichier.obtenirInstance());
		}
		else if (!(commande == Commandes.ALLER_MODE_EDITION || commande == Commandes.ALLER_MODE_JEU))
		{
			groupeBoutonsNormaux_.add(elementMenu);
		}

		if (commande.activerQuandObjetSelectionne())
		{
			elementMenu.setEnabled(false);
		}

		return elementMenu;
	}

	/**
	 * Crée un élément de type bouton radio (<input type=radio name=a>Bouton)
	 * selon la commande passée en paramètres.
	 * 
	 * @param commande La commande reliée au bouton.
	 * @return L'item à mettre dans le menu.
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
	 * Ajuste les sélection dans les menus afin de sélectionner l'item relié à
	 * la commande passée en paramètres.
	 * 
	 * @param commande La commande à sélectionner
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

	public void rafraichirActivationBoutons(final boolean objetSelectionne)
	{
		activerBoutonsSelonEtatCamera();
		activerBoutonsObjetSelectionne(objetSelectionne);
	}

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
