package net.mrnustik.university.solitaire.io.json;

import net.mrnustik.university.solitaire.io.BoardSaver;
import net.mrnustik.university.solitaire.io.converter.Converter;
import net.mrnustik.university.solitaire.io.file.FileUtils;
import net.mrnustik.university.solitaire.io.json.converter.BoardToJsonConverter;
import net.mrnustik.university.solitaire.board.Board;

import java.io.File;
import java.io.IOException;

/**
 * Created by mrnda on 4/7/2017.
 */
public class JsonBoardSaver implements BoardSaver {

    @Override
    public void save(String path, Board board) throws IOException {
        Converter<Board, String> converter = new BoardToJsonConverter();
        String json = converter.convert(board);
        File file = new File(path);
        FileUtils.writeToFile(file, json);
    }
}
