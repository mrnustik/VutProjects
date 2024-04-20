package net.mrnustik.university.solitaire.model.io;

import net.mrnustik.university.solitaire.model.board.Board;

import java.io.IOException;


/**
 * Interface implemented by all classes that are able to save game Board, so it can be loaded again.
 *
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public interface BoardSaver {
    /**
     * Method used for board saving
     *
     * @param path  where the board should be saved
     * @param board to be saved
     * @throws IOException when any error occurs during saving
     */
    void save(String path, Board board) throws IOException;
}
