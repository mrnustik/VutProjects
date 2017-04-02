/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package net.mrnustik.university.solitaire.collections.impl;

import net.mrnustik.university.solitaire.model.Card;
import net.mrnustik.university.solitaire.collections.CardDeck;
import net.mrnustik.university.solitaire.collections.CardStack;
import net.mrnustik.university.solitaire.collections.CardStacker;

/**
 *
 * @author micha
 */
public class WorkingStack extends SolitaireStacker implements CardStack {

    public WorkingStack(int count, CardDeck deck) {
        for(int i = 0; i < count; i++)
        {
            super.put(deck.pop());
        }
        get().turnFaceUp();
    }

    
    
    @Override
    public Card pop(int index) {
        if(size() > 1 && size() > index)
        {
            Card card = super.pop(index);
            this.get(index - 1).turnFaceUp();
            return card;
        }
        return super.pop(index);
    }
    
    
    
    @Override
    public CardStacker pop(Card card) {
        int index = mCards.lastIndexOf(card);
        if(index >= 0)
        {
            CardStacker stacker = new SolitaireStacker();
            for(int i = index; i < this.size(); i++) {
                stacker.put(this.get(i));
            }
            for(int i = 0; i < stacker.size(); i++) { this.pop(i+index);}
            return stacker;
        }
        return null;
    }

    @Override
    public boolean put(Card card) {
        if(isEmpty())
        {
            if(card.getValue() == 13) return super.put(card);
        }
        else 
        {
            if(get().compareTo(card) == - 1 && !get().similarColorTo(card)) { 
                return super.put(card);
            }
        }
        return false;
    }

    
    @Override
    public boolean put(CardStacker stack) {
        for(int i = 0; i < stack.size(); i++) {
            if(this.put(stack.get(i)) == false)
            {
                return false;
            }
        }
        return false;
    }
     
}
