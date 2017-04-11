package net.mrnustik.university.solitaire.board.commands;

import net.mrnustik.university.solitaire.collections.CardStack;
import net.mrnustik.university.solitaire.collections.CardStacker;
import net.mrnustik.university.solitaire.board.commands.base.AbstractCommand;
import net.mrnustik.university.solitaire.model.Card;

/**
 * Created by mrnda on 11/04/2017.
 */
public class FromTargetToWorking extends AbstractCommand {
    
    private final CardStacker target;
    private final CardStack working;

    public FromTargetToWorking(CardStacker target, CardStack working) {
        this.target = target;
        this.working = working;
    }

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

    @Override
    public void undo() {
        if (wasSuccessful()) {
            Card card = working.pop();
            target.put(card);
        }
    }
}
