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
    private final CardDeck mDeck;
    private final CardStacker mStacker;
    private final CardStacker[] mTargets;
    private final CardStack[] mWorkingStacks;

    private static final int WORKING_COUNT = 7;
    
    public Board(AbstractFactory factory) {
        this.mDeck = factory.createCardDeck();
        this.mStacker = factory.createPutDownStacker();
        this.mTargets = new CardStacker[4];
        for(int i = 0; i < this.mTargets.length; i++) {
            this.mTargets[i] = factory.createTargetStacker(Card.Color.values()[i]);
        }
        this.mWorkingStacks = new CardStack[WORKING_COUNT];
        for(int i = 0; i< this.mWorkingStacks.length; i++){
            this.mWorkingStacks[i] = factory.createWorkingPack(i+1, this.mDeck);
        }
    }
}
