package ija.ija2016.homework2.model.cards;

/**
 * Created by Mrnda on 3/9/2017.
 */
public class Card {
    public enum Color {
        CLUBS,
        DIAMONDS,
        HEARTS,
        SPADES;

        boolean similarColorTo(Card.Color color)
        {
            switch(this)
            {
                case CLUBS:
                case SPADES:
                    if(color == CLUBS || color == SPADES)
                        return true;
                    break;
                case DIAMONDS:
                case HEARTS:
                    if(color == DIAMONDS || color == HEARTS)
                        return true;
                    break;
            }
            return false;
        }
    }

    private Color mColor;
    private int mValue;
    private boolean mIsFaceUp = false;

    public Card(Color color, int value)
    {
        this.mColor = color;
        this.mValue = value;
    }

    public Color color()
    {
        return mColor;
    }

    public int value()
    {
        return mValue;
    }

    public boolean isTurnedFaceUp()
    {
        return mIsFaceUp;
    }

    public void turnFaceUp()
    {
        mIsFaceUp = true;
    }

    public boolean similarColorTo(Card card)
    {
        return color().similarColorTo(card.color());
    }

    public int compareValue(Card card)
    {
        return value() - card.value();
    }



}
