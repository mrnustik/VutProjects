package net.mrnustik.university.solitaire.io.json.converter;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import net.mrnustik.university.solitaire.collections.CardDeck;
import net.mrnustik.university.solitaire.collections.CardStack;
import net.mrnustik.university.solitaire.collections.CardStacker;
import net.mrnustik.university.solitaire.io.converter.Converter;
import net.mrnustik.university.solitaire.io.json.JsonInterfaceAdapter;
import net.mrnustik.university.solitaire.model.Board;

/**
 * Created by mrnda on 4/7/2017.
 */
public class BoardToJsonConverter implements Converter<Board, String> {

    private final Gson gson;

    public BoardToJsonConverter() {
        GsonBuilder builder = new GsonBuilder();
        builder.registerTypeAdapter(CardDeck.class, new JsonInterfaceAdapter<CardDeck>());
        builder.registerTypeAdapter(CardStack.class, new JsonInterfaceAdapter<CardStack>());
        builder.registerTypeAdapter(CardStacker.class, new JsonInterfaceAdapter<CardStacker>());
        builder.setPrettyPrinting();
        builder.setVersion(1.0);
        gson = builder.create();
    }

    @Override
    public String convert(Board source) {
        return gson.toJson(source);
    }
}
