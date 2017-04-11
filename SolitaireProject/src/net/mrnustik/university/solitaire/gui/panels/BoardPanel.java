package net.mrnustik.university.solitaire.gui.panels;

import net.mrnustik.university.solitaire.board.Board;
import net.mrnustik.university.solitaire.factory.SolitaireFactory;
import net.mrnustik.university.solitaire.gui.frames.MainFrame;
import net.mrnustik.university.solitaire.gui.view.CardView;
import net.mrnustik.university.solitaire.io.BoardSaver;
import net.mrnustik.university.solitaire.io.json.JsonBoardSaver;
import net.mrnustik.university.solitaire.board.BoardImpl;
import net.mrnustik.university.solitaire.model.Card;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.io.File;

/**
 * Created by mrnda on 4/7/2017.
 */
public class BoardPanel extends JPanel implements CardStackPanel.CardSelected {
    private Board board;

    private CardView deck;
    private CardView stacker;
    private CardView[] targets;

    private Selection selection;
    private JLabel scoreLabel;
    private CardStackPanel[] workingStacks;


    public BoardPanel() {
        this(new BoardImpl(new SolitaireFactory()));
    }

    public BoardPanel(Board board) {
        super(new GridLayout(2, 6, 5, 5));
        this.board = board;
        setMinimumSize(new Dimension(200, 150));
        setBackground(Color.decode("#146a2c"));
        initViews();
        selection = new Selection();
        paintBoard();
    }

    private void initViews() {
        setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        initDeck();
        initMenu();
        initTargetPacks();
        initWorkingStacks();

    }

    private void initWorkingStacks() {
        workingStacks = new CardStackPanel[7];
        for (int i = 0; i < 7; i++) {
            CardStackPanel panel = new CardStackPanel(board.getWorkingStack(i), i);
            panel.setCardSelectedListener(this);
            workingStacks[i] = panel;
            add(panel);
        }
    }

    private void initTargetPacks() {
        targets = new CardView[4];
        for (int i = 0; i < 4; i++) {
            final int index = i;
            targets[i] = new CardView(null);
            targets[i].addActionListener(l -> targetClicked(index));

            add(targets[i]);
        }
    }

    private void initDeck() {
        deck = new CardView(board.getDeckTop());
        add(deck);
        stacker = new CardView(board.getStackTop());
        add(stacker);
        deck.addActionListener(l -> {
            board.flipFromDeck();
            paintBoard();
            selection.reset();
        });

        stacker.addActionListener(l -> {
            selection.reset();
            selection.setCard(stacker.getCard());
            selection.setType(SelectionType.STACKER);
        });
    }

    private void initMenu() {
        JPanel gameMenu = new JPanel();
        gameMenu.setBackground(Color.decode("#146a2c"));
        gameMenu.setLayout(new BoxLayout(gameMenu, BoxLayout.PAGE_AXIS));
        add(gameMenu);

        gameMenu.add(Box.createVerticalGlue());

        JButton saveBtn = new JButton(new ImageIcon(getClass().getResource("/net/mrnustik/university/solitaire/gui/images/SAVE_ICON.png")));
        saveBtn.setAlignmentX(Component.CENTER_ALIGNMENT);
        saveBtn.addActionListener(l -> showSaveGameDialog());
        gameMenu.add(saveBtn);
        gameMenu.add(Box.createVerticalStrut(10));
        JButton undoBtn = new JButton(new ImageIcon(getClass().getResource("/net/mrnustik/university/solitaire/gui/images/UNDO_ICON.png")));
        undoBtn.setAlignmentX(Component.CENTER_ALIGNMENT);
        undoBtn.addActionListener(l -> {
            board.undo();
            paintBoard();
        });
        gameMenu.add(undoBtn);
        gameMenu.add(Box.createVerticalStrut(10));
        JButton endBtn = new JButton(new ImageIcon(getClass().getResource("/net/mrnustik/university/solitaire/gui/images/DELETE_ICON.png")));
        endBtn.setAlignmentX(Component.CENTER_ALIGNMENT);
        endBtn.addActionListener(l -> {
            MainFrame topFrame = (MainFrame) SwingUtilities.getWindowAncestor(this);
            topFrame.removeGame(this);
        });
        gameMenu.add(endBtn);
        gameMenu.add(Box.createVerticalGlue());
    }

    private void checkWin() {
        if (board.isWin()) {
            JOptionPane.showMessageDialog(this,
                    "You have successfully won this game!",
                    "Congratulations",
                    JOptionPane.PLAIN_MESSAGE);
        }
    }

    private void targetClicked(int index) {
        if (selection.isValid()) {
            boolean success = false;
            if (selection.getType() == SelectionType.STACKER) {
                success = board.fromStackerToTarget(index);
            } else if (selection.getType() == SelectionType.WORKING_PACK) {
                success = board.fromWorkingToTarget(selection.index, index);
            }
            paintBoard();
            checkWin();
            selection.reset();
        } else {
            selection.setType(SelectionType.TARGET);
            selection.setIndex(index);
        }

    }


    private void showSaveGameDialog() {
        JFileChooser fileChooser = new JFileChooser();
        FileNameExtensionFilter jsonFilter = new FileNameExtensionFilter("JSON files (*.json)", "json");
        fileChooser.addChoosableFileFilter(jsonFilter);
        fileChooser.setFileFilter(jsonFilter);
        int result = fileChooser.showSaveDialog(this);
        if (result == JFileChooser.APPROVE_OPTION) {
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

    private void paintBoard() {
        paintDeck();

        paintTargets();

        painWorkingPacks();

        repaint();
    }

    private void painWorkingPacks() {
        for (int i = 0; i < 7; i++) {
            workingStacks[i].setStack(board.getWorkingStack(i));
        }
    }

    private void paintTargets() {
        for (int i = 0; i < 4; i++) {
            targets[i].changeCard(board.getTargetTop(i));
        }
    }

    private void paintDeck() {
        deck.changeCard(board.getDeckTop());
        stacker.changeCard(board.getStackTop());
    }

    @Override
    public void onCardSelected(Card card, int index) {
        if (selection.isValid()) {
            boolean success = false;
            if (selection.getType() == SelectionType.STACKER) {
                success = board.fromStackerToWorking(index);
            } else if (selection.getType() == SelectionType.TARGET) {
                success = board.fromTargetToWorking(selection.getIndex(), index);
            } else if (selection.getType() == SelectionType.WORKING_PACK) {
                success = board.fromWorkingToWorking(selection.getIndex(), index, selection.getCard());
            }
            paintBoard();
            repaint();
            selection.reset();
        } else {
            selection.setType(SelectionType.WORKING_PACK);
            selection.setCard(card);
            selection.setIndex(index);
        }
    }

    private enum SelectionType {
        UNDEFINED,
        STACKER,
        TARGET,
        WORKING_PACK
    }


    class Selection {

        private SelectionType type = SelectionType.UNDEFINED;
        private int index = -1;
        private Card card;

        void reset() {
            type = SelectionType.UNDEFINED;
            index = -1;
        }

        boolean isValid() {
            return type != SelectionType.UNDEFINED;
        }

        int getIndex() {
            return index;
        }

        void setIndex(int index) {
            this.index = index;
        }

        SelectionType getType() {
            return type;
        }

        void setType(SelectionType type) {
            this.type = type;
        }

        public Card getCard() {
            return card;
        }

        public void setCard(Card card) {
            this.card = card;
        }
    }
}
