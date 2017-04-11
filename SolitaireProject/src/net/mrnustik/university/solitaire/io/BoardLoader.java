package net.mrnustik.university.solitaire.io;

import net.mrnustik.university.solitaire.board.Board;

import java.io.FileNotFoundException;

/**
 * Created by mrnda on 4/7/2017.
 */
public interface BoardLoader {
    Board load(String path) throws FileNotFoundException;
}
