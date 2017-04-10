/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package net.mrnustik.university.solitaire.collections.impl;

import net.mrnustik.university.solitaire.collections.CardStacker;
import net.mrnustik.university.solitaire.model.Card;

import java.util.ArrayList;
import java.util.List;

/**
 * @author micha
 */
public class SolitaireStacker implements CardStacker {

    final List<Card> mCards;

    public SolitaireStacker() {
        mCards = new ArrayList<>();
    }

    @Override
    public int size() {
        return mCards.size();
    }

    @Override
    public boolean isEmpty() {
        return mCards.isEmpty();
    }

    @Override
    public Card pop() {
        if (!this.isEmpty()) {
            return this.pop(this.size() - 1);
        }
        return null;
    }

    @Override
    public Card pop(int index) {
        if (!this.isEmpty()) {
            return mCards.remove(index);
        }
        return null;
    }

    @Override
    public Card get() {
        if (!this.isEmpty()) {
            return mCards.get(this.size() - 1);
        }
        return null;
    }

    @Override
    public Card get(int index) {
        if (index < this.size()) {
            return mCards.get(index);
        }
        return null;
    }

    @Override
    public boolean put(Card card) {
        return mCards.add(card);
    }


}
