package net.mrnustik.university.solitaire.board.commands;

import net.mrnustik.university.solitaire.collections.CardStacker;
import net.mrnustik.university.solitaire.board.commands.base.AbstractCommand;
import net.mrnustik.university.solitaire.model.Card;

/**
 * Created by mrnda on 11/04/2017.
 */
public class FromStackerToTargetCommand extends AbstractCommand {

    private final CardStacker stacker;
    private final CardStacker target;

    public FromStackerToTargetCommand(CardStacker stacker, CardStacker target) {
        this.stacker = stacker;
        this.target = target;
    }


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

    @Override
    public void undo() {
        if (wasSuccessful()) {
            Card card = target.pop();
            stacker.put(card);
        }
    }
}
