package net.mrnustik.university.solitaire.model.board.commands;

import net.mrnustik.university.solitaire.model.collections.CardStack;
import net.mrnustik.university.solitaire.model.collections.CardStacker;
import net.mrnustik.university.solitaire.model.board.commands.base.AbstractCommand;
import net.mrnustik.university.solitaire.model.card.Card;

/**
 * Command for moving cards between working stacks
 *
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public class FromWorkingToWorkingCommand extends AbstractCommand {

    /**
     * Working stack where the cards should be taken from
     */
    private final CardStack originStack;

    /**
     * Working stack the cards should be put on
     */
    private final CardStack destinationStack;

    /**
     * Last card that should be taken from the stack
     */
    private final Card card;

    /**
     * Indicator whether the top card of from stack was turned
     */
    private boolean wasTurned = false;

    /**
     * Creates the command
     *
     * @param originStack      stack where the cards should be taken from
     * @param destinationStack stack the cards should be put on
     * @param card             last card that should be taken from the stack
     */
    public FromWorkingToWorkingCommand(CardStack originStack, CardStack destinationStack, Card card) {
        this.originStack = originStack;
        this.destinationStack = destinationStack;
        this.card = card;
    }

    /**
     * Moves cards between two working stacks
     *
     * @return true if operation was successful
     */
    @Override
    public boolean execute() {
        success = false;
        int indexOfCard = originStack.lastIndexOf(card);
        if (indexOfCard > 0) {
            wasTurned = !originStack.get(indexOfCard - 1).isFaceUp();
        }
        CardStacker movingStacker = originStack.getStack(card);
        if (movingStacker != null) {
            success = destinationStack.put(movingStacker);
            if (success)
                originStack.pop(card);
        }
        return success;
    }

    /**
     * If the operation was successful, it returns the cards back to the origin stack
     */
    @Override
    public void undo() {
        if (wasSuccessful()) {
            if (wasTurned && !originStack.isEmpty())
                originStack.get().turnFaceDown();
            CardStacker stack = destinationStack.pop(card);
            originStack.putWithoutCheck(stack);
        }
    }
}
