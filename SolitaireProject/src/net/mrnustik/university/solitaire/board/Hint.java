package net.mrnustik.university.solitaire.board;

/**
 * Immutable representation of possible move on the board.
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public class Hint {
    /**
     * Source of the move
     */
    private final Selection fromSelection;
    /**
     * Destination of the move
     */
    private final Selection toSelection;

    /**
     * Creates the hint with defined attributes
     * @param fromSelection source of the move
     * @param toSelection destination of the move
     */
    public Hint(Selection fromSelection, Selection toSelection) {
        this.fromSelection = fromSelection;
        this.toSelection = toSelection;
    }

    /**
     * Returns the origin of the move
     * @return origin of the move
     */
    public Selection getFromSelection() {
        return fromSelection;
    }

    /**
     * Returns the destination of the move
     * @return destination of the move
     */
    public Selection getToSelection() {
        return toSelection;
    }
}
