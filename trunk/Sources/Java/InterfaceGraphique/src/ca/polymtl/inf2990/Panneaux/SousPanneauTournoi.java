package ca.polymtl.inf2990.Panneaux;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dialog;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JComponent;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

import ca.polymtl.inf2990.Fenetre;
import ca.polymtl.inf2990.BarresOutils.Commandes;
import ca.polymtl.inf2990.BarresOutils.EvenementsSelectionneurFichier;
import ca.polymtl.inf2990.Etats.EtatJouerTournoi;
import ca.polymtl.inf2990.Etats.GestionnaireEtats;
import ca.polymtl.inf2990.Jeu.JoueurAbstrait;
import ca.polymtl.inf2990.Jeu.JoueurAbstrait.TypeJoueur;
import ca.polymtl.inf2990.Jeu.JoueurHumain;
import ca.polymtl.inf2990.Jeu.JoueurVirtuel;
import ca.polymtl.inf2990.Jeu.OperationsJoueursJNI;
import ca.polymtl.inf2990.Jeu.Tournoi;

/**
 * ATTENTION! CETTE CLASSE EST INUTILISABLE!
 * On la conserve uniquement pour reutiliser certaines parties de son code. Ce panneau en lui-meme n'est plus fonctionnel.
 * @author Gabriel Couvrette
 *
 */
public class SousPanneauTournoi extends PanneauAbstrait
{
	private ArrayList<JComboBox>	listeDesComboBox_;
	private JTextField				champSelectionTerrain_;
	private final String			texteParDefaut_	= "Cliquez ici pour sélectionner un terrain...";

	/**
	 * Crée le panneau selon un tournoi existant.
	 * @param tournoi Le tournoi de référence.
	 */
	public SousPanneauTournoi(final Tournoi tournoi)
	{
		if (tournoi.obtenirListeDesJoueurs() != null)
		{
			initialiserPanneau(tournoi.obtenirListeDesJoueurs());
		}
		else
		{
			initialiserPanneau(genererJoueursParDefaut(16));
		}
	}

	/**
	 * Crée les joueurs par défaut.
	 * @param nbJoueursInitial le nombre de joueurs initiaux.
	 * @return La liste des joueurs par défaut.
	 */
	public ArrayList<Object> genererJoueursParDefaut(final int nbJoueursInitial)
	{
		ArrayList<Object> joueurs = new ArrayList<Object>(nbJoueursInitial);
		for (int i = 0; i < nbJoueursInitial; ++i)
		{
			JoueurVirtuel joueur = new JoueurVirtuel("Défaut", 50, 50);
			joueurs.add(joueur);
		}
		return joueurs;
	}

	/**
	 * Crée un panneau de tournoi par défaut (tout les joueurs sont initialement
	 * le joueur par défaut)
	 * 
	 * @param nbJoueurs
	 */
	public SousPanneauTournoi(final int nbJoueursInitial)
	{
		initialiserPanneau(genererJoueursParDefaut(nbJoueursInitial));
	}

	private void initialiserPanneau(final ArrayList<Object> joueurs)
	{
		setLayout(new BorderLayout());
		Font police = new Font("Game Over", 0, 40);
		listeDesComboBox_ = new ArrayList<JComboBox>();
		JPanel panBase = new JPanel();
		EcouteurComboBox ecouteurComboBox = new EcouteurComboBox();
		int nbEtages = (int) Math.round(Math.log(joueurs.size()) / Math.log(2));
		panBase.setLayout(new GridLayout(1, nbEtages));

		JPanel panneauInitial = new JPanel(new BorderLayout());
		JPanel panneauJoueursInitiaux = new JPanel(new GridLayout(joueurs.size(), 1));
		ArrayList<Object> listeDesJoueurs = OperationsJoueursJNI.obtenirJoueurs();

		for (int i = 0; i < joueurs.size(); ++i)
		{
			JComboBox liste = new JComboBox();
			liste.setFont(police);
			liste.addItem(joueurs.get(i));
			liste.addItem(new JoueurHumain("*Joueur humain*"));
			for (int j = 0; j < listeDesJoueurs.size(); j++)
			{

				liste.addItem(OperationsJoueursJNI.obtenirJoueur((String) listeDesJoueurs.get(j)));
			}
			panneauJoueursInitiaux.add(liste);
			liste.addActionListener(ecouteurComboBox);
			listeDesComboBox_.add(liste);
		}

		panneauInitial.add(panneauJoueursInitiaux, BorderLayout.CENTER);
		panBase.add(panneauInitial);

		JPanel panneauBoutons = new JPanel(new FlowLayout(FlowLayout.CENTER));

		JButton boutonCancel = new JButton("Annuler");
		boutonCancel.setFont(police);
		boutonCancel.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(final ActionEvent e)
			{
				GestionnaireEtats.obtenirInstance().allerEtatPrecedent();
			}
		});

		JButton boutonOk = new JButton("Créer");
		boutonOk.setFont(police);
		boutonOk.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(final ActionEvent e)
			{
				actionOk();
			}
		});
		panneauBoutons.add(boutonOk);
		panneauBoutons.add(boutonCancel);

		for (int i = 0; i < nbEtages; ++i)
		{
			int nbElements = (int) Math.round(joueurs.size() / Math.pow(2, i));
			JPanel panneau2 = new JPanel(new BorderLayout());
			JPanel liste2 = new JPanel(new GridLayout(nbElements, 1));
			for (int j = 0; j < nbElements; ++j)
			{
				Separateur s = new Separateur(!(j % 2 == 0));
				liste2.add(s);
			}

			panneau2.add(liste2, BorderLayout.CENTER);

			panBase.add(panneau2);
			
			JPanel panneau = new JPanel(new BorderLayout());
			JPanel liste = new JPanel(new GridLayout(nbElements, 1));
			for (int j = 0; j < nbElements; ++j)
			{
				Separateur s = new Separateur(!(j % 2 == 0));
				liste.add(s);
			}

			panneau.add(liste, BorderLayout.CENTER);

			panBase.add(panneau);
		}			
			add(panBase, BorderLayout.CENTER);
			add(panneauBoutons, BorderLayout.SOUTH);

		champSelectionTerrain_ = new JTextField(texteParDefaut_);
		champSelectionTerrain_.setFont(police);
		champSelectionTerrain_.setEditable(false);
		champSelectionTerrain_.setColumns(30);
		champSelectionTerrain_.addFocusListener(new FocusAdapter() {
			@Override
			public void focusGained(final FocusEvent e)
			{
				e.getComponent().setFocusable(false);
				ActionEvent ev = new ActionEvent(e.getComponent(), ActionEvent.ACTION_FIRST, Commandes.EDITEUR_CHARGER.toString());
				EvenementsSelectionneurFichier.obtenirInstance().actionPerformed(ev);
				if (EvenementsSelectionneurFichier.obtenirDernierFichierSelectionne() != null)
				{
					if (GestionnaireEtats.obtenirInstance().validerTable())
					{
						JTextField element = ((JTextField) e.getComponent());
						element.setText(EvenementsSelectionneurFichier.obtenirDernierFichierSelectionne().getAbsolutePath());
						element.setCaretPosition(element.getText().length() - 1);
					}
				}
				requestFocus();
				e.getComponent().setFocusable(true);
			}
		});
		champSelectionTerrain_.setFocusable(true);

		panneauBoutons.add(champSelectionTerrain_);
	}

	/**
	 * Affiche la fenêtre demandant le nom du joueur humain à insérer dans la zone de joueur.
	 * @param source La zone dans laquelle inscrire la réponse du joueur.
	 */
	private void afficherFenetreNomHumain(final JComboBox source)
	{
		FenetreNomHumain fenetre = new FenetreNomHumain(source);
		fenetre.setBounds(getX(), getY(), 410,80);
		fenetre.pack();
		fenetre.setVisible(true);
	}

	/**
	 * Comportement du bouton OK
	 */
	private void actionOk()
	{
		if (!champValides())
		{
			JOptionPane.showMessageDialog(Fenetre.obtenirInstance(), "Certains choix de joueurs sont invalides.", "Joueurs invalides", JOptionPane.ERROR_MESSAGE);
			return;
		}

		String nom = "";
		do
		{
			nom = JOptionPane.showInputDialog(Fenetre.obtenirInstance(), "Entrez un nom pour ce tournoi : ");
		}
		while (!nomValide(nom));
		if (nom == null)
		{
			return;
		}
		Tournoi nouveauTournoi = new Tournoi();
		nouveauTournoi.setNom(nom);

		if (champSelectionTerrain_.getText().equals(texteParDefaut_))
		{
			GestionnaireEtats.obtenirInstance().genererTerrainParDefaut();

			// A conserver car la methode applique des variable importante
			// pour le jeu
			if (!GestionnaireEtats.obtenirInstance().validerTable())
			{
				return;
			}
		}
		else
		{
			nouveauTournoi.modifierTerrain(champSelectionTerrain_.getText());
		}

		ArrayList<Object> listeDesJoueurs = new ArrayList<Object>();
		for (int i = 0; i < listeDesComboBox_.size(); i++)
		{
			listeDesJoueurs.add(listeDesComboBox_.get(i).getSelectedItem());
		}

		nouveauTournoi.chargerJoueurs(listeDesJoueurs);

		OperationsJoueursJNI.creerNouveauTournoi(nouveauTournoi);

		GestionnaireEtats.obtenirInstance().changerEtat(new EtatJouerTournoi());
	}

	/**
	 * Valide l'entrée du joueur.
	 * @return La validité du nom.
	 */
	private boolean champValides()
	{
		for (int i = 0; i < listeDesComboBox_.size(); i++)
		{
			JoueurAbstrait joueurSelectionne = (JoueurAbstrait) listeDesComboBox_.get(i).getSelectedItem();
			if (joueurSelectionne == null)
			{
				return false;
			}
			if (joueurSelectionne.obtenirNom() == "*Joueur humain*")
			{
				return false;
			}
			if (joueurSelectionne.obtenirNom() == "")
			{
				return false;
			}
		}
		return true;
	}

	/**
	 * LA fenêtre qui demande le nom du joueur humain.
	 * @author Gabriel Couvrette
	 *
	 */
	private class FenetreNomHumain extends JDialog
	{
		JComboBox	componentSource_;
		JTextField	champNom_;
		JButton		boutonOk_, boutonCancel_;

		public FenetreNomHumain(final JComboBox source)
		{
			super(new JFrame(), "", Dialog.ModalityType.APPLICATION_MODAL);
			Font police = new Font("Game Over", Font.PLAIN, 40);
			componentSource_ = source;

			champNom_ = new JTextField(15);
			champNom_.setFont(police);
			boutonOk_ = new JButton("Ok");
			boutonOk_.setFont(police);
			boutonOk_.addActionListener(new ActionListener() {

				@Override
				public void actionPerformed(final ActionEvent e)
				{
					actionOk();
				}
			});

			boutonCancel_ = new JButton("Annuler");
			boutonCancel_.setFont(police);
			boutonCancel_.addActionListener(new ActionListener() {

				@Override
				public void actionPerformed(final ActionEvent e)
				{
					componentSource_.setSelectedIndex(-1);
					dispose();
				}
			});

			JPanel p = new JPanel(new FlowLayout());
			JLabel label = new JLabel("Nom : ");
			label.setFont(police);
			p.add(label);
			p.add(champNom_);
			p.add(boutonOk_);
			p.add(boutonCancel_);
			add(p);
		}

		private void actionOk()
		{
			if (nomValide(champNom_.getText()))
			{
				enleverJoueursHumains();
				JoueurHumain joueur = new JoueurHumain(champNom_.getText());
				componentSource_.addItem(joueur);
				componentSource_.setSelectedIndex(componentSource_.getItemCount() - 1);
				dispose();
			}
		}

		/**
		 * Parcours la liste et enleve les joueurs humains deja existants.
		 */
		private void enleverJoueursHumains()
		{
			int index = 0;
			int compte = componentSource_.getItemCount();
			for (int i = 0; i < compte; i++)
			{
				if (((JoueurAbstrait) componentSource_.getItemAt(index)).obtenirType() == TypeJoueur.HUMAIN && ((JoueurAbstrait) componentSource_.getItemAt(index)).obtenirNom() != "*Joueur humain*")
				{
					componentSource_.removeItemAt(index);
				}
				else
				{
					index++;
				}
			}
		}

	}

	/**
	 * Vérifie si le nom entré est valide. (En terme de caractères)
	 * 
	 * @param text Le nom à valider.
	 * @return La validité du nom.
	 */
	private boolean nomValide(final String text)
	{
		if (text == null)
		{
			return true;
		}

		if (text.length() == 0)
		{
			JOptionPane.showMessageDialog(Fenetre.obtenirInstance(), "Vous devez inscrire un nom!", "Nom invalide", JOptionPane.ERROR_MESSAGE);
			return false;
		}

		String reg = "[^(a-zA-Z0-9 )]";
		Pattern p = Pattern.compile(reg);
		Matcher m = p.matcher(text);
		if (m.find() || text.charAt(0) == ' ' || text.charAt(text.length() - 1) == ' ')
		{
			JOptionPane.showMessageDialog(Fenetre.obtenirInstance(), "Les caractères spéciaux et les espaces superflus sont interdits!", "Caractères interdits", JOptionPane.ERROR_MESSAGE);
			return false;
		}
		return true;
	}

	/**
	 * Composant dessinant les lignes noires des liens entre les parties.
	 * @author Gabriel Couvrette
	 *
	 */
	private class Separateur extends JComponent
	{
		boolean	inverse_;

		public Separateur(final boolean inverse)
		{
			inverse_ = inverse;
		}

		@Override
		protected void paintComponent(final Graphics g)
		{
			g.fillRect(0, (int) Math.round(getHeight() / 2.0) - 1, (int) Math.round(getWidth() / 2.0), 2);
			if (!inverse_)
			{
				g.fillRect((int) Math.round((getWidth() / 2.0) - 1.0), (int) Math.round(getHeight() / 2.0), 2, (int) Math.round(getHeight() / 2.0));
				g.fillRect((int) Math.round(getWidth() / 2.0), getHeight() - 1, (int) Math.round(getWidth() / 2.0), 1);
			}
			else
			{
				g.fillRect((int) Math.round(getWidth() / 2.0) - 1, 0, 2, (int) Math.round(getHeight() / 2.0));
				g.fillRect(getWidth() / 2, 0, getWidth() / 2, 1);
			}
		}
	}

	/**
	 * Ecouteur utile pour les listes déroulantes.
	 * @author Gabriel Couvrette
	 *
	 */
	private class EcouteurComboBox implements ActionListener
	{

		@Override
		public void actionPerformed(final ActionEvent e)
		{
			JoueurAbstrait joueurSelectionnee = (JoueurAbstrait) (((JComboBox) e.getSource()).getSelectedItem());
			if (joueurSelectionnee == null)
			{
				return;
			}
			if (joueurSelectionnee.obtenirNom().equals("*Joueur humain*"))
			{
				afficherFenetreNomHumain((JComboBox) e.getSource());
			}
		}

	}

	@Override
	protected void creer() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void rafraichir() {
		// TODO Auto-generated method stub
		
	}

}
