package ija.ija2016.homework2.model.cards;

/**
 * Created by Mrnda on 3/9/2017.
 */
public class Card {
    public enum Color {
        CLUBS,
        DIAMONDS,
        HEARTS,
        SPADES

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

}
