package ca.polymtl.inf2990.BarresOutils;

import java.awt.Component;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Enumeration;

import javax.swing.AbstractAction;
import javax.swing.AbstractButton;
import javax.swing.Action;
import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JToggleButton;
import javax.swing.JToolBar;
import javax.swing.KeyStroke;
import javax.swing.border.Border;
import javax.swing.border.TitledBorder;

/**
 * Barre d'outils contenant tout les boutons d'ajout d'objets.
 * 
 * @author Charles Étienne Lalonde
 * 
 */
public class BarreInsertion extends JToolBar
{
	/**
	 * Classe qui gère l'appel d'actions par les boutons.
	 * 
	 * @author gacou
	 * 
	 */
	private class ActiverBouton extends AbstractAction
	{
		private static final long	serialVersionUID	= 1L;

		@Override
		public void actionPerformed(final ActionEvent e)
		{
			final JToggleButton b = (JToggleButton) e.getSource();
			b.getModel().setSelected(true);
			b.doClick();
		}
	}

	private static final long	serialVersionUID	= 1L;

	private final ButtonGroup	groupeBoutons_;

	/**
	 * Construit la barre d'insertion.
	 * 
	 * @param parent Le parent sur lequel on va ajouter la barre.
	 */
	public BarreInsertion(final Component parent)
	{
		groupeBoutons_ = new ButtonGroup();
		final Action activerBouton = new ActiverBouton();

		JToolBar panneauBoutonsAjout = new JToolBar();
		JLabel labelTemp = new JLabel();
		Border bordurePanneauBoutonsAjout = BorderFactory.createLineBorder(labelTemp.getForeground(), 2);
		bordurePanneauBoutonsAjout = BorderFactory.createTitledBorder(bordurePanneauBoutonsAjout, "Objets", TitledBorder.LEADING, TitledBorder.DEFAULT_POSITION, new Font("Game Over", Font.ITALIC, 30));
		panneauBoutonsAjout.setBorder(bordurePanneauBoutonsAjout);
		// Ajout des boutons
		panneauBoutonsAjout.add(creerBouton(Commandes.INSERER_MURET, activerBouton));
		panneauBoutonsAjout.add(creerBouton(Commandes.INSERER_PORTAIL, activerBouton));
		panneauBoutonsAjout.add(creerBouton(Commandes.INSERER_ACCELERATEUR, activerBouton));
		panneauBoutonsAjout.add(creerBouton(Commandes.INSERER_RONDELLE, activerBouton));
		panneauBoutonsAjout.add(creerBouton(Commandes.INSERER_MAILLET, activerBouton));

		setOrientation(JToolBar.VERTICAL);
		panneauBoutonsAjout.setOrientation(JToolBar.VERTICAL);

		add(panneauBoutonsAjout);
		/*
		 * // Curseur par défaut: Celui de déplacement de la caméra Component
		 * composant = parent; while (!(composant instanceof JFrame)) {
		 * System.out.println(composant.getClass().toString()); composant =
		 * composant.getParent(); }
		 * 
		 * ((JFrame) composant).setCursor(new Cursor(Cursor.HAND_CURSOR));
		 */
	}

	/**
	 * Ajoute un bouton à la barre.
	 * 
	 * @param commandeDuBouton La commande qu'on veut que le bouton exécute.
	 * @param activerBouton L'action a ajouter.
	 */
	public JToggleButton creerBouton(final Commandes commandeDuBouton, final Action activerBouton)
	{
		// Crée le bouton
		final JToggleButton bouton = new JToggleButton();
		final int positionTableaux = commandeDuBouton.obtenirId();
		if (!ConstantesOutils.IMAGES[positionTableaux].equals(""))
		{
			try
			{
				bouton.setIcon(new ImageIcon(getClass().getResource("/ca/polymtl/inf2990/icones/" + ConstantesOutils.IMAGES[positionTableaux])));
			}
			catch (final NullPointerException npe)
			{
				bouton.setText(ConstantesOutils.NOMS_MENUS[positionTableaux]);
			}
		}
		else
		{
			bouton.setText(ConstantesOutils.NOMS_MENUS[positionTableaux]);
		}
		bouton.setActionCommand(commandeDuBouton.toString());
		bouton.setToolTipText(ConstantesOutils.TOOLTIPS[positionTableaux]);

		// Ajoute tous les raccourcis
		for (final String element : ConstantesOutils.RACCOURCIS[positionTableaux])
		{
			bouton.getInputMap(WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke(element), commandeDuBouton.toString());
		}
		bouton.getActionMap().put(commandeDuBouton.toString(), activerBouton);

		if (commandeDuBouton.activerQuandObjetSelectionne())
		{
			bouton.setEnabled(false);
		}

		// Ajout dans la barre
		groupeBoutons_.add(bouton);
		return bouton;
	}

	/**
	 * Sélectionne le bouton concerné par la commande passée en paramêtres.
	 * 
	 * @param commande La commande à sélectionner.
	 */
	public void modifierSelection(final Commandes commande)
	{
		if (commande == Commandes.CAMERA_FIXE || commande == Commandes.CAMERA_LIBRE || commande == Commandes.CAMERA_ORBITE)
		{
			return;
		}

		Enumeration<AbstractButton> boutons = groupeBoutons_.getElements();
		AbstractButton boutonASelectionner = null;
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
			groupeBoutons_.clearSelection();
		}
	}

	/**
	 * Ajoute un écouteur sur les boutons.
	 * 
	 * @param ecouteur L'écouteur à ajouter.
	 */
	public void ajouterEcouteurAction(final ActionListener ecouteur)
	{
		final Enumeration<AbstractButton> boutons = groupeBoutons_.getElements();
		while (boutons.hasMoreElements())
		{
			boutons.nextElement().addActionListener(ecouteur);
		}
	}

	/**
	 * @see void activerBoutonsObjetSelectionne(final boolean objetSelectionne)
	 */
	public void rafraichirActivationBoutons(final boolean objetSelectionne)
	{
		activerBoutonsObjetSelectionne(objetSelectionne);
	}

	/**
	 * Active le bouton et desactive les autres.
	 * 
	 * @param objetSelectionne
	 */
	public void activerBoutonsObjetSelectionne(final boolean objetSelectionne)
	{
		final Enumeration<AbstractButton> boutons = groupeBoutons_.getElements();
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
	}
}
