package net.mrnustik.university.solitaire.io.converter;

/**
 * Created by mrnda on 4/7/2017.
 */
public interface Converter<SRC, DEST> {
    DEST convert(SRC source);
}
