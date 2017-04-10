package net.mrnustik.university.solitaire.gui.panels;

import net.mrnustik.university.solitaire.collections.CardStack;
import net.mrnustik.university.solitaire.gui.view.CardView;
import net.mrnustik.university.solitaire.model.Card;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;

/**
 * Created by micha on 09.04.2017.
 */
public class CardStackPanel extends JLayeredPane implements ComponentListener {


    private final int index;
    private Timer timer;
    private CardStack stack;
    private CardSelected listener = null;

    CardStackPanel(CardStack stack, int index) {
        super();
        addComponentListener(this);
        timer = new Timer(30, e -> paintStack());
        timer.setRepeats(false);
        this.stack = stack;
        this.index = index;
    }

    void setCardSelectedListener(CardSelected listener) {
        this.listener = listener;
    }

    void setStack(CardStack stack) {
        this.stack = stack;
        paintStack();
    }

    private void paintStack() {
        removeAll();
        int y = 0;
        if (stack.size() == 0) {
            addCard(0, 0, null);
        }
        for (int index = 0; index < stack.size(); index++) {
            Card card = stack.get(index);
            addCard(y, index, card);
            if (card.isFaceUp())
                y += 20;
            else
                y += 5;
        }
        revalidate();
        repaint();
    }

    private void addCard(int y, int i, Card card) {
        final CardView cardView = new CardView(card);
        cardView.setBounds(0, y, getSize().width, (getSize().width * 240) / 165);
        cardView.addActionListener((ActionEvent l) -> {
            if (listener != null) {
                listener.onCardSelected(cardView.getCard(), index);
            }
        });
        add(cardView, new Integer(i));
    }

    @Override
    public void componentResized(ComponentEvent e) {
        if (timer.isRunning())
            timer.restart();
        else
            timer.start();
    }

    @Override
    public void componentMoved(ComponentEvent e) {

    }

    @Override
    public void componentShown(ComponentEvent e) {

    }

    @Override
    public void componentHidden(ComponentEvent e) {

    }

    public interface CardSelected {
        void onCardSelected(Card card, int index);
    }
}
