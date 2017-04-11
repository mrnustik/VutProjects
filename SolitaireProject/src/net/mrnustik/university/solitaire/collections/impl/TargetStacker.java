/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package net.mrnustik.university.solitaire.collections.impl;

import net.mrnustik.university.solitaire.model.Card;

/**
 * @author micha
 */
public class TargetStacker extends SolitaireStacker {

    private final Card.Color mColor;

    public TargetStacker(Card.Color mColor) {
        this.mColor = mColor;
    }

    @Override
    public boolean put(Card card) {
        return this.canPut(card) && super.put(card);
    }

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
