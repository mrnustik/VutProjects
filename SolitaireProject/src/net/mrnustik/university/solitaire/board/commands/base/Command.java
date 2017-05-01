package net.mrnustik.university.solitaire.board.commands.base;

/**
 * Command design pattern interface
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public interface Command {

    /**
     * Executes the command
     * @return true if command was successfull
     */
    boolean execute();

    /**
     * Reverts back the changes done by command.
     */
    void undo();

    /**
     * @return true if command was successful
     */
    boolean wasSuccessful();

    /**
     * Add points to the command execution
     * @param points score to be given to the command
     */
    void setScore(int points);

    /**
     * @return score of the command
     * @see Command#setScore(int)
     */
    int getScore();
}
