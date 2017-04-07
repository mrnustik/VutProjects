package net.mrnustik.university.solitaire.gui.panels;

import net.mrnustik.university.solitaire.factory.SolitaireFactory;
import net.mrnustik.university.solitaire.gui.view.CardView;
import net.mrnustik.university.solitaire.model.Board;
import net.mrnustik.university.solitaire.model.Card;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

/**
 * Created by mrnda on 4/7/2017.
 */
public class GamePanel extends JPanel {
    private Board board;

    private CardView deck;
    private CardView stacker;
    private CardView[] targets;

    private Card selected = null;

    public GamePanel() {
        super(new GridLayout(2,6));
        board = new Board(new SolitaireFactory());
        setMinimumSize(new Dimension(200,150));
        setBackground(Color.decode("#146a2c"));
        initViews();
        paintBoard();

    }

    private void initViews() {
        deck = new CardView(board.getDeckTop());
        add(deck);
        stacker = new CardView(board.getStackTop());
        add(stacker);
        deck.addActionListener(l->{
            board.flipFromDeck();
            paintBoard();
        });

        stacker.addActionListener(l->{
            selected = stacker.getCard();
        });

        JPanel separator = new JPanel();
        separator.setBackground(Color.decode("#146a2c"));
        add(separator);

        targets = new CardView[4];
        for(int i = 0; i < 4; i++)
        {
            final int index = i;
            targets[i] = new CardView(null);
            targets[i].addActionListener(l->{
                if(selected != null)
                {
                    board.fromStackerToTarget(index);
                }
                paintBoard();
            });

            add(targets[i]);
        }



        for(int i = 0; i < 7; i++)
        {
            add(new CardView(board.getStackTop()));
        }
    }

    private void paintBoard() {
        deck.changeCard(board.getDeckTop());
        stacker.changeCard(board.getStackTop());

        for(int i = 0; i < 4; i++) {
            targets[i].changeCard(board.getTargetTop(i));
        }

        revalidate();
        repaint();
    }
}
