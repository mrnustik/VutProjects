package net.mrnustik.university.solitaire.gui.view;

import net.mrnustik.university.solitaire.model.Card;

import javax.swing.*;
import java.awt.*;
import java.net.URL;

/**
 * Created by mrnda on 07/04/2017.
 */
public class CardView extends JButton {

    private Card mCard;

    public CardView(Card card) {
        super();
        setSize(50,100);
        setBorder(null);
        this.mCard = card;
    }

    @Override
    public Icon getIcon() {
        URL resource = null;
        if(mCard == null) return super.getIcon();
        if(mCard.isFaceUp()) {
            resource = getClass().getResource("/net/mrnustik/university/solitaire/gui/images/" + mCard.toString() + ".png");
        } else {
            resource = getClass().getResource("/net/mrnustik/university/solitaire/gui/images/card_default.png");
        }
        if(resource != null)
            return getScaledIcon(new ImageIcon(resource));
        else
            return super.getIcon();
    }

    private Icon getScaledIcon(ImageIcon icon) {
        Image img = icon.getImage();
        Image newimg = img.getScaledInstance(getWidth(),-1, Image.SCALE_SMOOTH);
        icon = new ImageIcon(newimg);
        return icon;
    }

    public Card getCard() {
        return mCard;
    }

    public void changeCard(Card card) {
        this.mCard = card;
    }
}
