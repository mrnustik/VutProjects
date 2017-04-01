/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package net.mrnustik.university.solitaire.model.collections;

import net.mrnustik.university.solitaire.model.Card;

/**
 *
 * @author micha
 */
public interface CardDeck {
    
    /**
     * @return pops the first card from the stack
     */
    Card pop();
    
    /**
     * Pops card from CardDeck to CardStack from param. 
     * If isEmpty() gets all card from stack to itself. 
     * @param where where to put card
     * @return false if is empty
     */
    boolean pop(CardStack where);
}
