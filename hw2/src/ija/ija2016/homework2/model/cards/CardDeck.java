package ija.ija2016.homework2.model.cards;


/**
 * Created by Mrnda on 3/9/2017.
 */
public interface CardDeck {


    Card pop();

    boolean put(Card card);

    int size();

    boolean isEmpty();

    Card get();

    Card get(int index);
}
