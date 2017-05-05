package net.mrnustik.university.solitaire.model.io;

import net.mrnustik.university.solitaire.model.board.Board;

import java.io.FileNotFoundException;

/**
 * Interface that should be implemented by all classes, that are able to load game Board save file.
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public interface BoardLoader {
    /**
     * Method that reads the Board from file specified in path.
     * @param path to board save file
     * @return board that has been saved in file
     * @throws FileNotFoundException when file specified in path could not be found
     */
    Board load(String path) throws FileNotFoundException;
}
