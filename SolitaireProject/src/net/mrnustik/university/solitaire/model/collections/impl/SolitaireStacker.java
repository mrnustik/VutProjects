/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package net.mrnustik.university.solitaire.model.collections.impl;

import net.mrnustik.university.solitaire.model.collections.CardStacker;
import net.mrnustik.university.solitaire.model.card.Card;

import java.util.ArrayList;
import java.util.List;

/**
 * Put off stacker used for basic one card operations
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public class SolitaireStacker implements CardStacker {

    /**
     * List of cards in stacker
     */
    final List<Card> mCards;

    /**
     * Creates the stacker
     */
    public SolitaireStacker() {
        mCards = new ArrayList<>();
    }

    /**
     * Retrieves count of cards in stacker
     * @return count of cards in stacker
     */
    @Override
    public int size() {
        return mCards.size();
    }

    /**
     * Checks whether stacker is empty
     * @return true if stacker is empty
     */
    @Override
    public boolean isEmpty() {
        return mCards.isEmpty();
    }

    /**
     * Removes top card from stacker
     * @return returns top card or null if empty
     */
    @Override
    public Card pop() {
        if (!this.isEmpty()) {
            return this.pop(this.size() - 1);
        }
        return null;
    }

    /**
     * Removes card on specified index from stacker
     * @param index index of the card
     * @return specified card or null
     */
    @Override
    public Card pop(int index) {
        if (!this.isEmpty()) {
            return mCards.remove(index);
        }
        return null;
    }

    /**
     * Retrieves the top card from stacker without removing it
     * @return top card of the stacker or null if empty
     */
    @Override
    public Card get() {
        if (!this.isEmpty()) {
            return mCards.get(this.size() - 1);
        }
        return null;
    }

    /**
     * Retrieves the card specified in index from stacker without removing it
     * @param index index of the card
     * @return specified card or null if the index is too big
     */
    @Override
    public Card get(int index) {
        if (index < this.size()) {
            return mCards.get(index);
        }
        return null;
    }

    /**
     * Adds card on stack
     * @param card Card to be put on the stacker
     * @return true if card can be put in there
     */
    @Override
    public boolean put(Card card) {
        return mCards.add(card);
    }

    /**
     * Checks whether the card can be put on the stack
     * @param card card to put on
     * @return always true
     */
    @Override
    public boolean canPut(Card card) {
        return true;
    }


}
