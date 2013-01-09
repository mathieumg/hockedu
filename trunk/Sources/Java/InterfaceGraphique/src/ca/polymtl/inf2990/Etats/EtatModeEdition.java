package ca.polymtl.inf2990.Etats;

import java.awt.BorderLayout;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

import javax.swing.SwingUtilities;

import ca.polymtl.inf2990.Fenetre;
import ca.polymtl.inf2990.GestionnaireEvenements;
import ca.polymtl.inf2990.Lancement;
import ca.polymtl.inf2990.BarresOutils.BarreDeMenus;
import ca.polymtl.inf2990.BarresOutils.BarreInsertion;
import ca.polymtl.inf2990.BarresOutils.BarreOutils;
import ca.polymtl.inf2990.BarresOutils.Commandes;
import ca.polymtl.inf2990.BarresOutils.MenuContextuel;
import ca.polymtl.inf2990.Panneaux.PanneauAbstrait;
import ca.polymtl.inf2990.Panneaux.PanneauCanevas;

/**
 * Etat lorsque la fenetre affiche le mode édition
 * 
 * @author Charles Étienne Lalonde
 * 
 */
public class EtatModeEdition extends EtatAbstrait {

	private static BarreOutils barreOutils_;
	private static MenuContextuel menuContextuel_;
	private static RelierBarresOutils relierBarresOutils;
	private static BarreInsertion barreInsertion_;
	private static EcouteurMenuContextuel ecouteurMenuContextuel_;
	private static EcouteurActivationBoutons ecouteurActivationBoutons_;

	// Position du menu contextuel.
	private Point positionMenuContextuel_;

	/**
	 * Classe qui écoute les actions produites sur les barres d'outils.
	 * Lorsqu'une action est détectée, elle va aller sélectionner les boutons
	 * dans les autres barres qui correspondent à cette action afin que
	 * l'interface reste à un état cohérent.
	 * 
	 * @author Charles Étienne Lalonde
	 * 
	 */
	private class RelierBarresOutils implements ActionListener {
		@Override
		public void actionPerformed(final ActionEvent e) {
			final Commandes commande = Commandes.valueOf(e.getActionCommand());
			if (commande != null) {
				// on modifie les sélections des menus/barres d'outils/menus
				// contextuels afin qu'il soit au même état que la barre qui a
				// été utilisé pour choisir l'action.
				if (commande == Commandes.EDITEUR_NOUVEAU) {
					return;
				}
				modifierSelection(commande);
			}
		}
	}

	public void modifierSelection(final Commandes commande) {
		((BarreDeMenus) (Fenetre.obtenirInstance().getJMenuBar()))
				.modifierSelection(commande);
		barreOutils_.modifierSelection(commande);
		menuContextuel_.modifierSelection(commande);
		barreInsertion_.modifierSelection(commande);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see ca.polymtl.inf2990.Etats.EtatAbstrait#initialiser()
	 */
	@Override
	public void initialiser() {
		if (barreOutils_ == null) {
			// Construction de la barre d'outils.
			barreOutils_ = new BarreOutils(Fenetre.obtenirInstance());
			barreOutils_.ajouterEcouteurAction(GestionnaireEvenements
					.obtenirInstance());
			barreOutils_.setFloatable(false);
		}

		if (menuContextuel_ == null) {
			// Construction du menu contextuel
			menuContextuel_ = new MenuContextuel();
			menuContextuel_.ajouterEcouteurAction(GestionnaireEvenements
					.obtenirInstance());
		}

		if (barreInsertion_ == null) {
			// Construction de la barre d'insertion.
			barreInsertion_ = new BarreInsertion(Fenetre.obtenirInstance());
			barreInsertion_.ajouterEcouteurAction(GestionnaireEvenements
					.obtenirInstance());
			barreInsertion_.setFloatable(false);
		}

		if (ecouteurActivationBoutons_ == null) {
			ecouteurActivationBoutons_ = new EcouteurActivationBoutons();
			barreOutils_.ajouterEcouteurAction(ecouteurActivationBoutons_);
			menuContextuel_.ajouterEcouteurAction(ecouteurActivationBoutons_);
			barreInsertion_.ajouterEcouteurAction(ecouteurActivationBoutons_);
			((BarreDeMenus) (Fenetre.obtenirInstance().getJMenuBar()))
					.ajouterEcouteurAction(ecouteurActivationBoutons_);
		}

		if (relierBarresOutils == null) {
			// Ajout de relierBarresOutils aux éléments
			relierBarresOutils = new RelierBarresOutils();
			barreOutils_.ajouterEcouteurAction(relierBarresOutils);
			menuContextuel_.ajouterEcouteurAction(relierBarresOutils);
			barreInsertion_.ajouterEcouteurAction(relierBarresOutils);
			((BarreDeMenus) (Fenetre.obtenirInstance().getJMenuBar()))
					.ajouterEcouteurAction(relierBarresOutils);
		}

		if (ecouteurMenuContextuel_ == null) {
			ecouteurMenuContextuel_ = new EcouteurMenuContextuel(
					menuContextuel_);
		}
		// reveiller();
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see ca.polymtl.inf2990.Etats.EtatAbstrait#terminer()
	 */
	@Override
	public void terminer() {
		mettreAuRepos();
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see ca.polymtl.inf2990.Etats.EtatAbstrait#mettreAuRepos()
	 */
	@Override
	public void mettreAuRepos() {
		Lancement.desactiverMinuterie();
		obtenirPanneau().remove(barreInsertion_);
		obtenirPanneau().remove(barreOutils_);
		Fenetre.obtenirInstance().obtenirCanevas()
				.removeMouseListener(ecouteurMenuContextuel_);
		Fenetre.obtenirInstance().obtenirCanevas()
				.removeMouseListener(ecouteurActivationBoutons_);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see ca.polymtl.inf2990.Etats.EtatAbstrait#obtenirNom()
	 */
	@Override
	public Etats obtenirNom() {
		// TODO Auto-generated method stub
		return Etats.MODE_EDITION;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see ca.polymtl.inf2990.Etats.EtatAbstrait#creerPanneau()
	 */
	@Override
	protected PanneauAbstrait creerPanneau() {
		return null;
	}

	@Override
	public PanneauAbstrait obtenirPanneau() {
		return PanneauCanevas.obtenirInstance();
	}

	@Override
	public void reveiller() {
		Lancement.activerMinuterie();
		obtenirPanneau().add(barreInsertion_, BorderLayout.EAST);
		obtenirPanneau().add(barreOutils_, BorderLayout.NORTH);
		Fenetre.obtenirInstance().obtenirCanevas()
				.addMouseListener(ecouteurMenuContextuel_);
		Fenetre.obtenirInstance().obtenirCanevas()
				.addMouseListener(ecouteurActivationBoutons_);
		((BarreDeMenus) (Fenetre.obtenirInstance().getJMenuBar()))
				.modifierSelection(Commandes.ORBIT);
		barreOutils_.modifierSelection(Commandes.ORBIT);
		menuContextuel_.modifierSelection(Commandes.ORBIT);
		barreInsertion_.modifierSelection(Commandes.ORBIT);

		((BarreDeMenus) (Fenetre.obtenirInstance().getJMenuBar()))
				.modifierSelection(Commandes.CAMERA_ORBITE);
		barreOutils_.modifierSelection(Commandes.CAMERA_ORBITE);
		menuContextuel_.modifierSelection(Commandes.CAMERA_ORBITE);
		barreInsertion_.modifierSelection(Commandes.CAMERA_ORBITE);
	}

	private class EcouteurActivationBoutons extends MouseAdapter implements
			ActionListener {
		@Override
		public void mouseReleased(final MouseEvent e) {
			SwingUtilities.invokeLater(new Runnable() {

				@Override
				public void run() {
					activationSelectionBoutons();
				}
			});
		}

		@Override
		public void actionPerformed(final ActionEvent e) {
			SwingUtilities.invokeLater(new Runnable() {
				@Override
				public void run() {
					activationSelectionBoutons();
				}
			});
		}

		void activationSelectionBoutons() {
			boolean objetSelectionne = objetEstSelectionne();
			boolean pointButSelectionne = pointButEstSelectionne();
			boolean pointPasButSelectionne = pointPasButEstSelectionne();
			((BarreDeMenus) (Fenetre.obtenirInstance().getJMenuBar()))
					.rafraichirActivationBoutons(objetSelectionne);
			barreOutils_.rafraichirActivationBoutons(objetSelectionne,
					pointButSelectionne, pointPasButSelectionne);
			menuContextuel_.rafraichirActivationBoutons(objetSelectionne);
			barreInsertion_.rafraichirActivationBoutons(objetSelectionne);
		}
	}

	/**
	 * Ecouteur sur le menu contextuel afin de gerer les actions qui se
	 * produisent dessus.
	 * 
	 * @author Charles Étienne Lalonde
	 * 
	 */
	private class EcouteurMenuContextuel extends MouseAdapter {
		MenuContextuel menuContextuel_;

		public EcouteurMenuContextuel(final MenuContextuel menuContextuel) {
			menuContextuel_ = menuContextuel;
		}

		/**
		 * Cette méthode demande au C++ de sélectionner l'objet à la position
		 * courante de la souris et affiche le menu contextuel ensuite.
		 * 
		 * @param e
		 *            l'événement qui a déclenché l'interruption.
		 */
		private void afficherMenuContextuel(final MouseEvent e) {
			if (e.isPopupTrigger()) {

				menuContextuel_
						.changerObjetSelectionnablePositionSouris(objetEstSousCurseur(
								e.getX(), e.getY()));
				positionMenuContextuel_ = e.getPoint();
				menuContextuel_.show(e.getComponent(), e.getX(), e.getY());
			}
		}

		@Override
		public void mousePressed(final MouseEvent e) {
			afficherMenuContextuel(e);
		}

		@Override
		public void mouseReleased(final MouseEvent e) {
			afficherMenuContextuel(e);
		}
	}

	/**
	 * Fonction qui doit être appelée lorsque le menu contextuel est appelé afin
	 * d'ajuster le menu selon si un objet est situé sous le curseur de la
	 * souris ou non.
	 * 
	 * @param x
	 *            La position x de la fenetre.
	 * @param y
	 *            La position y de la fenetre.
	 */
	public static native boolean objetEstSousCurseur(int x, int y);

	/**
	 * Fonction qui doit être appelée lorsque l'on clic sur le canevas ou que
	 * l'on pèse sur un bouton afin d'ajuster le menu selon si un objet est
	 * sélectionné ou non.
	 */
	public static native boolean objetEstSelectionne();

	/**
	 * Fonction qui doit être appelée pour savoir si un point de but est
	 * sélectionné
	 */
	public static native boolean pointButEstSelectionne();

	/**
	 * Fonction qui doit être appelée pour savoir si un point qui n'est pas un
	 * point de but est sélectionné
	 */
	public static native boolean pointPasButEstSelectionne();

}
