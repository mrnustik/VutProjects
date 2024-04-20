/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package net.mrnustik.university.solitaire.model.collections.impl;

import net.mrnustik.university.solitaire.model.collections.CardDeck;
import net.mrnustik.university.solitaire.model.collections.CardStack;
import net.mrnustik.university.solitaire.model.collections.CardStacker;
import net.mrnustik.university.solitaire.model.card.Card;

/**
 * Stack where multiple cards can be moved from/to.
 * Cards can be put on this only if their color is not similar to the top card and have greater value.
 *
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public class WorkingStack extends SolitaireStacker implements CardStack {

    /**
     * Creates the stack and takes number of cards (specified in count) from the deck.
     * Also turns the top Card face up
     *
     * @param count of the cards to be taken
     * @param deck  where the cards should be taken from
     */
    public WorkingStack(int count, CardDeck deck) {
        for (int i = 0; i < count; i++) {
            super.put(deck.pop());
        }
        get().turnFaceUp();
    }

    /**
     * Removes card specified in index from the stacker.
     * Also turns the card below face up.
     *
     * @param index index of the card
     * @return card on the index or null if none exists
     */
    @Override
    public Card pop(int index) {
        if (size() > 1 && size() > index) {
            Card card = super.pop(index);
            this.get(index - 1).turnFaceUp();
            return card;
        }
        return super.pop(index);
    }

    /**
     * Removes stack of cards from the stacker. Card specified in param will be the last card taken.
     *
     * @param card card until what the stack should be taken
     * @return stack of cards until specified card or null if the card is not there
     */
    @Override
    public CardStacker pop(Card card) {
        int index = lastIndexOf(card);
        if (index >= 0) {
            CardStacker stacker = new SolitaireStacker();
            for (int i = index; i < this.size(); i++) {
                stacker.put(this.get(i));
            }
            for (int i = this.size() - 1; i >= index; i--) {
                this.pop(i);
            }
            return stacker;
        }
        return null;
    }

    /**
     * Puts the card on the stack, if it is possible.
     *
     * @param card Card to be put on the stack
     * @return true if card was put on the stack
     */
    @Override
    public boolean put(Card card) {
        return canPut(card) && super.put(card);
    }

    /**
     * Puts the stack of card on the stacker, if it possible.
     *
     * @param stack stack to be put on the deck
     * @return true if the stack was put on the stack
     */
    @Override
    public boolean put(CardStacker stack) {
        for (int i = 0; i < stack.size(); i++) {
            if (!this.put(stack.get(i))) {
                return false;
            }
        }
        return true;
    }

    /**
     * Puts the stack on stack without checking constraints
     *
     * @param stack to be put on
     */
    @Override
    public void putWithoutCheck(CardStacker stack) {
        for (int i = 0; i < stack.size(); i++) {

            super.put(stack.get(i));
        }
    }

    /**
     * Gets the stack of cards until specified cards without removing it
     *
     * @param card card until the stack should be taken
     * @return stack of cards if card from param is present else return null
     */
    @Override
    public CardStacker getStack(Card card) {
        int index = lastIndexOf(card);
        if (index >= 0) {
            CardStacker stacker = new SolitaireStacker();
            for (int i = index; i < this.size(); i++) {
                stacker.put(this.get(i));
            }
            return stacker;
        }
        return null;
    }

    /**
     * Returns last index of card present in the stack
     *
     * @param card to get index of
     * @return index of the last card from parameters
     */
    @Override
    public int lastIndexOf(Card card) {
        return mCards.lastIndexOf(card);
    }

    /**
     * Puts card without checking
     *
     * @param card puts the card on deck without checking
     */
    @Override
    public void putWithoutCheck(Card card) {
        super.put(card);
    }

    /**
     * Counts the hashcode of stack
     *
     * @return hashcode of stack
     */
    @Override
    public int hashCode() {
        return this.mCards.hashCode();
    }

    /**
     * Checks whether two stacks are equal
     *
     * @param obj object to be compared to
     * @return true if stacks are equal
     */
    @Override
    public boolean equals(Object obj) {
        if (obj instanceof WorkingStack) {
            WorkingStack stack = (WorkingStack) obj;
            return this.mCards.equals(stack.mCards);
        }
        return false;
    }

    /**
     * Checks whether card can be put on the stack.
     * Constraints are:
     * * card must not have similar color to the card on top
     * * card must have value of the top card + 1
     * * if the stack is empty only king can be put
     *
     * @param card card to put on
     * @return true if card passed through the constraints
     */
    @Override
    public boolean canPut(Card card) {
        if (isEmpty()) {
            if (card.getValue() == 13) return true;
        } else {
            if (get().compareTo(card) == -1 && !get().similarColorTo(card)) {
                return true;
            }
        }
        return false;
    }
}
