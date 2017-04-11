package net.mrnustik.university.solitaire.board;

/**
 * Created by mrnda on 11/04/2017.
 */
public class Hint {
    private final Selection fromSelection;
    private final Selection toSelection;

    public Hint(Selection fromSelection, Selection toSelection) {
        this.fromSelection = fromSelection;
        this.toSelection = toSelection;
    }

    public Selection getFromSelection() {
        return fromSelection;
    }

    public Selection getToSelection() {
        return toSelection;
    }
}
