package net.mrnustik.university.solitaire.view.frames;

import net.mrnustik.university.solitaire.model.board.Board;
import net.mrnustik.university.solitaire.view.panels.BoardPanel;
import net.mrnustik.university.solitaire.model.io.BoardLoader;
import net.mrnustik.university.solitaire.model.io.json.JsonBoardLoader;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * Main JFrame object of the application.
 *
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public class MainFrame extends JFrame {

    /**
     * Maximum count of the games, that can be played simultaneously
     */
    private static final int MAX_GAMES_COUNT = 4;

    /**
     * List of currently running games
     */
    private final List<BoardPanel> games;

    /**
     * Menu item for new game creation
     */
    private JMenuItem newGameItem;

    /**
     * Menu item for loading new game
     */
    private JMenuItem loadGameItem;

    /**
     * Creates the initial board
     *
     * @throws HeadlessException when the app is started in headless environment (without GUI)
     */
    public MainFrame() throws HeadlessException {
        super();
        games = new ArrayList<>();
        setTitle("Unicolitaire");
        setSize(800, 600);
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        initLayout();
        initMenuBar();
        startNewGame();
    }

    /**
     * Initializes layout of the JFrame
     */
    private void initLayout() {
        GridLayout layout = new GridLayout(0, 2);
        layout.setHgap(5);
        layout.setVgap(5);
        setLayout(layout);

    }

    /**
     * Initializes all things in menu bar
     */
    private void initMenuBar() {
        JMenuBar menuBar = new JMenuBar();
        JMenu menu = new JMenu("File");
        menu.setMnemonic(KeyEvent.VK_F);
        menu.getAccessibleContext().setAccessibleDescription(
                "The only menu in this program that has menu items");
        menuBar.add(menu);


        newGameItem = new JMenuItem("New game");
        newGameItem.setMnemonic(KeyEvent.VK_CONTROL | KeyEvent.VK_N);
        newGameItem.addActionListener((ActionEvent e) -> startNewGame());
        newGameItem.setAccelerator(KeyStroke.getKeyStroke('N', Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));

        menu.add(newGameItem);

        loadGameItem = new JMenuItem("Load game");
        loadGameItem.setAccelerator(KeyStroke.getKeyStroke('L', Toolkit.getDefaultToolkit().getMenuShortcutKeyMask()));
        loadGameItem.addActionListener(e -> showLoadGameDialog());
        menu.add(loadGameItem);

        setJMenuBar(menuBar);
    }

    /**
     * Shows JFileChooser dialog for opening the save game file
     *
     * @see JFileChooser
     */
    private void showLoadGameDialog() {
        JFileChooser fileChooser = new JFileChooser();
        FileNameExtensionFilter jsonFilter = new FileNameExtensionFilter("JSON files (*.json)", "json");
        fileChooser.addChoosableFileFilter(jsonFilter);
        fileChooser.setFileFilter(jsonFilter);
        int result = fileChooser.showOpenDialog(this);
        if (result == JFileChooser.APPROVE_OPTION) {
            loadGame(fileChooser.getSelectedFile().getPath());
        }
    }

    /**
     * Loads game specified in path
     *
     * @param path to the save game
     */
    private void loadGame(String path) {
        BoardLoader loader = new JsonBoardLoader();
        try {
            Board board = loader.load(path);
            BoardPanel panel = new BoardPanel(board);
            addGamePanel(panel);
        } catch (IOException e) {
            JOptionPane.showMessageDialog(this, "Something wrong happened when loading game." + e.getMessage());
        }
    }

    /**
     * Adds panel of a game to the JFrame
     *
     * @param panel to be add to the game
     */
    private void addGamePanel(BoardPanel panel) {
        games.add(panel);
        add(panel);
        checkLayout();
        EventQueue.invokeLater(() -> {
            this.revalidate();
            this.repaint();
        });
    }

    /**
     * Changes the layout manager of the JFrame if there is too many games.
     */
    private void checkLayout() {
        if (games.size() > 1) {
            setLayout(new GridLayout(0, 2, 5, 5));
        } else {
            setLayout(new CardLayout());
        }
    }

    /**
     * Creates new game objects
     */
    private void startNewGame() {

        BoardPanel panel = new BoardPanel();
        addGamePanel(panel);
        checkFull();
    }

    /**
     * Checks whether the game plan is not full and disables the menu items when it is.
     */
    private void checkFull() {
        if (games.size() == MAX_GAMES_COUNT) {
            loadGameItem.setEnabled(false);
            newGameItem.setEnabled(false);
        } else {
            loadGameItem.setEnabled(true);
            newGameItem.setEnabled(true);
        }
    }

    /**
     * Removes game from the game panel
     *
     * @param boardPanel game to be removed
     */
    public void removeGame(BoardPanel boardPanel) {
        this.remove(boardPanel);
        games.remove(boardPanel);
        checkLayout();
        revalidate();
        repaint();
        checkFull();
    }
}
