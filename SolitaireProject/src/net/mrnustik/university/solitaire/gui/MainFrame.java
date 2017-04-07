package net.mrnustik.university.solitaire.gui;

import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyEvent;

/**
 * Created by mrnda on 4/7/2017.
 */
public class MainFrame extends JFrame {

    public MainFrame() throws HeadlessException {
        super();
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
    }

}
