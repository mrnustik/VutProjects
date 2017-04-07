package net.mrnustik.university.solitaire.gui.panels;

import net.mrnustik.university.solitaire.factory.SolitaireFactory;
import net.mrnustik.university.solitaire.model.Board;

import javax.swing.*;

/**
 * Created by mrnda on 4/7/2017.
 */
public class GamePanel extends JPanel {
    private Board board;

    public GamePanel() {
        super();
        board = new Board(new SolitaireFactory());

    }
}
