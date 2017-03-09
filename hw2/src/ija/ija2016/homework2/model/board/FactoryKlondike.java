package ija.ija2016.homework2.model.board;

import ija.ija2016.homework2.model.cards.Card;
import ija.ija2016.homework2.model.cards.CardDeck;
import ija.ija2016.homework2.model.cards.CardStack;
import ija.ija2016.homework2.model.cards.implementation.*;

/**
 * Created by Mrnda on 3/9/2017.
 */
public class FactoryKlondike extends AbstractFactorySolitaire {


    public Card createCard(Card.Color color, int value) {
        if(value < 1 || value > 13) return null;
        return new CardImpl(color, value);
    }

    @Override
    public CardDeck createCardDeck() {
        return CardDeckImpl.createStandardCardDeck();
    }

    @Override
    public CardDeck createTargetPack(Card.Color color) {
        return new TargetCardDeckImpl(color);
    }

    @Override
    public CardStack createWorkingPack() {
        return new WorkingPackImpl();
    }
}
