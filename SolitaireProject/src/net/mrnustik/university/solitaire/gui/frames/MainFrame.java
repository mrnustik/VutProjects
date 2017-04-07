package net.mrnustik.university.solitaire.gui.frames;

import net.mrnustik.university.solitaire.gui.panels.GamePanel;

import javax.swing.*;

import java.awt.*;
import java.awt.event.KeyEvent;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by mrnda on 4/7/2017.
 */
public class MainFrame extends JFrame {

    private final List<GamePanel> games;

    public MainFrame() throws HeadlessException {
        super();
        games = new ArrayList<>();
        setTitle("Super solitaire");
        setSize(800,600);
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        initLayout();
        initMenuBar();
        startNewGame();
    }

    private void initLayout() {
        GridLayout layout = new GridLayout(2, 2);
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


        JMenuItem item = new JMenuItem("New game");
        item.setMnemonic(KeyEvent.VK_CONTROL | KeyEvent.VK_N);
        item.addActionListener(e -> {
            startNewGame();
            if(games.size() == 4)
                ((JMenuItem)e.getSource()).setEnabled(false);
        });
        menu.add(item);

        setJMenuBar(menuBar);
    }

    private void startNewGame()
    {
        if(games.size() > 0) {
            setLayout(new GridLayout(0,2, 5,5));
        } else {
            setLayout(new CardLayout());
        }
        GamePanel panel = new GamePanel();

        games.add(panel);
        add(panel);
        revalidate();
        repaint();
    }

}
