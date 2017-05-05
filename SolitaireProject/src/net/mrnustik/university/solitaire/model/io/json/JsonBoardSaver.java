package net.mrnustik.university.solitaire.model.io.json;

import net.mrnustik.university.solitaire.model.board.Board;
import net.mrnustik.university.solitaire.model.io.BoardSaver;
import net.mrnustik.university.solitaire.model.io.converter.Converter;
import net.mrnustik.university.solitaire.model.io.file.FileUtils;
import net.mrnustik.university.solitaire.model.io.json.converter.BoardToJsonConverter;

import java.io.File;
import java.io.IOException;

/**
 * Class used to save Board into JSON.
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 * @see BoardSaver
 */
public class JsonBoardSaver implements BoardSaver {

    /**
     * Method, that saves Board given in parameters to a JSON file specified in path.
     * @param path where the board should be saved
     * @param board to be saved
     * @throws IOException when can't write to file
     */
    @Override
    public void save(String path, Board board) throws IOException {
        Converter<Board, String> converter = new BoardToJsonConverter();
        String json = converter.convert(board);
        File file = new File(path);
        FileUtils.writeToFile(file, json);
    }
}
