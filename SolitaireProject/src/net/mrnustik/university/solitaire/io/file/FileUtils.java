package net.mrnustik.university.solitaire.io.file;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.file.Files;
import java.nio.file.Paths;

/**
 * Created by mrnda on 4/7/2017.
 */
public final class FileUtils {
    private FileUtils(int i) {}

    public static String readFile(File file) throws IOException {
        byte[] encoded = Files.readAllBytes(file.toPath());
        return new String(encoded);
    }

    public static void writeToFile(File file, String data) throws IOException {
        try(  PrintWriter out = new PrintWriter(file)  ){
            out.println( data );
        }
    }

}
