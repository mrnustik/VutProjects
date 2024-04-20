package ija.ija2016.homework2.model.cards.implementation;

import ija.ija2016.homework2.model.board.AbstractFactorySolitaire;
import ija.ija2016.homework2.model.board.FactoryKlondike;
import ija.ija2016.homework2.model.cards.Card;
import ija.ija2016.homework2.model.cards.CardDeck;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by Mrnda on 3/9/2017.
 */
public class StandardCardDeck implements CardDeck {

    public static CardDeck createStandardCardDeck()
    {
        AbstractFactorySolitaire factory = new FactoryKlondike();
        CardDeck deck = new StandardCardDeck();
        for (int i = 1; i <= 13; i++) { deck.put(factory.createCard(Card.Color.CLUBS,i)); }
        for (int i = 1; i <= 13; i++) { deck.put(factory.createCard(Card.Color.DIAMONDS,i)); }
        for (int i = 1; i <= 13; i++) { deck.put(factory.createCard(Card.Color.HEARTS,i)); }
        for (int i = 1; i <= 13; i++) { deck.put(factory.createCard(Card.Color.SPADES,i)); }

        return deck;
    }

    protected List<Card> mCards = new ArrayList<>();

    @Override
    public Card pop() {
        if(isEmpty()) return null;
        else {
            Card card = mCards.get(mCards.size() - 1);
            mCards.remove(mCards.size() -1);
            return card;
        }
    }

    public int lastIndexOf(Card card)
    {
        return mCards.lastIndexOf(card);
    }

    public void remove(int index)
    {
        mCards.remove(index);
    }


    @Override
    public boolean put(Card card) {
        mCards.add(card);
        return true;
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
    public Card get() {
        if(mCards.isEmpty()) return null;
        return mCards.get(mCards.size() - 1);
    }

    @Override
    public Card get(int index) {
        if(this.size() < index) return null;
        return mCards.get(index);
    }
}
