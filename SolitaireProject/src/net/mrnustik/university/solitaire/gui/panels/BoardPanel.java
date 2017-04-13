package net.mrnustik.university.solitaire.gui.panels;

import net.mrnustik.university.solitaire.board.Board;
import net.mrnustik.university.solitaire.board.Hint;
import net.mrnustik.university.solitaire.board.Selection;
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
            paintDeck();
            selection.reset();
            repaint();
        });

        stacker.addActionListener(l -> {
            selection.reset();
            selection.setCard(stacker.getCard());
            selection.setType(Selection.SelectionType.STACKER);
        });
    }

    private void initMenu() {
        JPanel gameMenu = new JPanel();
        gameMenu.setBackground(Color.decode("#146a2c"));
        gameMenu.setLayout(new GridBagLayout());
        add(gameMenu);

        GridBagConstraints constraints = new GridBagConstraints();
        constraints.gridx = 0;
        constraints.gridy = 0;
        constraints.gridwidth = 2;
        constraints.fill = GridBagConstraints.HORIZONTAL;
        scoreLabel = new JLabel("Score: 0", SwingConstants.CENTER);
        scoreLabel.setForeground(Color.white);
        gameMenu.add(scoreLabel, constraints);
        JButton saveBtn = new JButton(new ImageIcon(getClass().getResource("/net/mrnustik/university/solitaire/gui/images/SAVE_ICON.png")));
        saveBtn.addActionListener(l -> showSaveGameDialog());
        constraints.gridy = 1;
        constraints.gridx = 0;
        constraints.gridwidth = 1;
        gameMenu.add(saveBtn, constraints);
        JButton undoBtn = new JButton(new ImageIcon(getClass().getResource("/net/mrnustik/university/solitaire/gui/images/UNDO_ICON.png")));
        undoBtn.addActionListener(l -> {
            board.undo();
            paintBoard();
        });
        constraints.gridx = 1;
        gameMenu.add(undoBtn, constraints);
        JButton endBtn = new JButton(new ImageIcon(getClass().getResource("/net/mrnustik/university/solitaire/gui/images/DELETE_ICON.png")));
        endBtn.addActionListener(l -> {
            MainFrame topFrame = (MainFrame) SwingUtilities.getWindowAncestor(this);
            topFrame.removeGame(this);
        });
        constraints.gridy = 2;
        constraints.gridx = 0;
        gameMenu.add(endBtn, constraints);

        JButton hintButton = new JButton(new ImageIcon(getClass().getResource("/net/mrnustik/university/solitaire/gui/images/HINT_ICON.png")));
        hintButton.addActionListener(l -> hintClicked());
        constraints.gridy = 2;
        constraints.gridx = 1;
        gameMenu.add(hintButton, constraints);
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
            if (selection.getType() == Selection.SelectionType.STACKER) {
                success = board.fromStackerToTarget(index);
            } else if (selection.getType() == Selection.SelectionType.WORKING_PACK) {
                success = board.fromWorkingToTarget(selection.getIndex(), index);
            }
            if(success) {
                if(selection.getType() == Selection.SelectionType.STACKER){
                    paintDeck();
                } else if(selection.getType() == Selection.SelectionType.WORKING_PACK){
                    paintWorkingPacks();
                }
                paintTargets();
                paintScore();
                repaint();
                checkWin();
            }
            selection.reset();
        } else {
            selection.setType(Selection.SelectionType.TARGET);
            selection.setIndex(index);
        }

    }

    private void hintClicked() {
        Hint hint = board.getHint();
        if(hint != null) {
            highlightHint(hint);
        }
    }

    private void highlightHint(Hint hint) {
        if(hint.getFromSelection() != null){
            highlightSelection(hint.getFromSelection());
        }
        if(hint.getToSelection() != null){
            highlightSelection(hint.getToSelection());
        }
    }

    private void highlightSelection(Selection selection) {
        if(selection.getType() == Selection.SelectionType.DECK){
            deck.highlight();
        } else if(selection.getType() == Selection.SelectionType.STACKER) {
            stacker.highlight();
        } else if(selection.getType() == Selection.SelectionType.TARGET) {
            int index = selection.getIndex();
            targets[index].highlight();
        } else if(selection.getType() == Selection.SelectionType.WORKING_PACK) {
            int index = selection.getIndex();
            workingStacks[index].highlight(selection.getCard());
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

        paintWorkingPacks();

        paintScore();
    }

    private void paintScore() {
        this.scoreLabel.setText("Score: "+ board.getScore());
    }

    private void paintWorkingPacks() {
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
        repaint();
    }

    @Override
    public void onCardSelected(Card card, int index) {
        if (selection.isValid()) {
            boolean success = false;
            if (selection.getType() == Selection.SelectionType.STACKER) {
                success = board.fromStackerToWorking(index);
            } else if (selection.getType() == Selection.SelectionType.TARGET) {
                success = board.fromTargetToWorking(selection.getIndex(), index);
            } else if (selection.getType() == Selection.SelectionType.WORKING_PACK) {
                success = board.fromWorkingToWorking(selection.getIndex(), index, selection.getCard());
            }
            if(success) {
                paintBoard();
            }
            selection.reset();
        } else {
            selection.setType(Selection.SelectionType.WORKING_PACK);
            selection.setCard(card);
            selection.setIndex(index);
        }
    }


}
