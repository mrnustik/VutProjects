/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package net.mrnustik.university.solitaire.collections;

import net.mrnustik.university.solitaire.model.Card;

/**
 * Representation of a standard card deck
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public interface CardDeck {

    /**
     * @return pops the first card from the stack
     */
    Card pop();


    /**
     * @return returns first card from the stack without deleting it
     */
    Card get();

    /**
     * Pops card from CardDeck to CardStack from param.
     * If isEmpty() gets all card from stack to itself
     * @param where where to put card
     * @return false if is empty
     */
    boolean pop(CardStacker where);

    /**
     * @param card returned card
     */
    void returnCard(CardStacker card);

    /**
     * @return true if the deck is empty
     */
    boolean isEmpty();
}
