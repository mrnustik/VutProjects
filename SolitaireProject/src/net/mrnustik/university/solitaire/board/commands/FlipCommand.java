package net.mrnustik.university.solitaire.board.commands;

import net.mrnustik.university.solitaire.collections.CardDeck;
import net.mrnustik.university.solitaire.collections.CardStacker;
import net.mrnustik.university.solitaire.board.commands.base.AbstractCommand;

/**
 * Created by mrnda on 11/04/2017.
 */
public class FlipCommand extends AbstractCommand {

    private CardStacker stacker;
    private CardDeck deck;

    public FlipCommand(CardStacker stacker, CardDeck deck) {
        this.stacker = stacker;
        this.deck = deck;
    }


    @Override
    public boolean execute() {
        success = true;
        return deck.pop(stacker);
    }

    @Override
    public void undo() {
        if (wasSuccessful()) {
            deck.returnCard(stacker);
        }
    }
}
