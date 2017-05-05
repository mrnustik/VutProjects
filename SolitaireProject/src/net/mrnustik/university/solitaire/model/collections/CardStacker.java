/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package net.mrnustik.university.solitaire.model.collections;

import net.mrnustik.university.solitaire.model.card.Card;

/**
 * Representation of a card stack where only one card can be moved at a time
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
@SuppressWarnings("BooleanMethodIsAlwaysInverted")
public interface CardStacker {

    /**
     * @return return size of stacker
     */
    int size();

    /**
     * @return true if Stacker is empty
     */
    boolean isEmpty();

    /**
     * @return returns top card and removes it from stack
     */
    Card pop();

    /**
     * @param index index of the card
     * @return pops the card from given index
     */
    Card pop(int index);

    /**
     * @return returns top card
     */
    Card get();

    /**
     * @param index index of the card
     * @return returns card on given index
     */
    Card get(int index);

    /**
     * @param card Card to be put on the stacker
     * @return true if operation was successful
     */
    boolean put(Card card);

    /**
     * @param card card to put on
     * @return true if card can be put on
     */
    boolean canPut(Card card);

}
