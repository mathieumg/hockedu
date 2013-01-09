package ca.polymtl.inf2990.Panneaux;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;

import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.ScrollPaneConstants;

import ca.polymtl.inf2990.Fenetre;
import ca.polymtl.inf2990.Composants.BoutonHockey;
import ca.polymtl.inf2990.Etats.EtatGererTournoi;
import ca.polymtl.inf2990.Etats.EtatNouveauTournoi;
import ca.polymtl.inf2990.Etats.GestionnaireEtats;
import ca.polymtl.inf2990.GestionnaireImages.NomImage;
import ca.polymtl.inf2990.Jeu.OperationTournoiJNI;
import ca.polymtl.inf2990.Jeu.Tournoi;


/**
 * Panneau qui liste tous les tournois existants
 * @author Gabriel Couvrette
 *
 */
public class PanneauListeTournois extends PanneauAbstrait
{
	
	JPanel	listePanel;
	JScrollPane centerPanel;
	ArrayList<ElementListe> listeDesElements;

	
	/**
	 * Construit le panneau.
	 * @param titre Le texte à afficher au haut de la fenêtre.
	 */
	public PanneauListeTournois(String titre)
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
	 * Charge les profils de joueurs par JNI et les affichent dans sa liste.
	 */
	public void chargerTournois()
	{		
			ArrayList<String> liste=OperationTournoiJNI.obtenirListeTournois();

			listePanel.removeAll();
			//on place l'element "nouveau profil" sur le dessus de la liste.
			ElementListe nouveauProfil=new ElementListe(null);
			nouveauProfil.setText("** Nouveau Tournoi **");
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
	 * Sous-élément représentant les items de la liste (la barre blanche contenant le nom ainsi que le bouton de suppression)
	 * @author Gabriel Couvrette
	 *
	 */
	private class ElementListe extends JPanel
	{
		private String nomTournoi_;
		JLabel componentNom_;
		BoutonHockey boutonSuppression_;
		
		public ElementListe(String nomTournoi)
		{
			nomTournoi_ = nomTournoi;
			setBackground(Color.gray.darker());
			setPreferredSize(new Dimension(centerPanel.getWidth(), 40));
			setLayout(new BorderLayout());
			componentNom_ = new JLabel(nomTournoi);
			add(componentNom_, BorderLayout.WEST);
			JPanel tempPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
			boutonSuppression_ = new BoutonHockey(NomImage.SUPPRIMER_NORMAL, NomImage.SUPPRIMER_ENFONCE, NomImage.SUPPRIMER_SURVOLE, "");	
			boutonSuppression_.setPreferredSize(new Dimension(20, 20));
			boutonSuppression_.addMouseListener(new EcouteurSuppression());
			tempPanel.add(boutonSuppression_);
			add(tempPanel, BorderLayout.EAST);
			addMouseListener(new Ecouteur());
		}

		/**
		 * Retorune le nom du tournoi contenu dans cet élément.
		 * @return Le nom.
		 */
		public String obtenirNomTournoi()
		{
			return nomTournoi_;

		}
		
		/**
		 * Définis si on doit afficher le bouton de suppression ou non.
		 */
		public void setDeletable(boolean b)
		{
			boutonSuppression_.setVisible(b);
		}
		
		/**
		 * Dis si l'élément peut être supprimé.
		 * @return si l'élément peut être supprimé
		 */
		public boolean estDeletable()
		{
			return boutonSuppression_.isVisible();
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
				ElementListe elementTriggered=(ElementListe)(e.getSource());
				if(!elementTriggered.estDeletable())
				{
					GestionnaireEtats.obtenirInstance().changerEtat(new EtatNouveauTournoi());
					return;
				}
				
				String nomTournoi=elementTriggered.obtenirNomTournoi();
				Tournoi tournoi=new Tournoi();
				if(nomTournoi!=null)
				{
					tournoi=(Tournoi)OperationTournoiJNI.obtenirTournoi(nomTournoi);
					if(tournoi==null)
					{
						JOptionPane.showMessageDialog(Fenetre.obtenirInstance(), "Ce tournoi est corrompu.");
						return;
					}
				}
				
				EtatGererTournoi etat=new EtatGererTournoi(tournoi);
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
							if(((ElementListe)((BoutonHockey)(e.getSource())).getParent().getParent()).obtenirNomTournoi()!=null)
							OperationTournoiJNI.supprimerTournoi(((ElementListe)((BoutonHockey)(e.getSource())).getParent().getParent()).obtenirNomTournoi());
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
		chargerTournois();
	}
	
}
