package net.mrnustik.university.solitaire.view.panels;

import net.mrnustik.university.solitaire.model.collections.CardStack;
import net.mrnustik.university.solitaire.view.view.CardView;
import net.mrnustik.university.solitaire.model.card.Card;

import javax.swing.*;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.util.ArrayList;
import java.util.List;

/**
 * JLayeredPane used to represent stack of cards
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
class CardStackPanel extends JLayeredPane implements ComponentListener {

    /**
     * Index of the workings stack
     */
    private final int index;

    /**
     * Timer used for redrawing after resize
     */
    private final Timer timer;

    /**
     * Stack to be shown in the panel
     */
    private CardStack stack;

    /**
     * Listener of the card seelected event
     */
    private CardSelected listener = null;

    /**
     * Cardviews that are currently shown.
     */
    private final List<CardView> cards;

    /**
     * Creates the panel with initial stack and gives it its index.
     * @param stack to be represented
     * @param index of the working stack
     */
    CardStackPanel(CardStack stack, int index) {
        super();
        addComponentListener(this);
        timer = new Timer(30, e -> paintStack());
        timer.setRepeats(false);
        this.stack = stack;
        this.index = index;
        this.cards = new ArrayList<>();
    }

    /**
     * Sets the event listener for card selection
     * @param listener listener of the selection
     */
    void setCardSelectedListener(CardSelected listener) {
        this.listener = listener;
    }

    /**
     * Changes the current stack of cards
     * @param stack to be shown
     */
    void setStack(CardStack stack) {
        this.stack = stack;
        paintStack();
    }

    /**
     * Paints the current state of the stack
     */
    private void paintStack() {
        int y = 0;
        int index;
        if (stack.size() == 0) {
            index = 1;
            if(cards.size() == 0){
                createCardView(0,null);
            }
            cards.get(0).setCard(null);
        } else {
            for (index = 0; index < stack.size(); index++) {
                Card card = stack.get(index);
                CardView view;
                if (index < cards.size()) {
                    view = cards.get(index);
                } else {
                    view = createCardView(index, card);
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
        repaint();
    }

    /**
     * Creates CardView and adds it to the panel
     * @param index z index of the card
     * @param card card model
     * @return created CardView
     */
    private CardView createCardView(int index, Card card) {
        CardView view = new CardView(card);
        cards.add(view);
        add(view, new Integer(index));
        view.addActionListener(l -> cardClicked(l.getSource()));
        return view;
    }

    /**
     * Event listener, that passes the event to the {@link CardStackPanel#listener}
     * @param source source of the event
     */
    private void cardClicked(Object source) {
        if(source instanceof CardView) {
            CardView view = (CardView) source;
            if(listener != null){
                listener.onCardSelected(view.getCard(), index);
            }
        }
    }

    /**
     * Event listener for resizing of the panel
     * @param e event arguments
     */
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

    /**
     * Highlights the cards in stack until specified card
     * @param highlightCard last card to be highlighted
     */
    void highlight(Card highlightCard) {
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

    /**
     * Interface for card selection event listening
     */
    public interface CardSelected {
        /**
         * Method called when the card selection event happens
         * @param card card that was selected
         * @param index of the working stack
         */
        void onCardSelected(Card card, int index);
    }
}
