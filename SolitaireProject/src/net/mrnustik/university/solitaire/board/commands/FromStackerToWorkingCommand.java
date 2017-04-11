package net.mrnustik.university.solitaire.board.commands;

import net.mrnustik.university.solitaire.collections.CardStack;
import net.mrnustik.university.solitaire.collections.CardStacker;
import net.mrnustik.university.solitaire.board.commands.base.AbstractCommand;
import net.mrnustik.university.solitaire.model.Card;

/**
 * Created by mrnda on 11/04/2017.
 */
public class FromStackerToWorkingCommand extends AbstractCommand {

    private final CardStacker stacker;
    private final CardStack workingStack;

    public FromStackerToWorkingCommand(CardStacker stacker, CardStack workingStack) {
        this.stacker = stacker;
        this.workingStack = workingStack;
    }


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

    @Override
    public void undo() {
        if (wasSuccessful()) {
            Card card = workingStack.pop();
            stacker.put(card);
        }
    }
}
