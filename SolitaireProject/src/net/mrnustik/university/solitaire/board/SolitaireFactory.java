/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package net.mrnustik.university.solitaire.board;

import net.mrnustik.university.solitaire.board.base.AbstractFactory;
import net.mrnustik.university.solitaire.model.Card;
import net.mrnustik.university.solitaire.model.collections.CardDeck;
import net.mrnustik.university.solitaire.model.collections.CardStack;
import net.mrnustik.university.solitaire.model.collections.CardStacker;
import net.mrnustik.university.solitaire.model.collections.implementation.SolitaireDeck;
import net.mrnustik.university.solitaire.model.collections.implementation.SolitaireStacker;
import net.mrnustik.university.solitaire.model.collections.implementation.TargetStacker;
import net.mrnustik.university.solitaire.model.collections.implementation.WorkingStack;

/**
 *
 * @author micha
 */
public class SolitaireFactory extends AbstractFactory {

    @Override
    public Card createCard(Card.Color color, int value) {
        if(value < 1 && value > 13) return null;
        return new Card(color, value);
    }

    @Override
    public CardDeck createCardDeck() {
        return SolitaireDeck.createStandardCardDeck(this);
    }

    @Override
    public CardStacker createTargetStacker(Card.Color color) {
        return new TargetStacker(color);
    }

    @Override
    public CardStacker createPutDownStacker() {
        return new SolitaireStacker();
    }

    @Override
    public CardStack createWorkingPack(int count, CardDeck deck) {
        return new WorkingStack(count, deck);
    }
    
}
