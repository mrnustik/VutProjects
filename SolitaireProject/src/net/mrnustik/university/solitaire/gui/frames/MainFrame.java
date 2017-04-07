package net.mrnustik.university.solitaire.gui.frames;

import net.mrnustik.university.solitaire.gui.panels.GamePanel;

import javax.swing.*;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
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
        initMenuBar();
    }

    private void initMenuBar() {
        JMenuBar menuBar = new JMenuBar();
        JMenu menu = new JMenu("File");
        menu.setMnemonic(KeyEvent.VK_F);
        menu.getAccessibleContext().setAccessibleDescription(
                "The only menu in this program that has menu items");
        menuBar.add(menu);
        setJMenuBar(menuBar);

        JMenuItem item = new JMenuItem("New game");
        item.setMnemonic(KeyEvent.VK_CONTROL | KeyEvent.VK_N);
        item.addActionListener(e -> {
            startNewGame();
            if(games.size() == 4)
                ((JMenuItem)e.getSource()).setEnabled(false);
        });
        menu.add(item);
    }

    private void startNewGame()
    {
        games.add(new GamePanel());
    }

}
