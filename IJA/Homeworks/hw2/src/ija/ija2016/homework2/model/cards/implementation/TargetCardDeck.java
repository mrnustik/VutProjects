package ija.ija2016.homework2.model.cards.implementation;

import ija.ija2016.homework2.model.cards.Card;

/**
 * Created by Mrnda on 3/9/2017.
 */
public class TargetCardDeck extends StandardCardDeck {

    private Card.Color mColor;

    public TargetCardDeck(Card.Color color)
    {
        this.mColor = color;
    }

    @Override
    public boolean put(Card card) {
        if(card.color() != mColor) return false;
        if(isEmpty())
        {
            if(card.value() == 1)
                return super.put(card);
            else
                return false;
        }
        if(card.value() == get().value() + 1)
        {
            return super.put(card);
        }
        return false;
    }
}
