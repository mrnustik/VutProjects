package net.mrnustik.university.solitaire.board;

import net.mrnustik.university.solitaire.gui.panels.BoardPanel;
import net.mrnustik.university.solitaire.model.Card;

/**
 * Created by mrnda on 11/04/2017.
 */
public class Selection {

    public enum SelectionType {
        UNDEFINED,
        STACKER,
        TARGET,
        WORKING_PACK
    }

    private SelectionType type = SelectionType.UNDEFINED;
    private int index = -1;
    private Card card;

    public void reset() {
        type = SelectionType.UNDEFINED;
        index = -1;
    }

    public boolean isValid() {
        return type != SelectionType.UNDEFINED;
    }

    public int getIndex() {
        return index;
    }

    public void setIndex(int index) {
        this.index = index;
    }

    public SelectionType getType() {
        return type;
    }

    public void setType(SelectionType type) {
        this.type = type;
    }

    public Card getCard() {
        return card;
    }

    public void setCard(Card card) {
        this.card = card;
    }


}
