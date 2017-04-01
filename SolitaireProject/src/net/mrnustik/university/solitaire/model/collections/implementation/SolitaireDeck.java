/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package net.mrnustik.university.solitaire.model.collections.implementation;

import net.mrnustik.university.solitaire.model.Card;
import net.mrnustik.university.solitaire.model.collections.CardDeck;
import net.mrnustik.university.solitaire.model.collections.CardStack;

/**
 *
 * @author micha
 */
public class SolitaireDeck extends SolitaireStacker implements CardDeck{

    @Override
    public boolean pop(CardStack where) {
        if(!this.isEmpty()) {
            final Card card = this.pop();
            card.turnFaceUp();
            return where.put(card);
        } else if (where.isEmpty()){ 
            while(where.isEmpty() != true){
                Card card = where.pop();
                card.turnFaceDown();
                this.put(card);
            }
        }
        return false;
    }
    
}
