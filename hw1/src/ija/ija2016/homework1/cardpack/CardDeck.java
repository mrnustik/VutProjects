package ija.ija2016.homework1.cardpack;

import java.util.Set;
import java.util.HashSet;

public class CardDeck {


    public static CardDeck createStandardDeck()
    {
        CardDeck deck = new CardDeck(52);
        for (int i = 1; i <= 13; i++) { deck.put(new Card(Card.Color.CLUBS,i)); }
        for (int i = 1; i <= 13; i++) { deck.put(new Card(Card.Color.DIAMONDS,i)); }
        for (int i = 1; i <= 13; i++) { deck.put(new Card(Card.Color.HEARTS,i)); }
        for (int i = 1; i <= 13; i++) { deck.put(new Card(Card.Color.SPADES,i)); }
        return deck;
    }

    private CardStack stack;

    public CardDeck(int size)
    {
        stack = new CardStack(size);
    }

    public int size()
    {
        return stack.size();
    }

    public void put(Card card)
    {
        stack.put(card);
    }

    public Card pop()
    {
        return stack.pop();
    }
}
