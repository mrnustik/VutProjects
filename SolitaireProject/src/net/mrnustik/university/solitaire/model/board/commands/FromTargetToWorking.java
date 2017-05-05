package net.mrnustik.university.solitaire.model.board.commands;

import net.mrnustik.university.solitaire.model.collections.CardStack;
import net.mrnustik.university.solitaire.model.collections.CardStacker;
import net.mrnustik.university.solitaire.model.board.commands.base.AbstractCommand;
import net.mrnustik.university.solitaire.model.card.Card;

/**
 * Command used for moving from target to working pack
 *
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public class FromTargetToWorking extends AbstractCommand {

    /**
     * Target stacker where the card will be taken from
     */
    private final CardStacker target;
    /**
     * Working pack where the card will be put on
     */
    private final CardStack working;

    /**
     * Creates the command
     *
     * @param target  where the card will be taken from
     * @param working where the card will be put on
     * @see CardStacker
     * @see CardStack
     */
    public FromTargetToWorking(CardStacker target, CardStack working) {
        this.target = target;
        this.working = working;
    }

    /**
     * Moves the card from target to the working stack.
     *
     * @return true if operation was successful
     */
    @Override
    public boolean execute() {
        success = false;
        Card card = target.get();
        if (card != null) {
            success = working.put(card);
            if (success)
                target.pop();
        }
        return success;
    }

    /**
     * If command was successful, moves the card back from working to target
     */
    @Override
    public void undo() {
        if (wasSuccessful()) {
            Card card = working.pop();
            target.put(card);
        }
    }
}
