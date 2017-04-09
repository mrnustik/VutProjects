package net.mrnustik.university.solitaire.gui.panels;

import net.mrnustik.university.solitaire.factory.SolitaireFactory;
import net.mrnustik.university.solitaire.gui.view.CardView;
import net.mrnustik.university.solitaire.io.BoardSaver;
import net.mrnustik.university.solitaire.io.json.JsonBoardSaver;
import net.mrnustik.university.solitaire.model.Board;
import net.mrnustik.university.solitaire.model.Card;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.io.File;

/**
 * Created by mrnda on 4/7/2017.
 */
public class GamePanel extends JPanel {
    private Board board;

    private CardView deck;
    private CardView stacker;
    private CardView[] targets;

    private Card selected = null;
    private JLabel scoreLabel;


    public GamePanel() {
        super(new GridLayout(2,6, 10 ,10));
        board = new Board(new SolitaireFactory());
        setMinimumSize(new Dimension(200,150));
        setBackground(Color.decode("#146a2c"));
        initViews();
        paintBoard();

    }

    private void initViews() {
        setBorder(BorderFactory.createEmptyBorder(10,10,10,10));
        initDeck();
        initMenu();
        initTargetPacks();
        initWorkingStacks();
    }

    private void initWorkingStacks() {
        for(int i = 0; i < 7; i++)
        {
            add(new CardView(board.getStackTop()));
        }
    }

    private void initTargetPacks() {
        targets = new CardView[4];
        for(int i = 0; i < 4; i++)
        {
            final int index = i;
            targets[i] = new CardView(null);
            targets[i].addActionListener(l->{
                if(selected != null)
                {
                    board.fromStackerToTarget(index);
                }
                paintBoard();
            });

            add(targets[i]);
        }
    }

    private void initDeck() {
        deck = new CardView(board.getDeckTop());
        add(deck);
        stacker = new CardView(board.getStackTop());
        add(stacker);
        deck.addActionListener(l->{
            board.flipFromDeck();
            paintBoard();
        });

        stacker.addActionListener(l->{
            selected = stacker.getCard();
        });
    }

    private void initMenu() {
        JPanel gameMenu = new JPanel();
        gameMenu.setBackground(Color.decode("#146a2c"));
        gameMenu.setLayout(new BoxLayout(gameMenu, BoxLayout.PAGE_AXIS));
        add(gameMenu);

        gameMenu.add(Box.createVerticalGlue());

        JButton saveBtn = new JButton("Save game");
        saveBtn.setAlignmentX(Component.CENTER_ALIGNMENT);
        saveBtn.addActionListener(l->{
            JFileChooser fileChooser = new JFileChooser();
            FileNameExtensionFilter xmlFilter = new FileNameExtensionFilter("JSON files (*.json)", "json");
            fileChooser.addChoosableFileFilter(xmlFilter);
            fileChooser.setFileFilter(xmlFilter);
            int result = fileChooser.showSaveDialog(this);
            if(result == JFileChooser.APPROVE_OPTION) {
                File file = fileChooser.getSelectedFile();
                saveGame(file.getPath());
            }
        });
        gameMenu.add(saveBtn);
        gameMenu.add(Box.createVerticalStrut(10));
        JButton undoBtn = new JButton("Undo");
        undoBtn.setAlignmentX(Component.CENTER_ALIGNMENT);
        undoBtn.addActionListener(l->{
            board.undo();
            paintBoard();
        });
        gameMenu.add(undoBtn);
        gameMenu.add(Box.createVerticalStrut(10));
        JButton endBtn = new JButton("End game");
        endBtn.setAlignmentX(Component.CENTER_ALIGNMENT);
        endBtn.addActionListener(l->{
            JFrame topFrame = (JFrame) SwingUtilities.getWindowAncestor(this);
            topFrame.remove(this);
        });
        gameMenu.add(endBtn);
        gameMenu.add(Box.createVerticalGlue());
    }

    private void saveGame(String path) {
        BoardSaver saver = new JsonBoardSaver();
        try {
            saver.save(path, board);
        } catch (Exception e) {
            JOptionPane.showMessageDialog(this, "Something wrong happened when saving." + e.getMessage());
        }
    }

    private void paintBoard() {
        deck.changeCard(board.getDeckTop());
        stacker.changeCard(board.getStackTop());

        for(int i = 0; i < 4; i++) {
            targets[i].changeCard(board.getTargetTop(i));
        }

        revalidate();
        repaint();
    }
}
