/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package net.mrnustik.university.solitaire.collections.impl;

import net.mrnustik.university.solitaire.collections.CardDeck;
import net.mrnustik.university.solitaire.collections.CardStack;
import net.mrnustik.university.solitaire.collections.CardStacker;
import net.mrnustik.university.solitaire.model.Card;

/**
 * @author micha
 */
public class WorkingStack extends SolitaireStacker implements CardStack {

    public WorkingStack(int count, CardDeck deck) {
        for (int i = 0; i < count; i++) {
            super.put(deck.pop());
        }
        get().turnFaceUp();
    }


    @Override
    public Card pop(int index) {
        if (size() > 1 && size() > index) {
            Card card = super.pop(index);
            this.get(index - 1).turnFaceUp();
            return card;
        }
        return super.pop(index);
    }


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

    @Override
    public boolean put(Card card) {
        return canPut(card) && super.put(card);
    }


    @Override
    public boolean put(CardStacker stack) {
        for (int i = 0; i < stack.size(); i++) {
            if (!this.put(stack.get(i))) {
                return false;
            }
        }
        return true;
    }

    @Override
    public void putWithoutCheck(CardStacker stack) {
        for (int i = 0; i < stack.size(); i++) {

            super.put(stack.get(i));
        }
    }

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

    @Override
    public int lastIndexOf(Card card) {
        return mCards.lastIndexOf(card);
    }

    @Override
    public void putWithoutCheck(Card card) {
        super.put(card);
    }


    @Override
    public boolean canPut(Card card) {
        if(isEmpty()){
            if(card.getValue() == 13) return true;
        } else {
            if (get().compareTo(card) == -1 && !get().similarColorTo(card)) {
                return true;
            }
        }
        return false;
    }
}
