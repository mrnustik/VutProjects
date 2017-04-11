package net.mrnustik.university.solitaire.board;

import net.mrnustik.university.solitaire.collections.CardStack;
import net.mrnustik.university.solitaire.model.Card;

/**
 * Created by mrnda on 11/04/2017.
 */
public interface Board {
    Card getDeckTop();

    Card getStackTop();

    Card getTargetTop(int i);

    CardStack getWorkingStack(int i);

    boolean isWin();

    int getScore();

    Hint getHint();

    void undo();

    boolean flipFromDeck();

    boolean fromStackerToTarget(int targetIndex);

    boolean fromWorkingToTarget(int workingIndex, int targetIndex);

    boolean fromStackerToWorking(int index);

    boolean fromTargetToWorking(int fromIndex, int toIndex);

    boolean fromWorkingToWorking(int fromIndex, int toIndex, Card card);
}
