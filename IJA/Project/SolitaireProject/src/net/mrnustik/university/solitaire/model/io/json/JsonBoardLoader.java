package net.mrnustik.university.solitaire.model.io.json;

import net.mrnustik.university.solitaire.model.board.Board;
import net.mrnustik.university.solitaire.model.io.BoardLoader;
import net.mrnustik.university.solitaire.model.io.converter.Converter;
import net.mrnustik.university.solitaire.model.io.file.FileUtils;
import net.mrnustik.university.solitaire.model.io.json.converter.JsonToBoardConverter;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;

/**
 * Class used for board loading from JSON file.
 *
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 * @see BoardLoader
 */
public class JsonBoardLoader implements BoardLoader {

    /**
     * Loads the board from JSON file specified in path.
     *
     * @param path to board save file
     * @return loaded Board
     * @throws FileNotFoundException when file specified in path was not found
     */
    @Override
    public Board load(String path) throws FileNotFoundException {
        File file = new File(path);
        if (!file.exists()) {
            throw new FileNotFoundException("File " + path + " was not found");
        }
        try {
            String json = FileUtils.readFile(file);
            Converter<String, Board> converter = new JsonToBoardConverter();
            return converter.convert(json);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

}
