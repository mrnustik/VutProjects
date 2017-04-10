/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package net.mrnustik.university.solitaire.collections.impl;

import net.mrnustik.university.solitaire.collections.CardDeck;
import net.mrnustik.university.solitaire.collections.CardStacker;
import net.mrnustik.university.solitaire.factory.base.AbstractFactory;
import net.mrnustik.university.solitaire.model.Card;

import java.util.Collections;

/**
 * @author micha
 */
public class SolitaireDeck extends SolitaireStacker implements CardDeck {

    private SolitaireDeck() {
    }

    public static CardDeck createStandardCardDeck(AbstractFactory factory) {
        SolitaireDeck deck = new SolitaireDeck();
        for (int i = 1; i <= 13; i++) {
            deck.put(factory.createCard(Card.Color.CLUBS, i));
        }
        for (int i = 1; i <= 13; i++) {
            deck.put(factory.createCard(Card.Color.SPADES, i));
        }
        for (int i = 1; i <= 13; i++) {
            deck.put(factory.createCard(Card.Color.HEARTS, i));
        }
        for (int i = 1; i <= 13; i++) {
            deck.put(factory.createCard(Card.Color.DIAMONDS, i));
        }
        deck.shuffle();
        return deck;
    }

    @Override
    public boolean pop(CardStacker where) {
        if (!this.isEmpty()) {
            final Card card = this.pop();
            card.turnFaceUp();
            return where.put(card);
        } else if (!where.isEmpty()) {
            while (!where.isEmpty()) {
                Card card = where.pop();
                card.turnFaceDown();
                this.put(card);
            }
        }
        return false;
    }

    @Override
    public void returnCard(CardStacker stacker) {
        Card card = stacker.pop();
        card.turnFaceDown();
        this.put(card);
    }

    private void shuffle() {
        Collections.shuffle(mCards);
    }

}
