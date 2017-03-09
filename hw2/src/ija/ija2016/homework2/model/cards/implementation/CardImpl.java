package ija.ija2016.homework2.model.cards.implementation;

import ija.ija2016.homework2.model.cards.Card;

/**
 * Created by Mrnda on 3/9/2017.
 */
public class CardImpl implements Card {

    private Color mColor;
    private int mValue;
    private boolean mIsFaceUp = false;

    public CardImpl(Color color, int value) {
        mColor = color;
        mValue = value;
    }

    @Override
    public boolean equals(Object obj) {
        if(obj instanceof Card)
        {
            Card card = (Card) obj;
            return card.color() == this.color() && card.value() == this.value();
        }
        return false;
    }

    @Override
    public int hashCode() {
        return color().hashCode() + value();
    }

    @Override
    public Color color() {
        return mColor;
    }

    @Override
    public int value() {
        return mValue;
    }

    @Override
    public boolean isTurnedFaceUp() {
        return mIsFaceUp;
    }

    @Override
    public boolean turnFaceUp() {
        if(false == mIsFaceUp)
        {
            mIsFaceUp = true;
            return true;
        }
        return false;
    }

    @Override
    public boolean similarColorTo(Card card) {
        return card.color().similarColorTo(this.color());
    }

    @Override
    public int compareValue(Card card) {
        return this.value() - card.value();
    }
}
