package net.mrnustik.university.solitaire.view;

import net.mrnustik.university.solitaire.model.board.Selection;
import net.mrnustik.university.solitaire.model.card.Card;
import net.mrnustik.university.solitaire.model.collections.CardStack;

/**
 * Interface representing view, that is showing all necessary information about a solitaire board.
 *
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public interface BoardView {

    /**
     * Highlights card on the deck
     *
     * @param selection card to be highlighted
     */
    void highlightSelection(Selection selection);

    /**
     * Shows game winning dialog.
     */
    void showWin();

    /**
     * Repaint score on the deck
     *
     * @param score points achieved by player
     */
    void showScore(int score);

    /**
     * Repaint card Deck and card Stacker
     *
     * @param deck    card that is on top of a deck
     * @param stacker card that is on top of a stacker
     */
    void repaintDeck(Card deck, Card stacker);

    /**
     * Repaint target stacker
     *
     * @param card  that is on top of target stacker
     * @param index of the target stack to be repainted
     */
    void repaintTargets(Card card, int index);

    /**
     * Repaint working stack
     *
     * @param stack to be painted
     * @param index of the stack
     */
    void repaintWorking(CardStack stack, int index);
}
