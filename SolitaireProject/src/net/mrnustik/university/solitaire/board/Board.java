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
public class Board {

    private static final int WORKING_COUNT = 7;
    private final CardDeck deck;
    private final CardStacker stacker;
    private final CardStacker[] targets;
    private final CardStack[] workingStacks;


    private transient List<Command> commandsHistory = new ArrayList<>();

    public Board(AbstractFactory factory) {
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

    public Card getDeckTop() {
        return deck.get();
    }

    public Card getStackTop() {
        return stacker.get();
    }

    public Card getTargetTop(int i) {
        return targets[i].get();
    }

    public CardStack getWorkingStack(int i) {
        return workingStacks[i];
    }

    public boolean isWin() {
        for (int i = 0; i < 4; i++) {
            if (targets[i].size() != 13)
                return false;
        }
        return true;
    }

    public void undo() {
        if (commandsHistory.size() > 0) {
            Command command = commandsHistory.remove(commandsHistory.size() - 1);
            command.undo();
        }
    }

    private void addCommandToHistory(Command command) {
        if (commandsHistory == null)
            commandsHistory = new ArrayList<>();
        if (command.wasSuccessful())
            commandsHistory.add(command);
    }

    private void executeCommand(Command command) {
        boolean success = command.execute();
        addCommandToHistory(command);
    }

    public void flipFromDeck() {
        Command command = new FlipCommand(stacker, deck);
        executeCommand(command);
    }

    public void fromStackerToTarget(int targetIndex) {
        Command command = new FromStackerToTargetCommand(stacker, targets[targetIndex]);
        executeCommand(command);
    }

    public void fromWorkingToTarget(int workingIndex, int targetIndex) {
        Command command = new FromWorkingToTargetCommand(workingStacks[workingIndex], targets[targetIndex]);
        executeCommand(command);
    }

    public void fromStackerToWorking(int index) {
        Command command = new FromStackerToWorkingCommand(stacker, workingStacks[index]);
        executeCommand(command);
    }

    public void fromTargetToWorking(int fromIndex, int toIndex) {
        Command command = new FromTargetToWorking(targets[fromIndex], workingStacks[toIndex]);
        executeCommand(command);

    }

    public void fromWorkingToWorking(int fromIndex, int toIndex, Card card) {
        Command command = new FromWorkingToWorkingCommand(workingStacks[fromIndex],
                workingStacks[toIndex],
                card);
        executeCommand(command);
    }

}
