package net.mrnustik.university.solitaire.model.io.json.converter;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import net.mrnustik.university.solitaire.model.board.Board;
import net.mrnustik.university.solitaire.model.collections.CardDeck;
import net.mrnustik.university.solitaire.model.collections.CardStack;
import net.mrnustik.university.solitaire.model.collections.CardStacker;
import net.mrnustik.university.solitaire.model.io.converter.Converter;
import net.mrnustik.university.solitaire.model.io.json.JsonInterfaceAdapter;

/**
 * Converter for converting Board to JSON format string
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 * @see Converter
 */
public class BoardToJsonConverter implements Converter<Board, String> {

    /**
     * Gson access attribute
     */
    private final Gson gson;

    /**
     * Constructor of the converter.
     */
    public BoardToJsonConverter() {
        GsonBuilder builder = new GsonBuilder();
        builder.registerTypeAdapter(CardDeck.class, new JsonInterfaceAdapter<CardDeck>());
        builder.registerTypeAdapter(CardStack.class, new JsonInterfaceAdapter<CardStack>());
        builder.registerTypeAdapter(CardStacker.class, new JsonInterfaceAdapter<CardStacker>());
        builder.registerTypeAdapter(Board.class, new JsonInterfaceAdapter<Board>());
        builder.setPrettyPrinting();
        builder.setVersion(1.0);
        gson = builder.create();
    }

    /**
     * Converts Board given in param to JSON string
     * @param source board to be converted
     * @return JSON format String representation of Board given as source
     */
    @Override
    public String convert(Board source) {
        return gson.toJson(source);
    }
}
