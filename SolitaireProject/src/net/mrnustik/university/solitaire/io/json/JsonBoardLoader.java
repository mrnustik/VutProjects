package net.mrnustik.university.solitaire.io.json;

import net.mrnustik.university.solitaire.io.BoardLoader;
import net.mrnustik.university.solitaire.io.converter.Converter;
import net.mrnustik.university.solitaire.io.file.FileUtils;
import net.mrnustik.university.solitaire.io.json.converter.JsonToBoardConverter;
import net.mrnustik.university.solitaire.model.Board;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

/**
 * Created by mrnda on 4/7/2017.
 */
public class JsonBoardLoader implements BoardLoader {


    @Override
    public Board load(String path) throws FileNotFoundException {
        File file = new File(path);
        if(!file.exists())
        {
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
