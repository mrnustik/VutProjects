/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package net.mrnustik.university.solitaire.collections;

import net.mrnustik.university.solitaire.model.Card;

/**
 *
 * @author micha
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
     * @param card card untile the stack should be taken
     * @return stacker until that card
     */
    CardStacker getStack(Card card);
}
