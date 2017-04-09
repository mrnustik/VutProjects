/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package net.mrnustik.university.solitaire.model;

import net.mrnustik.university.solitaire.command.AbstractCommand;
import net.mrnustik.university.solitaire.command.Command;
import net.mrnustik.university.solitaire.factory.base.AbstractFactory;
import net.mrnustik.university.solitaire.collections.CardDeck;
import net.mrnustik.university.solitaire.collections.CardStack;
import net.mrnustik.university.solitaire.collections.CardStacker;

import java.util.ArrayList;
import java.util.List;

/**
 *
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
        for(int i = 0; i < this.targets.length; i++) {
            this.targets[i] = factory.createTargetStacker(Card.Color.values()[i]);
        }
        this.workingStacks = new CardStack[WORKING_COUNT];
        for(int i = 0; i< this.workingStacks.length; i++){
            this.workingStacks[i] = factory.createWorkingPack(i+1, this.deck);
        }
    }

    public void undo() {
        if(commandsHistory.size() > 0)
        {
            Command command = commandsHistory.remove(commandsHistory.size() - 1);
            command.undo();
        }
    }

    public boolean flipFromDeck()
    {
        Command flipCommand = new FlipCommand();
        return executeCommand(flipCommand);
    }

    private boolean executeCommand(Command flipCommand) {
        boolean success = flipCommand.execute();
        addCommandToHistory(flipCommand);
        return success;
    }

    public boolean fromStackerToTarget(int targetIndex)
    {
        Command fromStackerToCardDeck = new FromStrackerToTargetCommand(targetIndex);
        return executeCommand(fromStackerToCardDeck);
    }

    private void addCommandToHistory(Command command) {
        if(commandsHistory == null)
            commandsHistory = new ArrayList<>();
        if(command.wasSuccessful())
            commandsHistory.add(command);
    }

    public Card getDeckTop() {
        return deck.get();
    }

    public Card getStackTop() {
        return stacker.get();
    }

    public Card getTargetTop(int i){
        return targets[i].get();
    }

    public CardStack getWorkingStack(int i) {return workingStacks[i];}

    public boolean isWin() {
        for(int i = 0;i < 4; i++)
        {
            if(targets[i].size() != 13)
                return false;
        }
        return true;
    }

    private class FlipCommand extends  AbstractCommand {
        @Override
        public boolean execute() {
            success = true;
            return deck.pop(stacker);
        }

        @Override
        public void undo() {
            if(wasSuccessful()){
                deck.returnCard(stacker);
            }
        }
    }

    private class FromStrackerToTargetCommand extends AbstractCommand {
        private final int targetIndex;

        public FromStrackerToTargetCommand(int targetIndex) {
            this.targetIndex = targetIndex;
        }

        @Override
        public boolean execute() {
            success = false;
            Card card = stacker.get();
            if(card != null)
            {
                if(targets[targetIndex].put(card)) {
                    success = true;
                    stacker.pop();
                }
            }
            return success;
        }

        @Override
        public void undo() {
            if(wasSuccessful())
            {
                Card card = targets[targetIndex].pop();
                stacker.put(card);
            }
        }
    }
}
