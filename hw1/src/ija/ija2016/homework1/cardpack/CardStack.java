package ija.ija2016.homework1.cardpack;


import java.util.ArrayList;
import java.util.List;

public class CardStack {


    protected List<Card> mCards;

    public CardStack(int size)
    {
        this.mCards = new ArrayList<>(size);
    }


    @Override
    public boolean equals(Object obj) {
        if(obj instanceof CardStack)
        {
            CardStack stack = (CardStack) obj;
            return stack.mCards.equals(mCards);
        }
        return false;
    }

    public boolean isEmpty()
    {
        return mCards.isEmpty();
    }

    public void put(Card card)
    {
        mCards.add(card);
    }

    public void put(CardStack stack) {
        if (stack != null) {
            mCards.addAll(stack.mCards);
        }
    }

    public Card pop()
    {
        if(!isEmpty())
        {
            Card card = mCards.get(mCards.size() - 1);
            mCards.remove(card);
            return card;
        }
        else
        {
            return null;
        }
    }

    public CardStack takeFrom(Card card)
    {
        int index = mCards.indexOf(card);
        if(index > 0)
        {
            CardStack stack = new CardStack(mCards.size() - index);
            for(index = index; index < mCards.size(); index++)
            {
                stack.put(mCards.get(index));
            }
            mCards.removeAll(stack.mCards);
            return stack;
        }
        return null;
    }

    public int size()
    {
        return mCards.size();
    }


}