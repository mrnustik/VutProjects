package net.mrnustik.university.solitaire.gui.panels;

import net.mrnustik.university.solitaire.collections.CardStack;
import net.mrnustik.university.solitaire.gui.view.CardView;
import net.mrnustik.university.solitaire.model.Card;

import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by micha on 09.04.2017.
 */
public class CardStackPanel extends JLayeredPane implements ComponentListener {


    private final int index;
    private Timer timer;
    private CardStack stack;
    private CardSelected listener = null;

    private final List<CardView> cards;

    CardStackPanel(CardStack stack, int index) {
        super();
        addComponentListener(this);
        timer = new Timer(30, e -> paintStack());
        timer.setRepeats(false);
        this.stack = stack;
        this.index = index;
        this.cards = new ArrayList<>();
    }

    void setCardSelectedListener(CardSelected listener) {
        this.listener = listener;
    }

    void setStack(CardStack stack) {
        this.stack = stack;
        paintStack();
    }

    private void paintStack() {
        int y = 0;
        int index;
        if (stack.size() == 0) {
            index = 1;
            cards.get(0).changeCard(null);
        } else {
            for (index = 0; index < stack.size(); index++) {
                Card card = stack.get(index);
                CardView view;
                if (index < cards.size()) {
                    view = cards.get(index);
                } else {
                    view = new CardView(card);
                    cards.add(view);
                    add(view, new Integer(index));
                    view.addActionListener(l -> {
                        cardClicked(l.getSource());
                    });
                }
                view.setVisible(true);
                view.setCard(card);
                view.setBounds(0, y, getSize().width, (getSize().width * 240) / 165);
                if (card.isFaceUp())
                    y += 20;
                else
                    y += 5;
            }
        }
        if(index < cards.size()){
            for(; index < cards.size(); index++){
                CardView view = cards.get(index);
                view.setVisible(false);
            }
        }
    }

    private void cardClicked(Object source) {
        if(source instanceof CardView) {
            CardView view = (CardView) source;
            if(listener != null){
                listener.onCardSelected(view.getCard(), index);
            }
        }
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

    public void highlight(Card highlightCard) {
        boolean highlight = false;
        for (CardView card :
                this.cards) {

            Card realCard = card.getCard();
            if(realCard == null){
                highlight = true;
            } else if(realCard.equals(highlightCard)) {
                highlight = true;
            }
            if(highlight)
                card.highlight();
        }
    }

    public interface CardSelected {
        void onCardSelected(Card card, int index);
    }
}
