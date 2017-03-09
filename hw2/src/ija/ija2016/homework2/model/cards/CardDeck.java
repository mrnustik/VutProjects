package ija.ija2016.homework2.model.cards;

import com.sun.javafx.scene.control.skin.VirtualFlow;

import java.util.ArrayList;
import java.util.List;

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
