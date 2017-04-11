package net.mrnustik.university.solitaire.board.commands.base;

/**
 * Created by mrnda on 4/7/2017.
 */
public abstract class AbstractCommand implements Command {
    protected boolean success;
    private int points = 0;

    public boolean wasSuccessful() {
        return success;
    }

    @Override
    public void setScore(int points) {
        this.points = points;
    }

    @Override
    public int getScore() {
        return this.points;
    }
}
