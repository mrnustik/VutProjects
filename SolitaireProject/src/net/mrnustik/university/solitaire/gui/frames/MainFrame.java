package net.mrnustik.university.solitaire.gui.frames;

import net.mrnustik.university.solitaire.board.Board;
import net.mrnustik.university.solitaire.gui.panels.BoardPanel;
import net.mrnustik.university.solitaire.io.BoardLoader;
import net.mrnustik.university.solitaire.io.json.JsonBoardLoader;
import net.mrnustik.university.solitaire.board.BoardImpl;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by mrnda on 4/7/2017.
 */
public class MainFrame extends JFrame {

    private static final int MAX_GAMES_COUNT = 4;
    private final List<BoardPanel> games;
    private JMenuItem newGameItem;
    private JMenuItem loadGameItem;

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

    private void initLayout() {
        GridLayout layout = new GridLayout(0, 2);
        layout.setHgap(5);
        layout.setVgap(5);
        setLayout(layout);

    }

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

    private void addGamePanel(BoardPanel panel) {
        games.add(panel);
        add(panel);
        checkLayout();
        EventQueue.invokeLater(() -> {
            this.revalidate();
            this.repaint();
        });
    }

    private void checkLayout() {
        if (games.size() > 1) {
            setLayout(new GridLayout(0, 2, 5, 5));
        } else {
            setLayout(new CardLayout());
        }
    }

    private void startNewGame() {

        BoardPanel panel = new BoardPanel();
        addGamePanel(panel);
        checkFull();
    }

    private void checkFull() {
        if (games.size() == MAX_GAMES_COUNT) {
            loadGameItem.setEnabled(false);
            newGameItem.setEnabled(false);
        } else {
            loadGameItem.setEnabled(true);
            newGameItem.setEnabled(true);
        }
    }

    public void removeGame(BoardPanel boardPanel) {
        this.remove(boardPanel);
        games.remove(boardPanel);
        checkLayout();
        revalidate();
        repaint();
        checkFull();
    }
}
