package ca.polymtl.inf2990.Panneaux;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.awt.Scrollbar;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.io.File;
import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.JTextField;
import javax.swing.ScrollPaneConstants;
import javax.swing.filechooser.FileFilter;

import ca.polymtl.inf2990.Fenetre;
import ca.polymtl.inf2990.Composants.BoutonHockey;
import ca.polymtl.inf2990.Etats.EtatGererElement;
import ca.polymtl.inf2990.Etats.GestionnaireEtats;
import ca.polymtl.inf2990.GestionnaireImages.NomImage;
import ca.polymtl.inf2990.Jeu.CanalRadio;
import ca.polymtl.inf2990.Jeu.ElementVisitable;
import ca.polymtl.inf2990.Jeu.JoueurVirtuel;
import ca.polymtl.inf2990.Jeu.OperationsJoueursJNI;
import ca.polymtl.inf2990.Jeu.OperationsRadioJNI;

/**
 * Panneau qui donne accès aux fonctions de création/modification d'un canal.
 * 
 * @author Gabriel Couvrette
 * 
 */
public class PanneauGestionCanal extends PanneauAbstrait
{

	JPanel					listePanel;
	JScrollPane				centerPanel;
	ArrayList<ElementListe>	listeDesElements;
	JTextField				titreComponent_;
	CanalRadio				canal_;

	/**
	 * Construit le panneau.
	 * 
	 * @param titre Le texte à afficher au haut de la fenêtre.
	 */
	public PanneauGestionCanal(CanalRadio canal)
	{
		canal_ = canal;
		setLayout(new BorderLayout());
		JPanel topPanel = new JPanel();
		JPanel leftPanel = new JPanel();
		JPanel rightPanel = new JPanel();
		listePanel = new JPanel();

		centerPanel = new JScrollPane();
		centerPanel.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
		topPanel.setLayout(new FlowLayout(FlowLayout.CENTER));

		// panneau du haut
		String nomDuCanal = "";
		if (canal_ != null)
			nomDuCanal = canal_.obtenirNomDuCanal();
		titreComponent_ = new JTextField(nomDuCanal, 20);
		topPanel.add(new JLabel("Nom du canal : "));
		topPanel.add(titreComponent_);
		add(topPanel, BorderLayout.NORTH);

		// panneau de gauche
		BoutonHockey boutonRetour = new BoutonHockey(NomImage.RETOUR_NORMAL, NomImage.RETOUR_ENFONCE, NomImage.RETOUR_SURVOLE, "");
		rightPanel.add(boutonRetour, FlowLayout.LEFT);
		boutonRetour.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e)
			{
				GestionnaireEtats.obtenirInstance().allerEtatPrecedent();
			}
		});
		add(rightPanel, BorderLayout.WEST);

		// panneau de droite:
		leftPanel.setPreferredSize(new Dimension(boutonRetour.getWidth(), getHeight()));
		add(leftPanel, BorderLayout.EAST);
		add(centerPanel, BorderLayout.CENTER);

		listePanel.setLayout(new BoxLayout(listePanel, BoxLayout.Y_AXIS));
		// panneau du centre
		EcouteurBoutons ecouteurBoutons = new EcouteurBoutons();
		JPanel panneauDuBas = new JPanel(new FlowLayout(FlowLayout.CENTER));
		JButton boutonOk = new JButton("Ok"), boutonCancel = new JButton("Annuler");
		boutonOk.setActionCommand("OK");
		boutonCancel.setActionCommand("ANNULER");
		panneauDuBas.add(boutonOk);
		panneauDuBas.add(boutonCancel);
		boutonOk.addActionListener(ecouteurBoutons);
		boutonCancel.addActionListener(ecouteurBoutons);
		add(panneauDuBas, BorderLayout.SOUTH);
	}

	/**
	 * Charge un canal dans le panneau.
	 * @param canal
	 */
	public void setCanal(CanalRadio canal)
	{
		canal_ = canal;
	}

	/**
	 * Charge le profil du canal.
	 */
	public void chargerCanal()
	{
		ArrayList<Object> liste = null;
		listeDesElements = new ArrayList<PanneauGestionCanal.ElementListe>();
		if (canal_ != null)
		{
			liste = canal_.obtenirListeDesChansons();
			titreComponent_.setText(canal_.obtenirNomDuCanal());
		}
		else
		{
			liste = new ArrayList<Object>();
		}

		listePanel.removeAll();
		// on place l'element "nouveau profil" sur le dessus de la liste.
		ElementListe nouveauProfil = new ElementListe(null);
		nouveauProfil.setText("** Nouvelle chanson **");
		nouveauProfil.setSize(new Dimension(listePanel.getWidth(), 40));
		nouveauProfil.setPreferredSize(new Dimension(listePanel.getWidth(), 40));
		nouveauProfil.setDeletable(false);
		listeDesElements.add(nouveauProfil);
		listePanel.add(nouveauProfil);
		listePanel.add(Box.createRigidArea(new Dimension(listePanel.getWidth(), 5)));

		// on place le reste.
		int nbElems=0;
		if(liste!=null)
		nbElems = liste.size();
		for (int i = 0; i < nbElems; ++i)
		{
			ElementListe elem = new ElementListe(new File((String) liste.get(i)));
			elem.setSize(new Dimension(listePanel.getWidth(), 40));
			elem.setPreferredSize(new Dimension(listePanel.getWidth(), 40));
			listePanel.add(elem);
			listeDesElements.add(elem);
			listePanel.add(Box.createRigidArea(new Dimension(listePanel.getWidth(), 5)));
		}
		centerPanel.setViewportView(listePanel);
		if ((nbElems + 1) * 40 < centerPanel.getHeight())
			listePanel.add(Box.createVerticalStrut(centerPanel.getHeight() - (nbElems + 1) * 40));
	}

	/**
	 * Ecouteur pour les boutons ok/cancel.
	 * 
	 * @author Gabriel Couvrette
	 * 
	 */
	private class EcouteurBoutons implements ActionListener
	{

		@Override
		public void actionPerformed(ActionEvent e)
		{
			String action = e.getActionCommand();
			if (action == "OK")
				actionOk();
			else
				actionCancel();

		}

	}

	/**
	 * Vérifie si le nom entré est valide. (En terme de caractères)
	 * 
	 * @param text Le nom à valider.
	 * @return La validité du nom.
	 */
	private boolean nomValide(String text)
	{
		if (text.length() == 0)
		{
			JOptionPane.showMessageDialog(Fenetre.obtenirInstance(), "Vous devez inscrire un nom!");
			return false;
		}

		String reg = "[^(a-zA-Z0-9 )]";
		Pattern p = Pattern.compile(reg);
		Matcher m = p.matcher(text);
		if (m.find() || text.charAt(0) == ' ' || text.charAt(text.length() - 1) == ' ')
		{
			JOptionPane.showMessageDialog(Fenetre.obtenirInstance(), "Les caractères spéciaux et les espaces superflues sont interdits!");
			return false;
		}
		return true;
	}

	/**
	 * Quoi faire quand on pese sur ok.
	 */
	private void actionOk()
	{
		if (!nomValide(titreComponent_.getText()))
		{
			return;
		}
		ArrayList<Object> chansons = new ArrayList<Object>();
		for (int i = 0; i < listeDesElements.size(); i++)
		{
			try
			{
				chansons.add(listeDesElements.get(i).obtenirNomChanson().getPath());
			}
			catch (Exception ex)
			{
			}
		}
		canal_ = new CanalRadio();
		canal_.assignerNomDuCanal(titreComponent_.getText());
		canal_.assignerListeDesChansons(chansons);
		OperationsRadioJNI.genererCanal(canal_);
		actionCancel();
	}

	/**
	 * Quoi faire quand on pese sur cancel
	 * 
	 */
	private void actionCancel()
	{
		titreComponent_.setText(null);
		listeDesElements = null;
		GestionnaireEtats.obtenirInstance().allerEtatPrecedent();
	}

	/**
	 * Sous-élément représentant les items de la liste (la barre blanche
	 * contenant le nom ainsi que le bouton de suppression)
	 * 
	 * @author Gabriel Couvrette
	 * 
	 */
	private class ElementListe extends JPanel
	{
		private File	nomChanson_;
		JLabel			componentNom_;
		BoutonHockey	boutonSuppression_;

		public ElementListe(File nomChanson)
		{
			setBackground(Color.gray.darker());
			if (nomChanson != null)
			{
				nomChanson_ = nomChanson;
				setPreferredSize(new Dimension(centerPanel.getWidth(), 40));
				setLayout(new BorderLayout());
				componentNom_ = new JLabel(nomChanson.getName());
				add(componentNom_, BorderLayout.WEST);
				JPanel tempPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
				boutonSuppression_ = new BoutonHockey(NomImage.SUPPRIMER_NORMAL, NomImage.SUPPRIMER_ENFONCE, NomImage.SUPPRIMER_SURVOLE, "");
				boutonSuppression_.setPreferredSize(new Dimension(20, 20));
				boutonSuppression_.addMouseListener(new EcouteurSuppression());
				tempPanel.add(boutonSuppression_);
				add(tempPanel, BorderLayout.EAST);
				addMouseListener(new Ecouteur());
			}
			else
			{
				nomChanson_ = null;
				setPreferredSize(new Dimension(centerPanel.getWidth(), 40));
				setLayout(new BorderLayout());
				componentNom_ = new JLabel();
				add(componentNom_, BorderLayout.WEST);
				JPanel tempPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
				boutonSuppression_ = new BoutonHockey(NomImage.SUPPRIMER_NORMAL, NomImage.SUPPRIMER_ENFONCE, NomImage.SUPPRIMER_SURVOLE, "");
				boutonSuppression_.setPreferredSize(new Dimension(20, 20));
				boutonSuppression_.addMouseListener(new EcouteurSuppression());
				tempPanel.add(boutonSuppression_);
				add(tempPanel, BorderLayout.EAST);
				addMouseListener(new Ecouteur());
			}
		}

		/**
		 * Retourne le nom de la chanson ciontenue dans cet element.
		 * @return Un FILE contenant les infos sur la chanson.
		 */
		public File obtenirNomChanson()
		{
			return nomChanson_;

		}

		/**
		 * Définis si on doit afficher le bouton de suppression ou non.
		 */
		public void setDeletable(boolean b)
		{
			boutonSuppression_.setVisible(b);
		}

		/**
		 * Change le texte de l'elementListe.
		 * 
		 * @param text Le texte a ecrire.
		 */
		public void setText(String text)
		{
			componentNom_.setText(text);
		}

		/**
		 * Écouteur utile pour les éléments de la liste.
		 * 
		 * @author Gabriel Couvrette
		 * 
		 */
		private class Ecouteur extends MouseAdapter
		{
			@Override
			public void mouseEntered(MouseEvent e)
			{
				setBackground(new Color(191, 209, 251));
			}

			@Override
			public void mouseExited(MouseEvent e)
			{
				setBackground(Color.gray.darker());
			}

			@Override
			public void mousePressed(MouseEvent e)
			{
				ElementListe elementSelectionne = (ElementListe)e.getSource();
				JFileChooser jfc = new JFileChooser("./");
				int choix = jfc.showOpenDialog(Fenetre.obtenirInstance());
				if(choix==JFileChooser.APPROVE_OPTION)
				{
					File fichierChoisi = jfc.getSelectedFile();
					if(fichierChoisi!=null&&jfc.accept(fichierChoisi))
					{
						if(elementSelectionne.estSupprimable())
						{
						canal_.retirerChanson(elementSelectionne.obtenirNomChanson().getPath());
						canal_.ajouterChanson(fichierChoisi.getPath());
						}
						else
						{
							canal_.ajouterChanson(fichierChoisi.getPath());
						}
					}
				}
				Fenetre.rafraichirFenetre();
			}
		}

		/**
		 * Écouteur pour la suppression.
		 * 
		 * @author Gabriel Couvrette
		 * 
		 */
		private class EcouteurSuppression extends MouseAdapter
		{
			@Override
			public void mouseReleased(MouseEvent e)
			{
				File nomChanson = ((ElementListe) ((BoutonHockey) (e.getSource())).getParent().getParent()).obtenirNomChanson();
				if (nomChanson != null)
				{
					for (int i = 1; i < listeDesElements.size(); i++)
					{
						if (listeDesElements.get(i).obtenirNomChanson().equals(nomChanson))
							listeDesElements.remove(i);
					}
				}
				canal_.retirerChanson(nomChanson.getPath());
				Fenetre.rafraichirFenetre();
		}
		}

		public boolean estSupprimable() {
			return boutonSuppression_.isVisible();
		}
	}

	@Override
	protected void creer()
	{
	}

	@Override
	public void rafraichir()
	{
		chargerCanal();
	}

}
