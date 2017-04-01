/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package net.mrnustik.university.solitaire.model;

/**
 *
 * @author micha
 */
public class Card implements Comparable{

    public enum Color {
        HEARTS,
        DIAMONDS,
        SPADES, 
        CLUBS;
        
        public boolean similarColorTo(Color color) {
            switch(this)
            {
                case HEARTS:
                case DIAMONDS:
                    return color == HEARTS || color == DIAMONDS;
                case SPADES:
                case CLUBS:
                default:
                    return color == SPADES || color == CLUBS;
            }
        }
    }
    
    private Color mColor;
    private int mValue;
    private boolean mFaceUp = false;
    
    public Card(Color color, int value) {
        this.mColor = color;
        this.mValue = value;
    }

    public Color getColor() {
        return mColor;
    }

    public int getValue() {
        return mValue;
    }

    public boolean isFaceUp() {
        return mFaceUp;
    }

    public void turnFaceUp() {
        mFaceUp = true;
    }
    
    public void turnFaceDown() {
        mFaceUp = false;
    }
    
    public void turn() {
        mFaceUp = !mFaceUp;
    }
    
    @Override
    public int compareTo(Object o) {
        if(o instanceof Card)
        {
            Card card = (Card) o;
            return card.getValue() - this.getValue();
        }
        return Integer.MAX_VALUE;
    }

    @Override
    public boolean equals(Object obj) {
        if(obj instanceof Card)
        {
            Card card = (Card) obj;
            return card.getValue() == this.getValue() 
                    && card.getColor() == this.getColor();
        }
        return false;
    }

    @Override
    public int hashCode() {
        return getColor().hashCode() + getValue();
    }
    
    
    
}
