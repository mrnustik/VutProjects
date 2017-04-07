/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package net.mrnustik.university.solitaire.model;

import net.mrnustik.university.solitaire.factory.base.AbstractFactory;
import net.mrnustik.university.solitaire.collections.CardDeck;
import net.mrnustik.university.solitaire.collections.CardStack;
import net.mrnustik.university.solitaire.collections.CardStacker;

/**
 *
 * @author micha
 */
public class Board {

    private final CardDeck deck;
    private final CardStacker stacker;
    private final CardStacker[] targets;
    private final CardStack[] workingStacks;

    private static final int WORKING_COUNT = 7;
    
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
}
