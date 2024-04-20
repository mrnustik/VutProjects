package net.mrnustik.university.solitaire.model.io.file;

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.file.Files;
import java.nio.file.Path;

/**
 * Helper for I/O file operations
 *
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public final class FileUtils {

    /**
     * Private constructor to ensure class can never be created
     * @param i param to disallow inheritance
     */
    private FileUtils(int i) {
    }

    /**
     * Reads whole content of file into string
     *
     * @param file to be read
     * @return content of file represented as String
     * @throws IOException when file does not exist
     * @see Files#readAllBytes(Path)
     */
    public static String readFile(File file) throws IOException {
        byte[] encoded = Files.readAllBytes(file.toPath());
        return new String(encoded);
    }

    /**
     * Writes string to a file.
     *
     * @param file to be stored in
     * @param data string data to store
     * @throws IOException when error occurs during writing
     * @see PrintWriter
     */
    public static void writeToFile(File file, String data) throws IOException {
        try (PrintWriter out = new PrintWriter(file)) {
            out.println(data);
        }
    }

}
