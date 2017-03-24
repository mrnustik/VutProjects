package ija.ija2016.homework2.model.cards;

/**
 * Created by Mrnda on 3/9/2017.
 */
public interface CardStack extends CardDeck {
    CardStack pop(Card card);

    boolean put(CardStack stack);
}
