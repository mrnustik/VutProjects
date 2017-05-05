/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package net.mrnustik.university.solitaire.model.collections.impl;

import net.mrnustik.university.solitaire.model.card.Card;


/**
 * Stacker with one specified target color. Cards only in order by their value can be put there.
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public class TargetStacker extends SolitaireStacker {

    /**
     * Color of which cards can be only put on this stacker
     */
    private final Card.Color mColor;

    /**
     * Creates the stacker with color specified
     * @param mColor color of which cards can be only put on this stacker
     */
    public TargetStacker(Card.Color mColor) {
        this.mColor = mColor;
    }

    /**
     * Tries to put the card on stacker, based on result of {@link TargetStacker#canPut(Card)}
     * @param card Card to be put on the stacker
     * @return true if card was put on the stacker
     */
    @Override
    public boolean put(Card card) {
        return this.canPut(card) && super.put(card);
    }

    /**
     * Checks whether the card has good color and value (top card + 1) to be put on this stacker.
     * @param card card to put on
     * @return true if card can be put on the stacker
     */
    @Override
    public boolean canPut(Card card) {
        if(card.getColor() == this.mColor) {
            if(this.isEmpty()){
                if(card.getValue() == 1){
                    return true;
                }
            } else if(this.get().compareTo(card) == 1) {
                return true;
            }
        }
        return false;
    }
}
