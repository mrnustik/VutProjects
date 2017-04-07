package net.mrnustik.university.solitaire.gui;

import javax.swing.*;
import java.awt.*;

/**
 * Created by mrnda on 4/7/2017.
 */
public class MainFrame extends JFrame {

    public MainFrame() throws HeadlessException {
        super();
        setTitle("Super solitaire");
        setSize(800,600);
        setLayout(new GridBagLayout());
        add(new JLabel("Test"));
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }
}
