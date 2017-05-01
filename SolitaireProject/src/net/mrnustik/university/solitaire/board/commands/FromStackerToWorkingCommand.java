package net.mrnustik.university.solitaire.board.commands;

import net.mrnustik.university.solitaire.collections.CardStack;
import net.mrnustik.university.solitaire.collections.CardStacker;
import net.mrnustik.university.solitaire.board.commands.base.AbstractCommand;
import net.mrnustik.university.solitaire.model.Card;

/**
 * Command used for moving cards from Stacker to Working pack
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public class FromStackerToWorkingCommand extends AbstractCommand {
    /**
     * Stacker where the card will be taken from
     */
    private final CardStacker stacker;
    /**
     * Working stack where the card will be put on
     */
    private final CardStack workingStack;

    /**
     * Creates the command
     * @param stacker where the card will be taken from
     * @param workingStack where the card will be put on
     * @see CardStacker
     * @see CardStack
     */
    public FromStackerToWorkingCommand(CardStacker stacker, CardStack workingStack) {
        this.stacker = stacker;
        this.workingStack = workingStack;
    }

    /**
     * Moves the card from stacker to working pack
     * @return true if operation was successful
     */
    @Override
    public boolean execute() {
        success = false;
        Card card = stacker.get();
        if (card != null) {
            if (workingStack.put(card)) {
                success = true;
                stacker.pop();
            }
        }
        return success;
    }

    /**
     * If the command was successful, moves the card back on the stacker
     */
    @Override
    public void undo() {
        if (wasSuccessful()) {
            Card card = workingStack.pop();
            stacker.put(card);
        }
    }
}
