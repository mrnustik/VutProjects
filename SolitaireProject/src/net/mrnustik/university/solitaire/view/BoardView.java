package net.mrnustik.university.solitaire.view;

import net.mrnustik.university.solitaire.model.board.Selection;
import net.mrnustik.university.solitaire.model.card.Card;
import net.mrnustik.university.solitaire.model.collections.CardStack;

/**
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public interface BoardView {

    void highlightSelection(Selection selection);

    void showWin();

    void showScore(int score);

    void repaintDeck(Card deck, Card stacker);

    void repaintTargets(Card card, int index);

    void repaintWorking(CardStack stack, int index);
}
