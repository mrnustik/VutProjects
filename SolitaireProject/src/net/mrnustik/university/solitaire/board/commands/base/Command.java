package net.mrnustik.university.solitaire.board.commands.base;

/**
 * Created by mrnda on 4/7/2017.
 */
public interface Command {
    /**
     * Executes the command.
     *
     * @return true if command was successfull
     */
    boolean execute();

    /**
     * Reverts back the command.
     */
    void undo();

    /**
     * @return true if command was successful
     */
    boolean wasSuccessful();
}
