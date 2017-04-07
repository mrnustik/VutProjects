package net.mrnustik.university.solitaire.command;

/**
 * Created by mrnda on 4/7/2017.
 */
public abstract class AbstractCommand implements Command {
    protected boolean success;

    public boolean wasSuccessful() {
        return success;
    }

}
