package ija.ija2016.homework1.cardpack;


public class Card {
    public enum Color {
        CLUBS("C"),
        DIAMONDS("D"),
        HEARTS("H"),
        SPADES("S");

        private String mStringValue;

        private Color(String strValue)
        {
            this.mStringValue = strValue;
        }

        @Override
        public String toString() {
            return mStringValue;
        }
    }

    private Card.Color mColor;
    private int mValue;

    public Card(Card.Color color, int value)
    {
        this.mColor = color;
        this.mValue = value;
    }

    public Card.Color color()
    {
        return mColor;
    }

    public int value()
    {
        return mValue;
    }

    @Override
    public String toString() {
        if(value() != 1 && value() < 11)
        {
            return String.format("%d(%s)", value(), mColor.toString());
        }
        else
        {
            char shortcut = ' ';
            switch (value())
            {
                case 1:
                    shortcut = 'A';
                    break;
                case 11:
                    shortcut = 'J';
                    break;
                case 12:
                    shortcut = 'Q';
                    break;
                case 13:
                    shortcut = 'K';
                    break;
            }
            return String.format("%c(%s)", shortcut, mColor.toString());
        }
    }
}
