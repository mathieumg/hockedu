package ca.polymtl.inf2990.Jeu;

import javax.swing.JPanel;

import ca.polymtl.inf2990.Panneaux.PanneauAbstrait;
import ca.polymtl.inf2990.Panneaux.VisiteurGenererPanneau;

/**
 * Pour le patron visiteur. Stipule que cet element peut etre consultee dans un panneau de creation/modification.
 * @author Gabriel Couvrette
 *
 */
public interface ElementVisitable {
	public PanneauAbstrait accepter(VisiteurGenererPanneau visiteur);
}
