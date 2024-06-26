/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package net.mrnustik.university.solitaire.model.board.factory.base;

import net.mrnustik.university.solitaire.model.collections.CardDeck;
import net.mrnustik.university.solitaire.model.collections.CardStack;
import net.mrnustik.university.solitaire.model.collections.CardStacker;
import net.mrnustik.university.solitaire.model.card.Card;

/**
 * Abstract factory design pattern used for creation of basic objects inside the game.
 *
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public abstract class AbstractFactory {
    /**
     * @param color color of the card
     * @param value value of the card (1 - 13)
     * @return card with defined color and value
     */
    public abstract Card createCard(Card.Color color, int value);

    /**
     * @return card deck that will be used the whole game
     */
    public abstract CardDeck createCardDeck();

    /**
     * @param color color that can be put on this deck
     * @return target stacker for specified color
     */
    public abstract CardStacker createTargetStacker(Card.Color color);

    /**
     * @return creates putdown stacker
     */
    public abstract CardStacker createPutDownStacker();

    /**
     * @param count count of base cards
     * @param deck  deck where the cards will be took from
     * @return working pack
     */
    public abstract CardStack createWorkingPack(int count, CardDeck deck);
}
