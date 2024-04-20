package net.mrnustik.university.solitaire.model.board.commands;

import net.mrnustik.university.solitaire.model.collections.CardStack;
import net.mrnustik.university.solitaire.model.collections.CardStacker;
import net.mrnustik.university.solitaire.model.board.commands.base.AbstractCommand;
import net.mrnustik.university.solitaire.model.card.Card;

/**
 * Command for moving cards from working stack to target stacker
 *
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public class FromWorkingToTargetCommand extends AbstractCommand {

    /**
     * Working stack where the card will be taken from
     */
    private final CardStack working;

    /**
     * Target stacker where the card should be put on
     */
    private final CardStacker target;

    /**
     * Indicator whether the working stack top card was turned
     */
    private boolean wasTurned = false;

    /**
     * Creates the command
     *
     * @param working where the card will be taken from
     * @param target  where the card should be put on
     * @see CardStack
     * @see CardStacker
     */
    public FromWorkingToTargetCommand(CardStack working, CardStacker target) {
        this.working = working;
        this.target = target;
    }

    /**
     * Moves the card from working stack to target stacker.
     *
     * @return true if operation was successful
     */
    @Override
    public boolean execute() {
        success = false;
        Card card = working.get();
        if (card != null) {
            if (working.size() != 1) {
                wasTurned = !working.get(working.size() - 2).isFaceUp();
            }
            success = target.put(card);
            if (success) working.pop();
        }
        return success;
    }

    /**
     * If the command was successful, it will move the card back from target stack to working stacker
     */
    @Override
    public void undo() {
        if (wasSuccessful()) {
            if (wasTurned)
                working.get().turnFaceDown();
            working.putWithoutCheck(target.pop());
        }
    }
}
