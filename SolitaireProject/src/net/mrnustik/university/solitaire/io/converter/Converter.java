package net.mrnustik.university.solitaire.io.converter;

/**
 * Common conversion interface
 * @param <SRC> source conversion type
 * @param <DEST> destination conversion type
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public interface Converter<SRC, DEST> {

    /**
     * Converts from SRC to DEST type
     * @param source source object to be converted
     * @return converted object into DEST type
     */
    DEST convert(SRC source);
}
