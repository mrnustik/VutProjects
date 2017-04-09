package net.mrnustik.university.solitaire.gui.panels;

import net.mrnustik.university.solitaire.collections.CardStack;
import net.mrnustik.university.solitaire.gui.layout.OverlapLayout;
import net.mrnustik.university.solitaire.gui.view.CardView;

import javax.swing.*;
import java.awt.*;

/**
 * Created by micha on 09.04.2017.
 */
public class CardStackPanel extends JPanel {


    private CardStack stack;

    public CardStackPanel(CardStack stack) {
        super(new OverlapLayout(new Point(0,20)));
        this.stack = stack;
    }

    public void setStack(CardStack stack) {
        this.stack = stack;
        paintStack();
    }


    private void paintStack() {
        removeAll();
        int y = 0;
        for(int i = 0; i < stack.size(); i++)
        {
            CardView cardView = new CardView(stack.get(i));
            cardView.setPreferredSize(new Dimension(getWidth(), (getWidth()/165) * 240));
            add(cardView);

            y += 20;
        }
        revalidate();
        repaint();
    }



}
