package net.mrnustik.university.solitaire.board.commands;

import net.mrnustik.university.solitaire.collections.CardStack;
import net.mrnustik.university.solitaire.collections.CardStacker;
import net.mrnustik.university.solitaire.board.commands.base.AbstractCommand;
import net.mrnustik.university.solitaire.model.Card;

/**
 * Created by mrnda on 11/04/2017.
 */
public class FromWorkingToWorkingCommand extends AbstractCommand {

    private final CardStack fromWorking;
    private final CardStack toWorking;
    private final Card card;
    private boolean wasTurned = false;

    public FromWorkingToWorkingCommand(CardStack fromWorking, CardStack toWorking, Card card) {
        this.fromWorking = fromWorking;
        this.toWorking = toWorking;
        this.card = card;
    }


    @Override
    public boolean execute() {
        success = false;
        int indexOfCard = fromWorking.lastIndexOf(card);
        if (indexOfCard > 0) {
            wasTurned = !fromWorking.get(indexOfCard - 1).isFaceUp();
        }
        CardStacker movingStacker = fromWorking.getStack(card);
        if (movingStacker != null) {
            success = toWorking.put(movingStacker);
            if (success)
                fromWorking.pop(card);
        }
        return success;
    }

    @Override
    public void undo() {
        if (wasSuccessful()) {
            if (wasTurned && !fromWorking.isEmpty())
                fromWorking.get().turnFaceDown();
            CardStacker stack = toWorking.pop(card);
            fromWorking.putWithoutCheck(stack);
        }
    }
}
