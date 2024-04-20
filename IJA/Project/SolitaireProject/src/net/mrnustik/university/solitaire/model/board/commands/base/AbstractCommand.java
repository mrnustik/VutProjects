package net.mrnustik.university.solitaire.model.board.commands.base;

/**
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public abstract class AbstractCommand implements Command {
    /**
     * boolean indicating success of the command
     */
    protected boolean success;

    /**
     * score of the command
     */
    private int points = 0;

    /**
     * @return true if command was successful
     */
    @Override
    public boolean wasSuccessful() {
        return success;
    }

    /**
     * Sets the score of command
     *
     * @param points score to be given to the command
     */
    @Override
    public void setScore(int points) {
        this.points = points;
    }

    /**
     * @return score of the command
     */
    @Override
    public int getScore() {
        return this.points;
    }
}
