/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package net.mrnustik.university.solitaire.model.card;


/**
 * Class representing one classic playing Card.
 * @version 1.0
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public class Card implements Comparable {

    /**
     * Color of the Card.
     * @see Card.Color
     */
    private final Color mColor;

    /**
     * Value of the card 2-10 are classic cards, 1 is ACE, 11 Jack, 12 Queen 13 King
     */
    private final int mValue;

    /**
     * Boolean indicating whether the card is turned face up.
     */
    private boolean mFaceUp = false;

    /**
     * Basic constructor for Card creation
     * @param color of the card
     * @param value of the card
     */
    public Card(Color color, int value) {
        this.mColor = color;
        this.mValue = value;
    }

    /**
     * Getter method for Card color getting
     * @return card color {@link Card#mValue}
     * @see Card.Color
     */
    public Color getColor() {
        return mColor;
    }

    /**
     * Getter method for Card value getting
     * @return card value {@link Card#mValue}
     */
    public int getValue() {
        return mValue;
    }

    /**
     * Getter method for Card face up indicator
     * @return true if Card lies face up {@link Card#mFaceUp}
     */
    public boolean isFaceUp() {
        return mFaceUp;
    }

    /**
     * Turns the Card face up.
     * Sets the face up attribute to true {@link Card#mFaceUp}
     */
    public void turnFaceUp() {
        mFaceUp = true;
    }

    /**
     * Turns the Card face down.
     * Sets the face up attribute to false {@link Card#mFaceUp}
     */
    public void turnFaceDown() {
        mFaceUp = false;
    }

    /**
     * Switches the cards side. It turns face up if is face down and the other way.
     * Sets the face up attribute {@link Card#mFaceUp}
     */
    public void turn() {
        mFaceUp = !mFaceUp;
    }

    /**
     * Method for checking whether the card is the same base color as card given in parameters.
     * @param card to be compared with
     * @return true if both cards are red or black, false otherwise
     * @see Card.Color#similarColorTo(Card.Color)
     */
    public boolean similarColorTo(Card card) {
        return card.getColor().similarColorTo(this.getColor());
    }

    /**
     * Compares two cards based on their value.
     * @param o card that this card should be compared to
     * @return value of given card minus value of this card
     * @see Card#getValue()
     */
    @Override
    public int compareTo(Object o) {
        if (o instanceof Card) {
            Card card = (Card) o;
            return card.getValue() - this.getValue();
        }
        return Integer.MAX_VALUE;
    }

    /**
     * Checks if two Cards are equal.
     * @param obj card to be equaled to
     * @return true if card have same color and value
     */
    @Override
    public boolean equals(Object obj) {
        if (obj instanceof Card) {
            Card card = (Card) obj;
            return card.getValue() == this.getValue()
                    && card.getColor() == this.getColor();
        }
        return false;
    }

    /**
     * Hashcode computation method for this card
     * Counted as hashCode of color + value of the card.
     * @return computed hash code.
     * @see Card#getColor()
     * @see Card#getValue()
     */
    @Override
    public int hashCode() {
        return getColor().hashCode() + getValue();
    }

    /**
     * Method for getting string representation of this Card.
     * Format of the output is VALUE_COLOR.
     * @return string representation of object
     * @see Card#getValue()
     * @see Card#getColor()
     */
    @Override
    public String toString() {
        if (getValue() != 1 && getValue() < 11) return this.getColor().toString() + "_" + this.getValue();
        else {
            char c = ' ';
            if (getValue() == 11)
                c = 'J';
            else if (getValue() == 12)
                c = 'Q';
            else if (getValue() == 13)
                c = 'K';
            else if (getValue() == 1)
                c = 'A';
            return this.getColor().toString() + "_" + c;
        }
    }

    /**
     * Enumerator used for representing Colors of a classic playing Card.
     */
    public enum Color {
        /**
         * Hearts card color(♥)
         */
        HEARTS,
        /**
         * Diamonds card color(♦)
         */
        DIAMONDS,
        /**
         * Spades card color(♠)
         */
        SPADES,
        /**
         * Clubs card color(♣)
         */
        CLUBS;

        /**
         * Method used to check whether two colors are similar.
         * Similar is meant in a classic color world (red and black)
         * @param color to be compared to
         * @return true if both colors are red or black, false otherwise
         */
        public boolean similarColorTo(Color color) {
            switch (this) {
                case HEARTS:
                case DIAMONDS:
                    return color == HEARTS || color == DIAMONDS;
                case SPADES:
                case CLUBS:
                default:
                    return color == SPADES || color == CLUBS;
            }
        }

        /**
         * Method to retrieve String represenation of Color
         * @return string representation of color
         * @see String
         */
        @Override
        public String toString() {
            switch (this) {
                case HEARTS:
                    return "HEARTS";
                case DIAMONDS:
                    return "DIAMONDS";
                case SPADES:
                    return "SPADES";
                case CLUBS:
                    return "CLUBS";
                default:
                    return "";
            }
        }
    }
}
