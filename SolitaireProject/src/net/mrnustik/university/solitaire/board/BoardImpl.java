/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package net.mrnustik.university.solitaire.board;

import net.mrnustik.university.solitaire.board.commands.*;
import net.mrnustik.university.solitaire.collections.CardDeck;
import net.mrnustik.university.solitaire.collections.CardStack;
import net.mrnustik.university.solitaire.collections.CardStacker;
import net.mrnustik.university.solitaire.board.commands.base.Command;
import net.mrnustik.university.solitaire.factory.base.AbstractFactory;
import net.mrnustik.university.solitaire.model.Card;

import java.util.ArrayList;
import java.util.List;

/**
 * @author micha
 */
public class BoardImpl implements Board {

    private static final int WORKING_COUNT = 7;
    public static final int POINTS_STACKER_TO_TARGET = 10;
    public static final int POINTS_WORKING_TO_TARGET = 10;
    public static final int POINTS_STACKER_TO_WORKING = 5;
    public static final int POINTS_TARGET_TO_WORKING = -15;
    private final CardDeck deck;
    private final CardStacker stacker;
    private final CardStacker[] targets;
    private final CardStack[] workingStacks;

    private int score = 0;

    private transient List<Command> commandsHistory = new ArrayList<>();

    public BoardImpl(AbstractFactory factory) {
        this.deck = factory.createCardDeck();
        this.stacker = factory.createPutDownStacker();
        this.targets = new CardStacker[4];
        for (int i = 0; i < this.targets.length; i++) {
            this.targets[i] = factory.createTargetStacker(Card.Color.values()[i]);
        }
        this.workingStacks = new CardStack[WORKING_COUNT];
        for (int i = 0; i < this.workingStacks.length; i++) {
            this.workingStacks[i] = factory.createWorkingPack(i + 1, this.deck);
        }
    }

    @Override
    public Card getDeckTop() {
        return deck.get();
    }

    @Override
    public Card getStackTop() {
        return stacker.get();
    }

    @Override
    public Card getTargetTop(int i) {
        return targets[i].get();
    }

    @Override
    public CardStack getWorkingStack(int i) {
        return workingStacks[i];
    }

    @Override
    public boolean isWin() {
        for (int i = 0; i < 4; i++) {
            if (targets[i].size() != 13)
                return false;
        }
        return true;
    }

    @Override
    public int getScore() {
        return this.score;
    }

    @Override
    public Hint getBestHint() {
        Hint hint = null;
        hint = findMoveWorkingToTarget().get(0);
        if(hint == null)
            hint = findMoveStackerToTarget().get(0);
        if(hint == null)
            hint = findMoveStackerToWorking().get(0);
        if(hint == null)
            hint = findMoveWorkingToWorking().get(0);
        if(hint == null) {
            if(!deck.isEmpty() || !stacker.isEmpty()) {
                Selection select = new Selection();
                select.setType(Selection.SelectionType.DECK);
                hint = new Hint(select, null);
            }
        }

        return hint;
    }

    @Override
    public List<Hint> getAllHints() {
        List<Hint> hints = new ArrayList<>();
        hints.addAll(findMoveStackerToTarget());
        hints.addAll(findMoveStackerToWorking());
        hints.addAll(findMoveWorkingToWorking());
        hints.addAll(findMoveWorkingToTarget());
        if(hints.size() == 0)
        {
            if(!deck.isEmpty() || !stacker.isEmpty()) {
                Selection select = new Selection();
                select.setType(Selection.SelectionType.DECK);
                hints.add(new Hint(select, null));
            }
        }
        return hints;
    }


    private List<Hint> findMoveWorkingToWorking() {
        List<Hint> hints = new ArrayList<>();
        for(int i = 0; i < workingStacks.length; i++){
            CardStack working = workingStacks[i];
            for(int j = 0; j < workingStacks.length; j++){
                if(i == j) continue;
                for(int k = 0; k < working.size(); k++){
                    Card card = working.get(k);
                    if(card != null) {
                        if (!card.isFaceUp()) continue;
                        CardStack toWorking = workingStacks[j];
                        if (toWorking.canPut(card)) {
                            Selection fromSelection = new Selection(Selection.SelectionType.WORKING_PACK, i, card);
                            Selection toSelection = new Selection(Selection.SelectionType.WORKING_PACK, j, toWorking.get());
                            hints.add(new Hint(fromSelection, toSelection));
                        }
                    }

                }
            }
        }
        return hints;
    }

    private List<Hint> findMoveStackerToWorking() {
        List<Hint> hints = new ArrayList<>();
        Card card = getStackTop();
        if(card != null) {
            for(int i = 0; i < workingStacks.length; i++){
                CardStacker working = workingStacks[i];
                if(working.canPut(card)){
                    Selection fromSelection = new Selection(Selection.SelectionType.STACKER, 0);
                    Selection toSelection = new Selection(Selection.SelectionType.WORKING_PACK, i, working.get());
                    hints.add(new Hint(fromSelection, toSelection));
                }
            }
        }
        return hints;
    }

    private List<Hint> findMoveStackerToTarget() {
        List<Hint> hints = new ArrayList<>();
        Card card = getStackTop();
        if(card != null) {
            for(int i = 0; i< targets.length; i++) {
                CardStacker target = targets[i];
                if(target.canPut(card)){
                    Selection fromSelection = new Selection(Selection.SelectionType.STACKER, 0);
                    Selection toSelection = new Selection(Selection.SelectionType.TARGET, i);
                    hints.add(new Hint(fromSelection, toSelection));
                }
            }
        }
        return hints;
    }

    private List<Hint> findMoveWorkingToTarget() {
        List<Hint> hints = new ArrayList<>();
        for (int i = 0; i < workingStacks.length; i++) {
            Card card = workingStacks[i].get();
            if(card != null) {
                for (int j = 0; j < targets.length; j++) {
                    CardStacker target = targets[j];
                    if (target.canPut(card)) {
                        Selection fromSelection = new Selection(Selection.SelectionType.WORKING_PACK, i, card);
                        Selection toSelection = new Selection(Selection.SelectionType.TARGET, j);
                        hints.add(new Hint(fromSelection, toSelection));
                    }
                }
            }
        }
        return hints;
    }

    private void addScore(int addition)
    {
        if(score + addition > 0) {
            this.score += addition;
        } else {
            this.score = 0;
        }
    }

    @Override
    public void undo() {
        if (commandsHistory.size() > 0) {
            Command command = commandsHistory.remove(commandsHistory.size() - 1);
            addScore(-command.getScore());
            command.undo();
        }
    }

    private void addCommandToHistory(Command command) {
        if (commandsHistory == null)
            commandsHistory = new ArrayList<>();
        if (command.wasSuccessful()) {
            addScore(command.getScore());
            commandsHistory.add(command);
        }
    }

    private boolean executeCommand(Command command) {
        boolean success = command.execute();
        addCommandToHistory(command);
        return success;
    }

    @Override
    public boolean flipFromDeck() {
        Command command = new FlipCommand(stacker, deck);
        return executeCommand(command);
    }

    @Override
    public boolean fromStackerToTarget(int targetIndex) {
        Card card = stacker.get();
        CardStacker target = getTargetStackerForCard(card);
        if(target != null) {
            Command command = new FromStackerToTargetCommand(stacker, target);
            command.setScore(POINTS_STACKER_TO_TARGET);
            return executeCommand(command);
        }
        return false;
    }

    private CardStacker getTargetStackerForCard(Card card) {
        for(int i = 0; i< targets.length; i++){
            if(targets[i].canPut(card))
            {
                return targets[i];
            }
        }
        return null;
    }

    @Override
    public boolean fromWorkingToTarget(int workingIndex, int targetIndex) {
        Card card = workingStacks[workingIndex].get();
        CardStacker target = getTargetStackerForCard(card);
        if(target != null) {
            Command command = new FromWorkingToTargetCommand(workingStacks[workingIndex], target);
            command.setScore(POINTS_WORKING_TO_TARGET);
            return executeCommand(command);
        }
        return false;
    }

    @Override
    public boolean fromStackerToWorking(int index) {
        Command command = new FromStackerToWorkingCommand(stacker, workingStacks[index]);
        command.setScore(POINTS_STACKER_TO_WORKING);
        return executeCommand(command);
    }

    @Override
    public boolean fromTargetToWorking(int fromIndex, int toIndex) {
        Command command = new FromTargetToWorking(targets[fromIndex], workingStacks[toIndex]);
        command.setScore(POINTS_TARGET_TO_WORKING);
        return executeCommand(command);
    }

    @Override
    public boolean fromWorkingToWorking(int fromIndex, int toIndex, Card card) {
        Command command = new FromWorkingToWorkingCommand(workingStacks[fromIndex],
                workingStacks[toIndex],
                card);
        return executeCommand(command);
    }

}
