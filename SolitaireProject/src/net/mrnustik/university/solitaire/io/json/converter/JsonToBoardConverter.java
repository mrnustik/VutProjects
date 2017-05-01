package net.mrnustik.university.solitaire.io.json.converter;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import net.mrnustik.university.solitaire.board.Board;
import net.mrnustik.university.solitaire.collections.CardDeck;
import net.mrnustik.university.solitaire.collections.CardStack;
import net.mrnustik.university.solitaire.collections.CardStacker;
import net.mrnustik.university.solitaire.io.converter.Converter;
import net.mrnustik.university.solitaire.io.json.JsonInterfaceAdapter;
import net.mrnustik.university.solitaire.board.BoardImpl;

/**
 * Converter used to convert JSON String to board.
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public class JsonToBoardConverter implements Converter<String, Board> {

    /**
     * Gson access wrapper
     */
    private final Gson gson;

    /**
     * Constructor of the converter
     */
    public JsonToBoardConverter() {
        GsonBuilder builder = new GsonBuilder();
        builder.registerTypeAdapter(CardDeck.class, new JsonInterfaceAdapter<CardDeck>());
        builder.registerTypeAdapter(CardStack.class, new JsonInterfaceAdapter<CardStack>());
        builder.registerTypeAdapter(CardStacker.class, new JsonInterfaceAdapter<CardStacker>());
        builder.registerTypeAdapter(Board.class, new JsonInterfaceAdapter<Board>());
        builder.setVersion(1.0);
        this.gson = builder.create();
    }

    /**
     * Converts JSON string into Board.
     * @param source JSON format string representing Board
     * @return board represented in source
     */
    @Override
    public Board convert(String source) {
        return gson.fromJson(source, BoardImpl.class);
    }
}
