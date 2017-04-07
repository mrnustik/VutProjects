package net.mrnustik.university.solitaire.gui.view;

import net.mrnustik.university.solitaire.model.Card;

import javax.swing.*;
import java.net.URL;

/**
 * Created by mrnda on 07/04/2017.
 */
public class CardView extends JButton {

    private Card mCard;

    public CardView(Card card) {
        this.mCard = card;
    }

    @Override
    public Icon getIcon() {
        URL resource = null;

        if(mCard.isFaceUp()) {
            resource = getClass().getResource("/net/mrnustik/university/solitaire/gui/images/" + mCard.toString() + ".png");
        } else {
            resource = getClass().getResource("/net/mrnustik/university/solitaire/gui/images/card_default.png");
        }
        if(resource != null)
            return new ImageIcon(resource);
        else
            return super.getIcon();
    }
}
