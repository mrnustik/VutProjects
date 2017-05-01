package net.mrnustik.university.solitaire.board;

import net.mrnustik.university.solitaire.model.Card;

/**
 * Selection representation object.
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public class Selection {


    /**
     * Types of selection
     */
    public enum SelectionType {
        /**
         * Default selection. Nothing is selected
         */
        UNDEFINED,
        /**
         * Deck is selected
         */
        DECK,
        /**
         * Stacker is selected
         */
        STACKER,
        /**
         * Target stacker is selected
         */
        TARGET,
        /**
         * Working stack is selected
         */
        WORKING_PACK
    }

    /**
     * Type of the selection
     */
    private SelectionType type = SelectionType.UNDEFINED;

    /**
     * Index of the working/target stack
     */
    private int index = -1;

    /**
     * Selected Card
     */
    private Card card;

    /**
     * Creates the selection with default values.
     * type = UDEFINED, index = -1
     */
    public Selection() {
        this(SelectionType.UNDEFINED, -1);
    }

    /**
     * Crates the selection with type and index specified
     * @param type of the selection
     * @param index of the target/working stack
     */
    public Selection(SelectionType type, int index) {
        this(type, index, null);
    }

    /**
     * Creates the selection with type, index and card specified
     * @param type of the selection
     * @param index of the target/working stack
     * @param card selected card
     */
    public Selection(SelectionType type, int index, Card card) {
        this.type = type;
        this.index = index;
        this.card = card;
    }

    /**
     * Resets the selection to default values.
     * type = UNDEFINED, index = -1
     */
    public void reset() {
        type = SelectionType.UNDEFINED;
        index = -1;
    }

    /**
     * Validity check of the selection
     * @return true if the selection is valid
     */
    public boolean isValid() {
        return type != SelectionType.UNDEFINED;
    }

    /**
     * Index getter for the selection
     * @return index of the target/working stack
     */
    public int getIndex() {
        return index;
    }

    /**
     * Index setter for the selection
     * @param index of the target/working stack
     */
    public void setIndex(int index) {
        this.index = index;
    }

    /**
     * Type getter for the selection
     * @return type of the selection
     */
    public SelectionType getType() {
        return type;
    }

    /**
     * Type setter for the selection
     * @param type of the selection
     */
    public void setType(SelectionType type) {
        this.type = type;
    }

    /**
     * Selected card getter for the selection
     * @return selected card
     */
    public Card getCard() {
        return card;
    }

    /**
     * Changes selected card for the selection
     * @param card selected card
     */
    public void setCard(Card card) {
        this.card = card;
    }


}
