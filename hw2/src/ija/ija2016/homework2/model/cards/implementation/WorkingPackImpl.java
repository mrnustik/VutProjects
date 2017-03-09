package ija.ija2016.homework2.model.cards.implementation;

import ija.ija2016.homework2.model.cards.Card;
import ija.ija2016.homework2.model.cards.CardStack;

/**
 * Created by Mrnda on 3/9/2017.
 */
public class WorkingPackImpl extends CardStackImpl {

    @Override
    public boolean put(Card card) {
        if(isEmpty())
        {
            if(card.value() != 13)
                return false;
            super.put(card);
            return true;
        }
        if(get().color().similarColorTo(card.color()))
        {
            return false;
        }
        else if(get().value() == card.value() + 1)
        {
            return super.put(card);
        }
        return false;
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
