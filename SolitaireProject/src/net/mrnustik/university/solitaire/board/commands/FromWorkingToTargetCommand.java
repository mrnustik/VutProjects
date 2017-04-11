package net.mrnustik.university.solitaire.board.commands;

import net.mrnustik.university.solitaire.collections.CardStack;
import net.mrnustik.university.solitaire.collections.CardStacker;
import net.mrnustik.university.solitaire.board.commands.base.AbstractCommand;
import net.mrnustik.university.solitaire.model.Card;

/**
 * Created by mrnda on 11/04/2017.
 */
public class FromWorkingToTargetCommand extends AbstractCommand {
    
    private final CardStack working;
    private final CardStacker target;
    private boolean wasTurned = false;

    public FromWorkingToTargetCommand(CardStack working, CardStacker target) {
        this.working = working;
        this.target = target;
    }

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

    @Override
    public void undo() {
        if (wasSuccessful()) {
            if (wasTurned)
                working.get().turnFaceDown();
            working.putWithoutCheck(target.pop());
        }
    }
}
