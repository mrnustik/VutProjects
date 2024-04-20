package net.mrnustik.university.solitaire.model.board.commands;

import net.mrnustik.university.solitaire.model.collections.CardStacker;
import net.mrnustik.university.solitaire.model.board.commands.base.AbstractCommand;
import net.mrnustik.university.solitaire.model.card.Card;

/**
 * Command for moving cards from Stacker to Target decks.
 *
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public class FromStackerToTargetCommand extends AbstractCommand {

    /**
     * Stacker where the card will be taken from
     */
    private final CardStacker stacker;

    /**
     * Target pack where the card will be put on
     */
    private final CardStacker target;

    /**
     * Creates the command
     *
     * @param stacker where the card will be taken from
     * @param target  where the card will be put on
     * @see CardStacker
     */
    public FromStackerToTargetCommand(CardStacker stacker, CardStacker target) {
        this.stacker = stacker;
        this.target = target;
    }

    /**
     * Moves the card from stacker to target
     *
     * @return true if operation was successful
     */
    @Override
    public boolean execute() {
        success = false;
        Card card = stacker.get();
        if (card != null) {
            if (target.put(card)) {
                success = true;
                stacker.pop();
            }
        }
        return success;
    }

    /**
     * If the command was successfully executed, it moves the card back on stacker.
     */
    @Override
    public void undo() {
        if (wasSuccessful()) {
            Card card = target.pop();
            stacker.put(card);
        }
    }
}
