package ija.ija2016.homework2.model.cards;

/**
 * Created by Mrnda on 3/9/2017.
 */
public interface Card {
    public enum Color {
        CLUBS {
            @Override
            public String toString() {
                return "C";
            }
        },
        DIAMONDS{
            @Override
            public String toString() {
                return "D";
            }
        },
        HEARTS{
            @Override
            public String toString() {
                return "H";
            }
        },
        SPADES{
            @Override
            public String toString() {
                return "S";
            }
        };

        public boolean similarColorTo(Card.Color color)
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


    Color color();

    int value();

    boolean isTurnedFaceUp();

    boolean turnFaceUp();

    boolean similarColorTo(Card card);

    int compareValue(Card card);


}
