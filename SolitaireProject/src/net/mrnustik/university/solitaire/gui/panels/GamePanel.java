package net.mrnustik.university.solitaire.gui.panels;

import net.mrnustik.university.solitaire.factory.SolitaireFactory;
import net.mrnustik.university.solitaire.gui.frames.MainFrame;
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
public class GamePanel extends JPanel implements CardStackPanel.CardSelected {
    private Board board;

    private CardView deck;
    private CardView stacker;
    private CardView[] targets;

    private Card selected = null;
    private JLabel scoreLabel;
    private CardStackPanel[] workingStacks;


    public GamePanel() {
        this(new Board(new SolitaireFactory()));
    }

    public GamePanel(Board board) {
        super(new GridLayout(2,6, 5 ,5));
        this.board = board;
        setMinimumSize(new Dimension(200,150));
        setBackground(Color.decode("#146a2c"));
        initViews();
    }

    private void initViews() {
        setBorder(BorderFactory.createEmptyBorder(10,10,10,10));
        initDeck();
        initMenu();
        initTargetPacks();
        initWorkingStacks();
    }

    private void initWorkingStacks() {
        workingStacks = new CardStackPanel[7];
        for(int i = 0; i < 7; i++)
        {
            CardStackPanel panel = new CardStackPanel(board.getWorkingStack(i), i);
            panel.setCardSelectedListener(this);
            workingStacks[i] = panel;
            add(panel);
        }
    }

    private void initTargetPacks() {
        targets = new CardView[4];
        for(int i = 0; i < 4; i++)
        {
            final int index = i;
            targets[i] = new CardView(null);
            targets[i].addActionListener(l-> targetClicked(index));

            add(targets[i]);
        }
    }

    private void targetClicked(int index) {
        if(selected != null)
        {
            board.fromStackerToTarget(index);
        }
        paintBoard();
        checkWin();
        clearSelected();
    }

    private void clearSelected() {
        selected = null;
    }

    private void checkWin() {
        if(board.isWin())
        {
            JOptionPane.showMessageDialog(this,
                    "You have successfully won this game!",
                    "Congratulations",
                    JOptionPane.PLAIN_MESSAGE);
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
        saveBtn.addActionListener(l-> showSaveGameDialog());
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
            MainFrame topFrame = (MainFrame) SwingUtilities.getWindowAncestor(this);
            topFrame.removeGame(this);
        });
        gameMenu.add(endBtn);
        gameMenu.add(Box.createVerticalGlue());
    }

    private void showSaveGameDialog() {
        JFileChooser fileChooser = new JFileChooser();
        FileNameExtensionFilter jsonFilter = new FileNameExtensionFilter("JSON files (*.json)", "json");
        fileChooser.addChoosableFileFilter(jsonFilter);
        fileChooser.setFileFilter(jsonFilter);
        int result = fileChooser.showSaveDialog(this);
        if(result == JFileChooser.APPROVE_OPTION) {
            File file = fileChooser.getSelectedFile();
            saveGame(file.getPath());
        }
    }

    private void saveGame(String path) {
        BoardSaver saver = new JsonBoardSaver();
        try {
            saver.save(path, board);
        } catch (Exception e) {
            JOptionPane.showMessageDialog(this, "Something wrong happened when saving." + e.getMessage());
        }
    }

    public void paintBoard() {
        deck.changeCard(board.getDeckTop());
        stacker.changeCard(board.getStackTop());

        for(int i = 0; i < 4; i++) {
            targets[i].changeCard(board.getTargetTop(i));
        }

        for(int i = 0; i <  7; i++)
        {
            workingStacks[i].setStack(board.getWorkingStack(i));
        }

        revalidate();
        repaint();
    }

    @Override
    public void onCardSelected(Card card, int index) {
        selected = card;
    }
}
