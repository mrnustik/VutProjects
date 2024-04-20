/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package net.mrnustik.university.solitaire.model.collections.impl;

import net.mrnustik.university.solitaire.model.collections.CardDeck;
import net.mrnustik.university.solitaire.model.collections.CardStacker;
import net.mrnustik.university.solitaire.model.board.factory.base.AbstractFactory;
import net.mrnustik.university.solitaire.model.card.Card;

import java.util.Collections;

/**
 * Standard card deck used for a game of Solitaire (Klondike)
 *
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public class SolitaireDeck extends SolitaireStacker implements CardDeck {

    /**
     * Private constructor so the factory method will be used
     */
    private SolitaireDeck() {
    }

    /**
     * Factory method used for creation of standard (52 card) deck
     *
     * @param factory used for creation of cards
     * @return standard card deck with 52 cards
     */
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

    /**
     * Removes first card from deck and puts it on specified stacker.
     * If deck is empty it tries to get all cards from the stacker back.
     *
     * @param where where to put card
     * @return true if both stack and deck are not empty
     */
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

    /**
     * Returns card from specified stacker onto deck.
     *
     * @param stacker where the card should be taken from
     */
    @Override
    public void returnCard(CardStacker stacker) {
        Card card = stacker.pop();
        if (card != null) {
            card.turnFaceDown();
            this.put(card);
        }
    }

    /**
     * Randomly shuffles card in deck.
     */
    private void shuffle() {
        Collections.shuffle(mCards);
    }

}
