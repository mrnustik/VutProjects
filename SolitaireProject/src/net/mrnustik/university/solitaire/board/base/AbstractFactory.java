/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package net.mrnustik.university.solitaire.board.base;

import net.mrnustik.university.solitaire.model.Card;
import net.mrnustik.university.solitaire.model.collections.CardDeck;
import net.mrnustik.university.solitaire.model.collections.CardStack;
import net.mrnustik.university.solitaire.model.collections.CardStacker;

/**
 *
 * @author micha
 */
public abstract class AbstractFactory {
   /** 
    * @param color color of the card 
    * @param value value of the card (1 - 13)
    * @return card with defined color and value
    */ 
   public abstract Card createCard(Card.Color color , int value);
   
   /**
    * @return card deck that will be used the whole game
    */
   public abstract CardDeck createCardDeck();
   
   /**
    * @param color color that can be put on this deck
    * @return target stacker for specified color
    */
   public abstract CardStacker createTargetStacker(Card.Color color);
   
   /**
    * @return creates putdown stacker
    */
   public abstract CardStacker createPutDownStacker();
   
   /**
    * @return creates working pack
    */
   public abstract CardStack createWorkingPack();
}
