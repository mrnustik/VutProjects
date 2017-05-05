package net.mrnustik.university.solitaire.presenter;

import net.mrnustik.university.solitaire.model.board.Board;
import net.mrnustik.university.solitaire.model.board.BoardImpl;
import net.mrnustik.university.solitaire.model.board.Hint;
import net.mrnustik.university.solitaire.model.board.Selection;
import net.mrnustik.university.solitaire.model.card.Card;
import net.mrnustik.university.solitaire.model.io.BoardSaver;
import net.mrnustik.university.solitaire.model.io.json.JsonBoardSaver;
import net.mrnustik.university.solitaire.view.BoardView;

import javax.swing.*;
import java.io.IOException;
import java.util.List;

/**
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public class BoardPresenter {

    private final Board board;

    private final BoardView view;

    private Selection selection;

    public BoardPresenter(BoardView view, Board board) {
        this.board = board;
        this.view = view;
        this.selection = new Selection();
        this.repaint();
    }

    public void deckClicked(){
        this.board.flipFromDeck();
        repaintDeck();
    }

    public void stackerClicked(){
        this.selection.reset();
        this.selection.setType(Selection.SelectionType.STACKER);
    }

    public void targetClicked(int index){
        if(this.selection.isValid()) {
            switch (this.selection.getType()) {
                case STACKER:
                    board.fromStackerToTarget();
                    repaintDeck();
                    repaintTargets();
                    repaintScore();
                    break;
                case WORKING_PACK:
                    board.fromWorkingToTarget(selection.getIndex());
                    repaintWorkingStack(selection.getIndex());
                    repaintTargets();
                    break;
            }
            repaintScore();
            this.selection.reset();
            if(this.board.isWin()) {
                view.showWin();
            }
        } else {
            selection.setType(Selection.SelectionType.TARGET);
            selection.setIndex(index);
        }
    }



    public void workingClicked(int index, Card card){
        if(this.selection.isValid()) {
            if(selection.getType() == Selection.SelectionType.WORKING_PACK
                    && selection.getIndex() == index
                    && selection.getCard().equals(card)) return;
            switch (this.selection.getType()) {
                case STACKER:
                    board.fromStackerToWorking(index);
                    repaintDeck();
                    repaintWorkingStack(index);
                    break;
                case TARGET:
                    board.fromTargetToWorking(selection.getIndex(), index);
                    repaintTargets();
                    repaintWorkingStack(index);
                    break;
                case WORKING_PACK:
                    board.fromWorkingToWorking(selection.getIndex(), index, selection.getCard());
                    repaintWorkingStack(selection.getIndex());
                    repaintWorkingStack(index);
                    break;
            }
            repaintScore();
            this.selection.reset();
        } else {
            selection.setType(Selection.SelectionType.WORKING_PACK);
            selection.setIndex(index);
            selection.setCard(card);
        }
    }

    private void repaint(){
        repaintDeck();
        repaintTargets();
        for(int index = 0; index < BoardImpl.WORKING_COUNT; index++){
            repaintWorkingStack(index);
        }
        repaintScore();
    }
    private void repaintScore() {
        view.showScore(board.getScore());
    }

    private void repaintWorkingStack(int index) {
        view.repaintWorking(board.getWorkingStack(index), index);
    }

    private void repaintTargets() {
        for(int index = 0; index < 4; index++)
            view.repaintTargets(board.getTargetTop(index), index);
    }

    private void repaintDeck() {
        view.repaintDeck(board.getDeckTop(), board.getStackerTop());
    }

    public void saveBoard(String path) throws IOException {
        BoardSaver saver = new JsonBoardSaver();
        saver.save(path, this.board);
    }

    public void undoClicked() {
        this.board.undo();
        repaint();
    }

    public void hintClicked() {
        List<Hint> hints = this.board.getAllHints();
        int delay = 0;
        for (Hint hint:
             hints) {
            if(hint.getFromSelection() != null) {
                Timer timer = new Timer(delay, l -> view.highlightSelection(hint.getFromSelection()));
                timer.setRepeats(false);
                timer.start();
            }
            delay += 100;
            if(hint.getToSelection() != null) {
                Timer timer = new Timer(delay, l -> view.highlightSelection(hint.getToSelection()));
                timer.setRepeats(false);
                timer.start();
            }
            delay += 1000;
        }
    }
}
