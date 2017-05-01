package net.mrnustik.university.solitaire.gui.view;

import net.mrnustik.university.solitaire.model.Card;

import javax.swing.*;
import java.awt.*;
import java.net.URL;

/**
 * JButton that is used for Card representation
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public class CardView extends JButton {

    /**
     * Scaled version of the back image icon
     */
    private static ImageIcon scaledBackIcon;

    /**
     * Currently shown card
     */
    private Card mCard;

    /**
     * Scaled icon of the currently shown card
     */
    private ImageIcon scaledIcon;

    /**
     * Last width of the picture. Check whether to repaint.
     */
    private int lastWidth = 0;

    /**
     * Last height of the picture. Check whether to repaint.
     */
    private int lastHeight = 0;

    /**
     * Indicator for force repainting picture.
     */
    private boolean cardChanged;

    /**
     * Creates CardView for specified card
     * @param card card to be represented by this view
     */
    public CardView(Card card) {
        super();
        setSize(100, 145);
        setBorder(null);
        setContentAreaFilled(false);
        setOpaque(false);
        this.cardChanged = true;
        this.mCard = card;
    }

    /**
     * Retrieves card icon from file, scales it to the best version and stores it for later use
     * @return Icon of the card
     */
    @Override
    public Icon getIcon() {
        URL resource = null;
        if (mCard == null)
            resource = getClass().getResource("/net/mrnustik/university/solitaire/gui/images/CARD_EMPTY.png");
        else if (mCard.isFaceUp())
            resource = getClass().getResource("/net/mrnustik/university/solitaire/gui/images/" + mCard.toString() + ".png");
        else {
            if (!shouldRedraw() && scaledBackIcon != null) return scaledBackIcon;
            resource = getClass().getResource("/net/mrnustik/university/solitaire/gui/images/CARD_BACK.png");
        }
        if (resource != null)
            if (shouldRedraw()) {
                ImageIcon icon = getScaledIcon(new ImageIcon(resource));
                if (mCard != null && !mCard.isFaceUp()) scaledBackIcon = icon;
                return icon;
            } else
                return this.scaledIcon;
        else
            return super.getIcon();
    }

    /**
     * Check whether the icon should be redrawed
     * @return true if the card should be redrawed
     */
    private boolean shouldRedraw() {
        return scaledIcon == null || cardChanged || getWidth() != lastWidth || getHeight() != lastHeight;
    }

    /**
     * Scales the card icon to suitable size.
     * @param icon Image to be resized
     * @return resized image
     */
    private ImageIcon getScaledIcon(ImageIcon icon) {
        Image img = icon.getImage();


        if (getWidth() != 0)
            img = img.getScaledInstance(getWidth(), -1, Image.SCALE_SMOOTH);

        icon = new ImageIcon(img);
        cardChanged = false;
        lastWidth = img.getWidth(this);
        lastHeight = img.getHeight(this);
        scaledIcon = icon;
        return icon;
    }

    /**
     * Getter for currently viewed card
     * @return currently viewed card
     */
    public Card getCard() {
        return mCard;
    }

    /**
     * Setter for viewed card. Forces repaint
     * @param card to be viewed
     */
    public void setCard(Card card){
        this.cardChanged = true;
        this.mCard = card;
        repaint();
    }

    /**
     * Changes card. Performs check whether card is not the same.
     * @param card to be changed to
     */
    public void changeCard(Card card) {
        if (card == null) {
            this.cardChanged = true;
            this.mCard = null;
        } else if (!card.equals(mCard)) {
            this.cardChanged = true;
            this.mCard = card;
        }
        if(cardChanged)
            repaint();
    }

    /**
     * Highlights the card. Draws a yellow rectangle around it.
     */
    public void highlight() {
        this.setBorder(BorderFactory.createLineBorder(Color.orange, 5));
        this.repaint();
        Timer timer = new Timer(450, l->{
           this.setBorder(null);
           this.repaint();
        });
        timer.setRepeats(false);
        timer.start();
    }
}
