/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package net.mrnustik.university.solitaire.model.board;

import net.mrnustik.university.solitaire.model.board.commands.*;
import net.mrnustik.university.solitaire.model.collections.CardDeck;
import net.mrnustik.university.solitaire.model.collections.CardStack;
import net.mrnustik.university.solitaire.model.collections.CardStacker;
import net.mrnustik.university.solitaire.model.board.commands.base.Command;
import net.mrnustik.university.solitaire.model.board.factory.base.AbstractFactory;
import net.mrnustik.university.solitaire.model.card.Card;

import java.util.ArrayList;
import java.util.List;

/**
 * Implementation of game board object.
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public class BoardImpl implements Board {

    /**
     * Count of the working stacks in a game.
     */
    private static final int WORKING_COUNT = 7;

    /**
     * Points received for moving card from stacker to target stacker
     */
    private static final int POINTS_STACKER_TO_TARGET = 10;

    /**
     * Points received for moving card from working stack to target stacker
     */
    private static final int POINTS_WORKING_TO_TARGET = 10;

    /**
     * Points received for moving card from stacker to working stack
     */
    private static final int POINTS_STACKER_TO_WORKING = 5;

    /**
     * Points removed for moving card from target stacker to working stack
     */
    private static final int POINTS_TARGET_TO_WORKING = -15;

    /**
     * Standard deck containing cards
     */
    private final CardDeck deck;

    /**
     * Put off stacker that will be used by the deck
     */
    private final CardStacker stacker;

    /**
     * Array of target stackers
     */
    private final CardStacker[] targets;

    /**
     * Array of working stackers
     */
    private final CardStack[] workingStacks;

    /**
     * Current game score
     */
    private int score = 0;

    /**
     * History of command that have been successfully performed
     */
    private transient List<Command> commandsHistory = new ArrayList<>();

    /**
     * Creates Board using given factory
     * @param factory factory used for creation of basic objects
     */
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

    /**
     * @inheritDoc
     */
    @Override
    public Card getDeckTop() {
        return deck.get();
    }

    /**
     * @inheritDoc
     */
    @Override
    public Card getStackerTop() {
        return stacker.get();
    }

    /**
     * @inheritDoc
     */
    @Override
    public Card getTargetTop(int i) {
        return targets[i].get();
    }

    /**
     * @inheritDoc
     */
    @Override
    public CardStack getWorkingStack(int i) {
        return workingStacks[i];
    }

    /**
     * @inheritDoc
     */
    @Override
    public boolean isWin() {
        for (int i = 0; i < 4; i++) {
            if (targets[i].size() != 13)
                return false;
        }
        return true;
    }

    /**
     * @inheritDoc
     */
    @Override
    public int getScore() {
        return this.score;
    }

    /**
     * @inheritDoc
     */
    @Override
    public Hint getBestHint() {
        Hint hint;
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

    /**
     * @inheritDoc
     */
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

    /**
     * Finds available move between two working stacks
     * @return list of hints containing available moves
     */
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

    /**
     * Finds all available moves between stacker and working stacks
     * @return list of available moves
     */
    private List<Hint> findMoveStackerToWorking() {
        List<Hint> hints = new ArrayList<>();
        Card card = getStackerTop();
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

    /**
     * Finds all available moves from stacker to target stacks
     * @return list of available moves
     */
    private List<Hint> findMoveStackerToTarget() {
        List<Hint> hints = new ArrayList<>();
        Card card = getStackerTop();
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

    /**
     * Finds all moves from working stacks to target stackers
     * @return list of available moves
     */
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

    /**
     * Increases/Decreases the score. Lowest possible value of score is 0.
     * @param addition number that should be added to the score
     */
    private void addScore(int addition)
    {
        if(score + addition > 0) {
            this.score += addition;
        } else {
            this.score = 0;
        }
    }

    /**
     * @inheritDoc
     */
    @Override
    public void undo() {
        if (commandsHistory.size() > 0) {
            Command command = commandsHistory.remove(commandsHistory.size() - 1);
            addScore(-command.getScore());
            command.undo();
        }
    }

    /**
     * Adds command to the history if it was successful.
     * @param command that should be added to history
     */
    private void addCommandToHistory(Command command) {
        if (commandsHistory == null)
            commandsHistory = new ArrayList<>();
        if (command.wasSuccessful()) {
            addScore(command.getScore());
            commandsHistory.add(command);
        }
    }

    /**
     * Executes command
     * @param command to be executed
     * @return true if command was successfull
     */
    private boolean executeCommand(Command command) {
        boolean success = command.execute();
        addCommandToHistory(command);
        return success;
    }

    /**
     * @inheritDoc
     */
    @Override
    public boolean flipFromDeck() {
        Command command = new FlipCommand(stacker, deck);
        return executeCommand(command);
    }

    /**
     * @inheritDoc
     */
    @Override
    public boolean fromStackerToTarget() {
        Card card = stacker.get();
        CardStacker target = getTargetStackerForCard(card);
        if(target != null) {
            Command command = new FromStackerToTargetCommand(stacker, target);
            command.setScore(POINTS_STACKER_TO_TARGET);
            return executeCommand(command);
        }
        return false;
    }

    /**
     * Finds target stacker where the card can be put on
     * @param card to be put on
     * @return true if there is an available target stacker
     */
    private CardStacker getTargetStackerForCard(Card card) {
        for (CardStacker target : targets) {
            if (target.canPut(card)) {
                return target;
            }
        }
        return null;
    }

    /**
     * @inheritDoc
     */
    @Override
    public boolean fromWorkingToTarget(int workingIndex) {
        Card card = workingStacks[workingIndex].get();
        CardStacker target = getTargetStackerForCard(card);
        if(target != null) {
            Command command = new FromWorkingToTargetCommand(workingStacks[workingIndex], target);
            command.setScore(POINTS_WORKING_TO_TARGET);
            return executeCommand(command);
        }
        return false;
    }

    /**
     * @inheritDoc
     */
    @Override
    public boolean fromStackerToWorking(int index) {
        Command command = new FromStackerToWorkingCommand(stacker, workingStacks[index]);
        command.setScore(POINTS_STACKER_TO_WORKING);
        return executeCommand(command);
    }

    /**
     * @inheritDoc
     */
    @Override
    public boolean fromTargetToWorking(int fromIndex, int toIndex) {
        Command command = new FromTargetToWorking(targets[fromIndex], workingStacks[toIndex]);
        command.setScore(POINTS_TARGET_TO_WORKING);
        return executeCommand(command);
    }

    /**
     * @inheritDoc
     */
    @Override
    public boolean fromWorkingToWorking(int fromIndex, int toIndex, Card card) {
        Command command = new FromWorkingToWorkingCommand(workingStacks[fromIndex],
                workingStacks[toIndex],
                card);
        return executeCommand(command);
    }

}
