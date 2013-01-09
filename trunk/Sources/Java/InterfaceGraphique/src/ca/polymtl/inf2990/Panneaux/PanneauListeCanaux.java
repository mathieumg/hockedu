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
import java.util.ArrayList;

import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.ScrollPaneConstants;

import ca.polymtl.inf2990.Fenetre;
import ca.polymtl.inf2990.Composants.BoutonHockey;
import ca.polymtl.inf2990.Etats.EtatGererCanal;
import ca.polymtl.inf2990.Etats.EtatGererElement;
import ca.polymtl.inf2990.Etats.GestionnaireEtats;
import ca.polymtl.inf2990.GestionnaireImages.NomImage;
import ca.polymtl.inf2990.Jeu.CanalRadio;
import ca.polymtl.inf2990.Jeu.ElementVisitable;
import ca.polymtl.inf2990.Jeu.JoueurVirtuel;
import ca.polymtl.inf2990.Jeu.OperationsJoueursJNI;
import ca.polymtl.inf2990.Jeu.OperationsRadioJNI;


/**
 * Panneau qui liste tous les canaux de la radio.
 * @author Gabriel Couvrette
 *
 */
public class PanneauListeCanaux extends PanneauAbstrait
{
	
	JPanel	listePanel;
	JScrollPane centerPanel;
	ArrayList<ElementListe> listeDesElements;

	
	/**
	 * Construit le panneau.
	 * @param titre Le texte à afficher au haut de la fenêtre.
	 */
	public PanneauListeCanaux(String titre)
	{
		setLayout(new BorderLayout());
		JPanel topPanel = new JPanel();
		JPanel leftPanel = new JPanel();
		JPanel rightPanel = new JPanel();
		listePanel = new JPanel();
		
		centerPanel = new JScrollPane();
		centerPanel.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
		topPanel.setLayout(new FlowLayout(FlowLayout.CENTER));

		// panneau du haut
		JLabel titreComponent = new JLabel(titre);
		topPanel.add(titreComponent);
		add(topPanel, BorderLayout.NORTH);

		// panneau de gauche
		BoutonHockey boutonRetour = new BoutonHockey(NomImage.RETOUR_NORMAL, NomImage.RETOUR_ENFONCE, NomImage.RETOUR_SURVOLE, "");
		rightPanel.add(boutonRetour, FlowLayout.LEFT);
		boutonRetour.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
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

	}


	/**
	 * Charge les canaux par JNI et les affichent dans sa liste.
	 */
	public void chargerCanaux()
	{		
			ArrayList<Object> liste=OperationsRadioJNI.obtenirCanaux();
			listePanel.removeAll();
			//on place l'element "nouveau profil" sur le dessus de la liste.
			ElementListe nouveauProfil=new ElementListe(null);
			nouveauProfil.setText("** Nouveau canal **");
			nouveauProfil.setSize(new Dimension(listePanel.getWidth(), 40));
			nouveauProfil.setPreferredSize(new Dimension(listePanel.getWidth(), 40));
			nouveauProfil.setDeletable(false);
			listePanel.add(nouveauProfil);
			listePanel.add(Box.createRigidArea(new Dimension(listePanel.getWidth(), 5)));
			
			
			//on place le reste.
			int nbElems=0;
			if(liste!=null)
			nbElems = liste.size();
			
			for (int i = 0; i < nbElems; ++i)
			{
				ElementListe elem = new ElementListe((String)liste.get(i));
				elem.setSize(new Dimension(listePanel.getWidth(), 40));
				elem.setPreferredSize(new Dimension(listePanel.getWidth(), 40));
				listePanel.add(elem);
				listePanel.add(Box.createRigidArea(new Dimension(listePanel.getWidth(), 5)));
			}
			centerPanel.setViewportView(listePanel);
			if ((nbElems+1) * 40 < centerPanel.getHeight())
				listePanel.add(Box.createVerticalStrut(centerPanel.getHeight() - (nbElems+1) * 40));
	}
	
	
	/**
	 * Sous-élément représentant les items de la liste
	 * @author Gabriel Couvrette
	 *
	 */
	private class ElementListe extends JPanel
	{
		private String nomCanal_;
		JLabel componentNom_;
		BoutonHockey boutonSuppression_;
		
		public ElementListe(String nomJoueur)
		{
			nomCanal_ = nomJoueur;
			setBackground(Color.gray.darker());
			setPreferredSize(new Dimension(centerPanel.getWidth(), 40));
			setLayout(new BorderLayout());
			componentNom_ = new JLabel(nomJoueur);
			add(componentNom_, BorderLayout.WEST);
			JPanel tempPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
			boutonSuppression_ = new BoutonHockey(NomImage.SUPPRIMER_NORMAL, NomImage.SUPPRIMER_ENFONCE, NomImage.SUPPRIMER_SURVOLE, "");	
			boutonSuppression_.setPreferredSize(new Dimension(20, 20));
			boutonSuppression_.addMouseListener(new EcouteurSuppression());
			tempPanel.add(boutonSuppression_);
			add(tempPanel, BorderLayout.EAST);
			addMouseListener(new Ecouteur());
		}

		public String obtenirNomCanal()
		{
			return nomCanal_;

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
		 * @param text Le texte a ecrire.
		 */
		public void setText(String text)
		{
			componentNom_.setText(text);
		}
		
		/**
		 * Écouteur utile pour les éléments de la liste.
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
				String nomCanal=((ElementListe)(e.getSource())).obtenirNomCanal();
				CanalRadio canal=new CanalRadio();
				if(nomCanal!=null)
				{
				 canal=(CanalRadio)OperationsRadioJNI.obtenirCanal(nomCanal);
				}
				EtatGererCanal etat=new EtatGererCanal(canal);
				GestionnaireEtats.obtenirInstance().changerEtat(etat);
				Fenetre.rafraichirFenetre();
			}
		}
		
		/**
		 * Écouteur pour la suppression.
		 * @author Gabriel Couvrette
		 *
		 */
		private class EcouteurSuppression extends MouseAdapter
		{
			@Override
						public void mouseReleased(MouseEvent e)
						{
							if(((ElementListe)((BoutonHockey)(e.getSource())).getParent().getParent()).obtenirNomCanal()!=null)
							OperationsRadioJNI.supprimerCanal(((ElementListe)((BoutonHockey)(e.getSource())).getParent().getParent()).obtenirNomCanal());
							rafraichir();
							Fenetre.rafraichirFenetre();
						}			
		}
	}


	@Override
	protected void creer() {
	}
	

	@Override
	public void rafraichir() {
		chargerCanaux();
	}
	
}
