package net.mrnustik.university.solitaire.model.board;

import net.mrnustik.university.solitaire.model.collections.CardStack;
import net.mrnustik.university.solitaire.model.card.Card;

import java.util.List;

/**
 * Represents game board of Solitaire
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public interface Board {

    /**
     * Gets top card from Deck
     * @return top card from deck
     */
    Card getDeckTop();

    /**
     * Gets top card from Stacker
     * @return top card from stacker
     */
    Card getStackerTop();

    /**
     * Gets top card from target stack on index
     * @param i index of the target stack
     * @return top card of target stack on index i
     */
    Card getTargetTop(int i);

    /**
     * Gets all cards from working stack on index
     * @param i index of the working stack
     * @return All cards from working stack
     */
    CardStack getWorkingStack(int i);

    /**
     * Check whether the game has been won.
     * @return true if game was won
     */
    boolean isWin();

    /**
     * Retrieves current score from the counter
     * @return current score of the game
     */
    int getScore();

    /**
     * Computes hint for the best possible move in this stage of the game
     * @return best hint for a move
     */
    Hint getBestHint();

    /**
     * Computes all possible moves in the current state of the game
     * @return list of all possible moves
     */
    List<Hint> getAllHints();

    /**
     * Undoes the last operation that was performed
     */
    void undo();

    /**
     * Turns card from the deck to the stacker
     * @return true if operation was successful
     */
    boolean flipFromDeck();

    /**
     * Moves card from stacker to target stacker
     * @param targetIndex index of the target stacker
     * @return true if operation was successful
     */
    boolean fromStackerToTarget(int targetIndex);

    /**
     * Moves card from working stack to target stacker
     * @param workingIndex index of the working stack where the card will be taken from
     * @param targetIndex index of the target stacker where the card will be put on
     * @return true if operation was successful
     */
    boolean fromWorkingToTarget(int workingIndex, int targetIndex);

    /**
     * Moves card from target stacker to working stack
     * @param index index of the working stack where the card will be put on
     * @return true if operation was successful
     */
    boolean fromStackerToWorking(int index);

    /**
     * Moves card from target stacker to working stack
     * @param fromIndex index of the working stack where the cards will be taken from
     * @param toIndex index of the target stacker where the cards will be put on
     * @return true if operation was successful
     */
    boolean fromTargetToWorking(int fromIndex, int toIndex);

    /**
     * Moves card from working stack to another working stack
     * @param fromIndex index of the working stack where the cards will taken from
     * @param toIndex index of the working stack where the cards will be put on
     * @param card last card that should be taken from the working stack
     * @return true if operation was successful
     */
    boolean fromWorkingToWorking(int fromIndex, int toIndex, Card card);
}
