package ca.polymtl.inf2990.Panneaux;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BoxLayout;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;

import ca.polymtl.inf2990.Composants.BoutonHockey;
import ca.polymtl.inf2990.Etats.GestionnaireEtats;
import ca.polymtl.inf2990.GestionnaireImages.NomImage;
import ca.polymtl.inf2990.Jeu.ElementVisitable;
import ca.polymtl.inf2990.Jeu.JoueurVirtuel;

/**
 * Panneau s'occupant d'afficher certaines informations et options selon le type d'objet qu'on lui demande de traiter (Visiteur)
 * @author Gabriel Couvrette
 *
 */
public class PanneauGestionElement extends PanneauAbstrait {

	JPanel panneauContenu_;
	JScrollPane centerPanel;
	ElementVisitable element_;

	
	/**
	 * Construit le panneau de base et donne un élément à la fenêtre afin de populer le panneau du centre.
	 * @param titre Le titre à afficher au haut du panneau.
	 * @param element L'élément à traiter.
	 */
	public PanneauGestionElement(String titre, ElementVisitable element) {
		element_ = element;
		setLayout(new BorderLayout());
		JPanel topPanel = new JPanel();
		JPanel leftPanel = new JPanel();
		JPanel rightPanel = new JPanel();
		panneauContenu_ = new JPanel();

		centerPanel = new JScrollPane();

		topPanel.setLayout(new FlowLayout(FlowLayout.CENTER));

		// panneau du haut
		JLabel titreComponent = new JLabel(titre);
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
		rightPanel.add(boutonRetour, FlowLayout.LEFT);
		add(rightPanel, BorderLayout.WEST);

		// panneau de droite:
		leftPanel.setPreferredSize(new Dimension(boutonRetour.getWidth(),
				getHeight()));
		add(leftPanel, BorderLayout.EAST);
		add(centerPanel, BorderLayout.CENTER);

		// panneau du centre
		panneauContenu_ = chargerElement(element);
		centerPanel.setViewportView(panneauContenu_);

	}

	/**
	 * Popule le panneau du centre selon l'élément à traiter.
	 * @param element L'élément à traiter.
	 * @return Le panneau construit selon l'élément requis (patron visiteur)
	 */
	public PanneauAbstrait chargerElement(ElementVisitable element) {
		if (element != null) {
			element_ = element;
			return new VisiteurGenererPanneau().visiterElement(element);
		}
		return null;
	}

	@Override
	protected void creer() {

	}

	@Override
	public void rafraichir() {
		PanneauAbstrait panneau = chargerElement(element_);
		centerPanel.setViewportView(panneau);
		panneau.rafraichir();
		
	}

}
