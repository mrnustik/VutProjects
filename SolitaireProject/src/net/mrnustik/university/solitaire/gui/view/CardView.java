package net.mrnustik.university.solitaire.gui.view;

import net.mrnustik.university.solitaire.model.Card;

import javax.swing.*;
import java.awt.*;
import java.net.URL;

/**
 * Created by mrnda on 07/04/2017.
 */
public class CardView extends JButton {

    private static ImageIcon scaledBackIcon;

    private Card mCard;
    private ImageIcon scaledIcon;
    private int lastWidth = 0;
    private int lastHeight = 0;
    private boolean cardChanged;


    public CardView(Card card) {
        super();
        setSize(100,145);
        setBorder(null);
        setBorderPainted(false);
        setContentAreaFilled(false);
        setFocusPainted(false);
        setOpaque(false);
        this.cardChanged = true;
        this.mCard = card;
    }



    @Override
    public Icon getIcon() {
        URL resource = null;
        if(mCard == null)
            resource = getClass().getResource("/net/mrnustik/university/solitaire/gui/images/CARD_EMPTY.png");
        else if(mCard.isFaceUp())
            resource = getClass().getResource("/net/mrnustik/university/solitaire/gui/images/" + mCard.toString() + ".png");
        else {
            if(!shouldRedraw() && scaledBackIcon != null) return scaledBackIcon;
            resource = getClass().getResource("/net/mrnustik/university/solitaire/gui/images/CARD_BACK.png");
        }
        if(resource != null)
            if(shouldRedraw()) {
                ImageIcon icon = getScaledIcon(new ImageIcon(resource));
                if (mCard != null && !mCard.isFaceUp()) scaledBackIcon = icon;
                return icon;
            }
            else
                return this.scaledIcon;
        else
            return super.getIcon();
    }

    private boolean shouldRedraw() {
        return scaledIcon == null || cardChanged || getWidth() != lastWidth || getHeight() != lastHeight;
    }

    private ImageIcon getScaledIcon(ImageIcon icon) {
        Image img = icon.getImage();


        if(getWidth() != 0)
            img = img.getScaledInstance(getWidth(),-1, Image.SCALE_SMOOTH);

        icon = new ImageIcon(img);
        cardChanged = false;
        lastWidth = img.getWidth(this);
        lastHeight = img.getHeight(this);
        scaledIcon = icon;
        return icon;
    }

    public Card getCard() {
        return mCard;
    }

    public void changeCard(Card card) {
        if(card == null) {
            this.cardChanged = true;
            this.mCard = card;
        }
        else if(!card.equals(mCard)) {
            this.cardChanged = true;
            this.mCard = card;
        }
    }
}
