package net.mrnustik.university.solitaire.io;

import net.mrnustik.university.solitaire.model.Board;

import java.io.IOException;


/**
 * Created by mrnda on 4/7/2017.
 */
public interface BoardSaver {
    void save(String path, Board boad) throws IOException;
}
