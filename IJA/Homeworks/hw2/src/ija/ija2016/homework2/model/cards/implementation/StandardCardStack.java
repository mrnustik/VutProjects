package ija.ija2016.homework2.model.cards.implementation;

import ija.ija2016.homework2.model.cards.Card;
import ija.ija2016.homework2.model.cards.CardStack;

/**
 * Created by Mrnda on 3/9/2017.
 */
public class StandardCardStack extends StandardCardDeck implements CardStack {
    @Override
    public CardStack pop(Card card) {
        int idx = this.lastIndexOf(card);
        if(idx > 0)
        {
            StandardCardStack stack = new StandardCardStack();
            for(int i = idx; i <  size(); i++)
            {
                stack.put(this.get(i));
            }
            mCards.removeAll(stack.mCards);
            return stack;
        }
        return null;
    }

    @Override
    public boolean put(CardStack stack) {
        if(stack.isEmpty()) return true;
        for(int i = 0; i < stack.size(); i++)
        {
            if(false == this.put(stack.get(i)))
            {
                return false;
            }
        }
        return true;
    }


}
