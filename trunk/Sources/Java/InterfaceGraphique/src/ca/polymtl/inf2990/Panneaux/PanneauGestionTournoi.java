package ca.polymtl.inf2990.Panneaux;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.Graphics;
import java.awt.GridLayout;
import java.awt.Point;
import java.awt.Polygon;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;

import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.JTextField;

import ca.polymtl.inf2990.Fenetre;
import ca.polymtl.inf2990.OpenGL;
import ca.polymtl.inf2990.Composants.BoutonHockey;
import ca.polymtl.inf2990.Etats.EtatJouerTournoi;
import ca.polymtl.inf2990.Etats.EtatMenuPrincipal;
import ca.polymtl.inf2990.Etats.GestionnaireEtats;
import ca.polymtl.inf2990.GestionnaireImages.NomImage;
import ca.polymtl.inf2990.Jeu.Tournoi;

/**
 *  Panneau affiché lorsqu'on veut gérer un tournoi existant, et visualiser ses paramètres avant de jouer le tournoi.
 * @author Gabriel Couvrette
 *
 */
public class PanneauGestionTournoi extends PanneauAbstrait
{

	Tournoi tournoi_;
	JPanel panneauContenu_;
	JPanel panneauVainqueursEtCarte;
	JPanel panneauVainqueurs;
	ElementTable elementTable;
	JScrollPane centerPanel;
	JLabel titreComponent;
	JScrollPane panneauVainqueursScroll;	
	JPanel panneauTable;
	
	/**
	 * Réinitialise le tournoi par JNI.
	 */
	public native void reinitialiserTournoi();
	
	@Override
	protected void creer() 
	{
		setLayout(new BorderLayout());
		JPanel centerSplitterPane=new JPanel(new BorderLayout());
		JPanel topPanel = new JPanel();
		JPanel leftPanel = new JPanel();
		JPanel rightPanel = new JPanel();
		panneauContenu_ = new JPanel();
		panneauVainqueursEtCarte = new JPanel();
		
		centerPanel = new JScrollPane();

		topPanel.setLayout(new FlowLayout(FlowLayout.CENTER));

		// panneau du haut
		titreComponent = new JLabel("");
		titreComponent.setForeground(Color.white);
		topPanel.add(titreComponent);
		add(topPanel, BorderLayout.NORTH);

		// panneau de gauche
		BoutonHockey boutonRetour = new BoutonHockey(NomImage.RETOUR_NORMAL,
				NomImage.RETOUR_ENFONCE, NomImage.RETOUR_SURVOLE, "");
		boutonRetour.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent e) {
				GestionnaireEtats.obtenirInstance().allerEtatPrecedent();
			}
		});
		
		add(rightPanel, BorderLayout.EAST);

		// panneau de droite:
		leftPanel.setPreferredSize(new Dimension(boutonRetour.getWidth(),
				getHeight()));

		leftPanel.add(boutonRetour, FlowLayout.LEFT);
		add(leftPanel, BorderLayout.WEST);
		
		centerSplitterPane.add(centerPanel,BorderLayout.CENTER);
		add(centerSplitterPane, BorderLayout.CENTER);
		
		//creation du panneau du bas.
		panneauVainqueursEtCarte.setLayout(new BorderLayout());
		panneauVainqueursScroll=new JScrollPane();		
		panneauVainqueurs=new JPanel();		
		panneauTable=new JPanel();
		
		panneauVainqueursScroll.setViewportView(panneauVainqueurs);
		
		panneauVainqueursEtCarte.add(panneauTable,BorderLayout.EAST);
		panneauVainqueursEtCarte.add(panneauVainqueursScroll,BorderLayout.CENTER);
		centerSplitterPane.add(panneauVainqueursEtCarte,BorderLayout.SOUTH);
		
		// panneau du centre
		centerPanel.setViewportView(panneauContenu_);
		
		//bouton jouer
		BoutonHockey boutonJouer = new BoutonHockey(NomImage.BOUTON_NORMAL, NomImage.BOUTON_ENFONCE, NomImage.BOUTON_SURVOLE, "JOUER !");
		boutonJouer.setPreferredSize(new Dimension(100,54));
		boutonJouer.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				//ici, lancer le tournoi dans le cpp.
				final int optionChoisie = JOptionPane.showOptionDialog(getParent(), "Voulez vous reinitialiser le tournoi ou continuer à la dernière partie jouée?", "Confirmation", JOptionPane.YES_NO_OPTION, JOptionPane.INFORMATION_MESSAGE, null, new String[] { "Réinitialiser", "Continuer", "Annuler" }, "Annuler");
				if (optionChoisie == 0)
				{
					reinitialiserTournoi();
					GestionnaireEtats.obtenirInstance().changerEtat(new EtatJouerTournoi());
				}
				else if (optionChoisie == 1)
				{
					GestionnaireEtats.obtenirInstance().changerEtat(new EtatJouerTournoi());
				}			
			}
		});
		rightPanel.add(boutonJouer);
		
		centerPanel.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
		panneauVainqueursScroll.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_NEVER);
	}

	@Override
	public void rafraichir() {
		// TODO Auto-generated method stub
		
	}
	
	/**
	 * Charge le panneau selon les informations sur le tournoi fourni en paramètres.
	 * @param tournoi LE tournoi de référence.
	 */
	public void chargerTournoi(Tournoi tournoi)
	{
		tournoi_=tournoi;
		////////////////////////////////////////////
		//	GENERATION DU NOM DU TOURNOI 	 	  //
		////////////////////////////////////////////
		titreComponent.setText(tournoi_.obtenirNom());
		////////////////////////////////////////////
		//	GENERATION DU PREVIEW DE LA TABLE 	  //
		////////////////////////////////////////////
		
		
		
		
		
		////////////////////////////////////////////
		//	GENERATION DU PREVIEW DE LA TABLE 	  //
		////////////////////////////////////////////
		double[] tableX=new double[tournoi_.obtenirPointsTable().size()];
		double[] tableY=new double[tournoi_.obtenirPointsTable().size()];
		
		int ordre[]={0,1,2,7,3,6,5,4};
		for(int i=0;i<tournoi_.obtenirPointsTable().size();i++)
		{
			tableX[ordre[i]]=(((Point)tournoi_.obtenirPointsTable().get(i)).getX()+250)/500.0;
			tableY[ordre[i]]=((((Point)tournoi_.obtenirPointsTable().get(i)).getY()+250)/500.0);
		}
		panneauTable.removeAll();
		elementTable=new ElementTable(tableX, tableY);
		elementTable.setSize(new Dimension(200, 200));
		elementTable.setPreferredSize(new Dimension(200, 200));
		panneauTable.add(elementTable);
		////////////////////////////////////////////
		//	GENERATION DU PREVIEW DE LA TABLE 	  //
		////////////////////////////////////////////
		

		////////////////////////////////////////////
		//	GENERATION DE LA LISTE DES JOUEURS 	  //
		////////////////////////////////////////////
		ArrayList<Object> joueursInitiaux = tournoi_.obtenirListeDesJoueurs();

		panneauContenu_.removeAll();
		panneauContenu_.setLayout(new GridLayout(17,1));
		JLabel l1 = new JLabel("Liste des joueurs initiaux : ");
		l1.setForeground(Color.WHITE);
		panneauContenu_.add(l1);		
		for(int i=0; i<joueursInitiaux.size();++i)
		{
			JTextField joueurInit = new JTextField((String)(joueursInitiaux.get(i)));
			joueurInit.setEditable(false);
			joueurInit.setPreferredSize(new Dimension(panneauVainqueursEtCarte.getWidth()-10,30));
			panneauContenu_.add(joueurInit);
		}
		centerPanel.setViewportView(panneauContenu_);
		////////////////////////////////////////////
		//	GENERATION DE LA LISTE DES JOUEURS 	  //
		////////////////////////////////////////////
		
		
		////////////////////////////////////////////
		//	GENERATION DE LA LISTE DES GAGNANTS	  //
		////////////////////////////////////////////
		ArrayList<Object> joueursGagnants = tournoi_.obtenirVainqueurs();

		panneauVainqueurs.removeAll();
		panneauVainqueurs.setLayout(new GridLayout((1+joueursGagnants.size()),1));
		
		JLabel l2 = new JLabel("Liste des anciens vainqueur : ");
		l2.setForeground(Color.WHITE);
		panneauVainqueurs.add(l2);	
		for(int i=0; i<joueursGagnants.size();++i)
		{
			JTextField joueur = new JTextField((String)(joueursGagnants.get(i)));
			joueur.setEditable(false);
			joueur.setPreferredSize(new Dimension(panneauVainqueursEtCarte.getWidth()-elementTable.getWidth()-10,30));
			panneauVainqueurs.add(joueur);
		}

		panneauVainqueurs.setPreferredSize(new Dimension(panneauVainqueursEtCarte.getWidth()-elementTable.getWidth(),30*joueursGagnants.size()));
		panneauVainqueursScroll.setPreferredSize(new Dimension(panneauVainqueursEtCarte.getWidth()-elementTable.getWidth(), this.getHeight()/3));
		panneauVainqueursScroll.setViewportView(panneauVainqueurs);
		////////////////////////////////////////////
		//	GENERATION DE LA LISTE DES GAGNANTS	  //
		////////////////////////////////////////////
	}
	
	/**
	 * Élément représentant un preview de la table sur laquelle le tournoi va prendre place.
	 * @author Gabriel Couvrette
	 *
	 */
private class ElementTable extends JComponent
{
	double[] polygoneTableX;
	double[] polygoneTableY;
	
	public ElementTable(double[] pointsX, double[] pointsY) {
		polygoneTableX=pointsX;
		polygoneTableY=pointsY;
	}
	@Override
	protected void paintComponent(Graphics g) {
		int[] tableX=new int[8];
		int[] tableY=new int[8];
		for(int i=0;i<polygoneTableX.length;++i)
		{
			tableX[i] = (int)(polygoneTableX[i]*getWidth());
			tableY[i] = (int)((1-polygoneTableY[i])*getHeight());
		}
		g.setColor(Color.blue);
		g.fillPolygon(tableX, tableY, tableX.length);
		g.setColor(Color.RED);
		g.drawPolygon(tableX, tableY, tableX.length);
	}
}
}
