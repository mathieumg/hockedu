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
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JToggleButton;
import javax.swing.JToolBar;
import javax.swing.KeyStroke;
import javax.swing.border.Border;
import javax.swing.border.TitledBorder;

import ca.polymtl.inf2990.Fenetre;

/**
 * Barre contenant tout les outils de vue/sélection/transformation.
 * 
 * @author Charles Étienne Lalonde
 * 
 */
public class BarreOutils extends JToolBar
{
	/**
	 * Classe qui gère l'exécution d'une action sur un bouton.
	 * 
	 * @author Charles Étienne Lalonde
	 * 
	 */
	private class ActiverBouton extends AbstractAction
	{
		private static final long	serialVersionUID	= 1L;

		public ActiverBouton()
		{
		}

		/**
		 * Quoi faire quand une action est exécutée.
		 */
		@Override
		public void actionPerformed(final ActionEvent e)
		{
			final AbstractButton b = (AbstractButton) e.getSource();
			final Commandes commande = Commandes.valueOf(b.getActionCommand());
			if (!groupeBoutonsToggle_.isSelected(b.getModel()))
			{
				b.getModel().setSelected(true);
				b.doClick();
			}
			else if (commande == Commandes.SUPPRIMER || commande == Commandes.DUPLIQUER)
			{
				b.doClick();
			}

			Fenetre.obtenirInstance().obtenirCanevas().requestFocus();

		}
	}

	private static final long	serialVersionUID	= 1L;

	private final ButtonGroup	groupeBoutonsCamera_;
	private final ButtonGroup	groupeBoutonsNormaux_;
	private final ButtonGroup	groupeBoutonsToggle_;

	/**
	 * Construit une barre d'outils.
	 * 
	 * @param parent Le composant sur lequel on va ajouté la barre.
	 */
	public BarreOutils(final Component parent)
	{
		groupeBoutonsToggle_ = new ButtonGroup();
		groupeBoutonsNormaux_ = new ButtonGroup();
		groupeBoutonsCamera_ = new ButtonGroup();
		final Action activerBouton = new ActiverBouton();

		JToolBar panneauTypesCameras = new JToolBar();
		JLabel labelTemp = new JLabel();
		Border bordurePanneauTypesCameras = BorderFactory.createLineBorder(labelTemp.getForeground(), 2);
		bordurePanneauTypesCameras = BorderFactory.createTitledBorder(bordurePanneauTypesCameras, "Caméras", TitledBorder.LEADING, TitledBorder.DEFAULT_POSITION, new Font("Game Over", Font.ITALIC, 30));
		panneauTypesCameras.setBorder(bordurePanneauTypesCameras);

		// Ajout des boutons
		panneauTypesCameras.add(creerBouton(Commandes.CAMERA_FIXE, activerBouton));
		panneauTypesCameras.add(creerBouton(Commandes.CAMERA_ORBITE, activerBouton));
		panneauTypesCameras.add(creerBouton(Commandes.CAMERA_LIBRE, activerBouton));
		add(panneauTypesCameras);

		JToolBar panneauOptionsCameras = new JToolBar();
		Border bordurePanneauOptionsCameras = BorderFactory.createLineBorder(labelTemp.getForeground(), 2);
		bordurePanneauOptionsCameras = BorderFactory.createTitledBorder(bordurePanneauOptionsCameras, "Options Caméra", TitledBorder.LEADING, TitledBorder.DEFAULT_POSITION, new Font("Game Over", Font.ITALIC, 30));
		panneauOptionsCameras.setBorder(bordurePanneauOptionsCameras);

		panneauOptionsCameras.add(creerBouton(Commandes.CAMERA, activerBouton));
		panneauOptionsCameras.add(creerBouton(Commandes.ORBIT, activerBouton));
		panneauOptionsCameras.add(creerBouton(Commandes.ZOOM_PROPORTIONNEL, activerBouton));
		add(panneauOptionsCameras);

		JToolBar panneauEdition = new JToolBar();
		Border bordurePanneauEdition = BorderFactory.createLineBorder(labelTemp.getForeground(), 2);
		bordurePanneauEdition = BorderFactory.createTitledBorder(bordurePanneauEdition, "Édition d'objets", TitledBorder.LEADING, TitledBorder.DEFAULT_POSITION, new Font("Game Over", Font.ITALIC, 30));
		panneauEdition.setBorder(bordurePanneauEdition);

		panneauEdition.add(creerBouton(Commandes.EDITEUR_SELECTION, activerBouton));
		panneauEdition.add(creerBouton(Commandes.EDITEUR_DEPLACER, activerBouton));
		panneauEdition.add(creerBouton(Commandes.EDITEUR_ROTATION, activerBouton));
		panneauEdition.add(creerBouton(Commandes.EDITEUR_ECHELLE, activerBouton));

		panneauEdition.add(creerBouton(Commandes.SUPPRIMER, activerBouton));
		panneauEdition.add(creerBouton(Commandes.DUPLIQUER, activerBouton));

		add(panneauEdition);

		// Premier élément sélectionné par défaut.
		groupeBoutonsToggle_.setSelected(groupeBoutonsToggle_.getElements().nextElement().getModel(), true);
	}

	/**
	 * Ajoute un bouton à la barre.
	 * 
	 * @param commandeDuBouton La commande qu'on veut que le bouton exécute.
	 * @param activerBouton L'action a ajouter.
	 */
	public AbstractButton creerBouton(final Commandes commandeDuBouton, final Action activerBouton)
	{
		AbstractButton bouton;
		// Crée le bouton
		if (commandeDuBouton == Commandes.DUPLIQUER || commandeDuBouton == Commandes.SUPPRIMER)
		{
			bouton = new JButton();
			bouton.addActionListener(new ActionListener() {
				@Override
				public void actionPerformed(final ActionEvent e)
				{
					modifierSelection(commandeDuBouton);
				}
			});
			groupeBoutonsNormaux_.add(bouton);
		}
		else if (commandeDuBouton == Commandes.CAMERA_FIXE || commandeDuBouton == Commandes.CAMERA_ORBITE || commandeDuBouton == Commandes.CAMERA_LIBRE)
		{
			bouton = new JToggleButton();
			groupeBoutonsCamera_.add(bouton);
		}
		else
		{
			bouton = new JToggleButton();
			groupeBoutonsToggle_.add(bouton);
		}

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
			bouton.getInputMap(WHEN_IN_FOCUSED_WINDOW).put(KeyStroke.getKeyStroke(element), commandeDuBouton);
		}
		bouton.getActionMap().put(commandeDuBouton, activerBouton);

		if (commandeDuBouton.activerQuandObjetSelectionne())
		{
			bouton.setEnabled(false);
		}

		return bouton;

	}

	public void ajouterEcouteurAction(final ActionListener ecouteur)
	{
		AbstractButton bouton;

		Enumeration<AbstractButton> boutons = groupeBoutonsToggle_.getElements();
		while (boutons.hasMoreElements())
		{
			bouton = boutons.nextElement();
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
	}

	/**
	 * Sélectionne le bouton concerné par la commande passée en paramêtres.
	 * 
	 * @param commande La commande à sélectionner.
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

		boutons = groupeBoutonsToggle_.getElements();
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
			groupeBoutonsToggle_.clearSelection();
		}
	}

	public void rafraichirActivationBoutons(final boolean objetSelectionne, final boolean pointButSelectionne, final boolean pointPasButSelectionne)
	{
		activerBoutonsSelonEtatCamera();
		activerBoutonsObjetSelectionne(objetSelectionne);
		if (pointButSelectionne && !pointPasButSelectionne)
		{
			activerBoutonsPointButSelectionne();
		}
		else if (pointPasButSelectionne)
		{
			activerBoutonsPointPasButSelectionne();
		}
	}

	public void activerBoutonsSelonEtatCamera()
	{
		Enumeration<AbstractButton> boutons = groupeBoutonsToggle_.getElements();
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

	/**
	 * Active le bouton selectionne.
	 * 
	 * @param objetSelectionne si on veux l'activer ou le desactiver.
	 */
	public void activerBoutonsObjetSelectionne(final boolean objetSelectionne)
	{
		Enumeration<AbstractButton> boutons = groupeBoutonsToggle_.getElements();
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

		boutons = groupeBoutonsCamera_.getElements();
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

	public void activerBoutonsPointButSelectionne()
	{
		Enumeration<AbstractButton> boutons = groupeBoutonsToggle_.getElements();
		AbstractButton bouton;
		while (boutons.hasMoreElements())
		{
			bouton = boutons.nextElement();
			Commandes commande = Commandes.valueOf(bouton.getActionCommand());
			bouton.setEnabled(!commande.desactiverSiPointButSelectionne());
		}

		boutons = groupeBoutonsNormaux_.getElements();
		while (boutons.hasMoreElements())
		{
			bouton = boutons.nextElement();
			Commandes commande = Commandes.valueOf(bouton.getActionCommand());
			bouton.setEnabled(!commande.desactiverSiPointButSelectionne());
		}

		boutons = groupeBoutonsCamera_.getElements();
		while (boutons.hasMoreElements())
		{
			bouton = boutons.nextElement();
			Commandes commande = Commandes.valueOf(bouton.getActionCommand());
			bouton.setEnabled(!commande.desactiverSiPointButSelectionne());
		}
	}

	public void activerBoutonsPointPasButSelectionne()
	{
		Enumeration<AbstractButton> boutons = groupeBoutonsToggle_.getElements();
		AbstractButton bouton;
		while (boutons.hasMoreElements())
		{
			bouton = boutons.nextElement();
			Commandes commande = Commandes.valueOf(bouton.getActionCommand());
			bouton.setEnabled(!commande.desactiverSiPointPasButSelectionne());
		}

		boutons = groupeBoutonsNormaux_.getElements();
		while (boutons.hasMoreElements())
		{
			bouton = boutons.nextElement();
			Commandes commande = Commandes.valueOf(bouton.getActionCommand());
			bouton.setEnabled(!commande.desactiverSiPointPasButSelectionne());
		}

		boutons = groupeBoutonsCamera_.getElements();
		while (boutons.hasMoreElements())
		{
			bouton = boutons.nextElement();
			Commandes commande = Commandes.valueOf(bouton.getActionCommand());
			bouton.setEnabled(!commande.desactiverSiPointPasButSelectionne());
		}
	}
}
