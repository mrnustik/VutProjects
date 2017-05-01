package net.mrnustik.university.solitaire.board.commands;

import net.mrnustik.university.solitaire.collections.CardDeck;
import net.mrnustik.university.solitaire.collections.CardStacker;
import net.mrnustik.university.solitaire.board.commands.base.AbstractCommand;

/**
 * Command for flipping card from Deck on Stack
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public class FlipCommand extends AbstractCommand {

    /**
     * Stacker where cards will turned on.
     */
    private CardStacker stacker;

    /**
     * Deck where the cards will be taken from.
     */
    private CardDeck deck;

    /**
     * Indicator whether the stacker was turned back on deck
     */
    private boolean turnedStacker = false;

    /**
     * Creates the command instance
     * @param stacker where cards will turned on.
     * @param deck where the cards will be taken from.
     * @see CardStacker
     * @see CardDeck
     */
    public FlipCommand(CardStacker stacker, CardDeck deck) {
        this.stacker = stacker;
        this.deck = deck;
    }

    /**
     * Turns the card from deck to stacker
     * @return true if command was successful
     */
    @Override
    public boolean execute() {
        success = true;
        turnedStacker = deck.isEmpty() && !stacker.isEmpty();
        return deck.pop(stacker);
    }

    /**
     * Reverts the commands effect, if the command was successful
     */
    @Override
    public void undo() {
        if (wasSuccessful()) {
            if(turnedStacker) {
                while(!deck.isEmpty()) {
                    deck.pop(stacker);
                }
            } else {
                deck.returnCard(stacker);
            }
        }
    }
}
