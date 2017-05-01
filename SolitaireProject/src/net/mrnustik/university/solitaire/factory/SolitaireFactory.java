/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package net.mrnustik.university.solitaire.factory;

import net.mrnustik.university.solitaire.collections.CardDeck;
import net.mrnustik.university.solitaire.collections.CardStack;
import net.mrnustik.university.solitaire.collections.CardStacker;
import net.mrnustik.university.solitaire.collections.impl.SolitaireDeck;
import net.mrnustik.university.solitaire.collections.impl.SolitaireStacker;
import net.mrnustik.university.solitaire.collections.impl.TargetStacker;
import net.mrnustik.university.solitaire.collections.impl.WorkingStack;
import net.mrnustik.university.solitaire.factory.base.AbstractFactory;
import net.mrnustik.university.solitaire.model.Card;

/**
 * Factory design pattern used for creation of basic objects inside the game.
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public class SolitaireFactory extends AbstractFactory {

    /**
     * Creates card with specified color and value
     * @param color color of the card
     * @param value value of the card (1 - 13)
     * @return specified card or null if value is not between 1  and 13
     */
    @Override
    public Card createCard(Card.Color color, int value) {
        if (value < 1 && value > 13) return null;
        return new Card(color, value);
    }

    /**
     * @inheritDoc
     */
    @Override
    public CardDeck createCardDeck() {
        return SolitaireDeck.createStandardCardDeck(this);
    }

    /**
     * @inheritDoc
     */
    @Override
    public CardStacker createTargetStacker(Card.Color color) {
        return new TargetStacker(color);
    }

    /**
     * @inheritDoc
     */
    @Override
    public CardStacker createPutDownStacker() {
        return new SolitaireStacker();
    }

    /**
     * @inheritDoc
     */
    @Override
    public CardStack createWorkingPack(int count, CardDeck deck) {
        return new WorkingStack(count, deck);
    }

}
