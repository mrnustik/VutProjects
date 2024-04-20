/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package net.mrnustik.university.solitaire.model.collections;

import net.mrnustik.university.solitaire.model.card.Card;

/**
 * Representation for stack of cards, where multiple cards can be moved
 *
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public interface CardStack extends CardStacker {

    /**
     * @param card card until what the stack should be taken
     * @return stack of cards until given card
     */
    CardStacker pop(Card card);

    /**
     * @param stack stack to be put on the deck
     * @return true if operation was successful
     */
    boolean put(CardStacker stack);

    /**
     * @param stack puts the stack without checking constraints
     */
    void putWithoutCheck(CardStacker stack);

    /**
     * @param card card untile the stack should be taken
     * @return stacker until that card
     */
    CardStacker getStack(Card card);

    /**
     * @param card Card to get index of
     * @return index of card
     */
    int lastIndexOf(Card card);

    /**
     * @param card puts the card on deck without checking
     */
    void putWithoutCheck(Card card);
}
