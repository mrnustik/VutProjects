package net.mrnustik.university.solitaire.gui.panels;

import net.mrnustik.university.solitaire.collections.CardStack;

import net.mrnustik.university.solitaire.gui.view.CardView;
import net.mrnustik.university.solitaire.model.Card;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;

/**
 * Created by micha on 09.04.2017.
 */
public class CardStackPanel extends JLayeredPane implements ComponentListener {


    public interface CardSelected {
        void onCardSelected(Card card, int index);
    }

    private final int index;
    private CardStack stack;
    private CardSelected listener = null;

    public CardStackPanel(CardStack stack, int index) {
        super();
        addComponentListener(this);
        this.stack = stack;
        this.index = index;
    }

    public void setCardSelectedListener(CardSelected listener) {
        this.listener = listener;
    }

    void setStack(CardStack stack) {
        this.stack = stack;
        paintStack();
    }



    private void paintStack() {
        removeAll();
        int y = 0;
        for(int i = 0; i < stack.size(); i++)
        {
            final CardView cardView = new CardView(stack.get(i));
            cardView.setBounds(0,y, getSize().width, (getSize().width*240)/165);
            cardView.addActionListener((ActionEvent l) ->{
                if(listener != null){
                    listener.onCardSelected(cardView.getCard(), index);
                }
            });
            add(cardView, new Integer(i));

            y += 15;
        }
        revalidate();
        repaint();
    }


    @Override
    public void componentResized(ComponentEvent e) {
        paintStack();
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
}
