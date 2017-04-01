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
public class Card {
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
}
