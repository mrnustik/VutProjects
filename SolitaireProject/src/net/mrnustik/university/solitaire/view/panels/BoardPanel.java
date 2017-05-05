package net.mrnustik.university.solitaire.view.panels;

import net.mrnustik.university.solitaire.model.board.Board;
import net.mrnustik.university.solitaire.model.board.Hint;
import net.mrnustik.university.solitaire.model.board.Selection;
import net.mrnustik.university.solitaire.model.board.factory.SolitaireFactory;
import net.mrnustik.university.solitaire.view.frames.MainFrame;
import net.mrnustik.university.solitaire.view.view.CardView;
import net.mrnustik.university.solitaire.model.io.BoardSaver;
import net.mrnustik.university.solitaire.model.io.json.JsonBoardSaver;
import net.mrnustik.university.solitaire.model.board.BoardImpl;
import net.mrnustik.university.solitaire.model.card.Card;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.io.File;

/**
 * JPanel used to describe layout of the single game
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public class BoardPanel extends JPanel implements CardStackPanel.CardSelected {

    /**
     * Board object of currently played game
     */
    private Board board;

    /**
     * View representing the deck of cards
     */
    private CardView deck;

    /**
     * View representing the put-down stacker of cards
     */
    private CardView stacker;

    /**
     * View representing the target stacker
     */
    private CardView[] targets;

    /**
     * View representing the working stacks
     */
    private CardStackPanel[] workingStacks;

    /**
     * Index of the working stack that should be repainted
     */
    int repaintWorkingIndex1 = -2;
    /**
     * Index of the second working stack that should repainted
     */
    int repaintWorkingIndex2 = -2;

    /**
     * Current selection
     */
    private Selection selection;

    /**
     * Label showing current score
     */
    private JLabel scoreLabel;

    /**
     * Creates the panel with freshly started new game.
     */
    public BoardPanel() {
        this(new BoardImpl(new SolitaireFactory()));
    }

    /**
     * Creates the panel with game, that is specified in param.
     * @param board that should be loaded into panel
     */
    public BoardPanel(Board board) {
        super(new GridLayout(2, 6, 5, 5));
        this.board = board;
        setMinimumSize(new Dimension(200, 150));
        setBackground(Color.decode("#146a2c"));
        initViews();
        selection = new Selection();
        paintBoard();
    }

    /**
     * Initializes all UI elements
     */
    private void initViews() {
        setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
        initDeck();
        initMenu();
        initTargetStacks();
        initWorkingStacks();

    }

    /**
     * Initializes working stacks views.
     */
    private void initWorkingStacks() {
        workingStacks = new CardStackPanel[7];
        for (int i = 0; i < 7; i++) {
            CardStackPanel panel = new CardStackPanel(board.getWorkingStack(i), i);
            panel.setCardSelectedListener(this);
            workingStacks[i] = panel;
            add(panel);
        }
    }

    /**
     * Initializes target stacks view
     */
    private void initTargetStacks() {
        targets = new CardView[4];
        for (int i = 0; i < 4; i++) {
            final int index = i;
            targets[i] = new CardView(null);
            targets[i].addActionListener(l -> targetClicked(index));

            add(targets[i]);
        }
    }

    /**
     * Initializes the deck and put-down stacker view
     */
    private void initDeck() {
        deck = new CardView(board.getDeckTop());
        add(deck);
        stacker = new CardView(board.getStackerTop());
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

    /**
     * Initializes menu items and score label
     */
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
        JButton saveBtn = new JButton(new ImageIcon(getClass().getResource("/net/mrnustik/university/solitaire/view/images/SAVE_ICON.png")));
        saveBtn.addActionListener(l -> showSaveGameDialog());
        constraints.gridy = 1;
        constraints.gridx = 0;
        constraints.gridwidth = 1;
        gameMenu.add(saveBtn, constraints);
        JButton undoBtn = new JButton(new ImageIcon(getClass().getResource("/net/mrnustik/university/solitaire/view/images/UNDO_ICON.png")));
        undoBtn.addActionListener(l -> {
            board.undo();
            paintBoard();
        });
        constraints.gridx = 1;
        gameMenu.add(undoBtn, constraints);
        JButton endBtn = new JButton(new ImageIcon(getClass().getResource("/net/mrnustik/university/solitaire/view/images/DELETE_ICON.png")));
        endBtn.addActionListener(l -> {
            MainFrame topFrame = (MainFrame) SwingUtilities.getWindowAncestor(this);
            topFrame.removeGame(this);
        });
        constraints.gridy = 2;
        constraints.gridx = 0;
        gameMenu.add(endBtn, constraints);

        JButton hintButton = new JButton(new ImageIcon(getClass().getResource("/net/mrnustik/university/solitaire/view/images/HINT_ICON.png")));
        hintButton.addActionListener(l -> hintClicked());
        constraints.gridy = 2;
        constraints.gridx = 1;
        gameMenu.add(hintButton, constraints);
    }

    /**
     * Checks whether the game has been won and shows the winning dialog
     */
    private void checkWin() {
        if (board.isWin()) {
            JOptionPane.showMessageDialog(this,
                    "You have successfully won this game!",
                    "Congratulations",
                    JOptionPane.PLAIN_MESSAGE);
        }
    }

    /**
     * Event handler for target stacker view clicked
     * @param index of the stacker that has been clicked on
     */
    private void targetClicked(int index) {
        if (selection.isValid()) {
            boolean success = false;
            if (selection.getType() == Selection.SelectionType.STACKER) {
                success = board.fromStackerToTarget(index);
            } else if (selection.getType() == Selection.SelectionType.WORKING_PACK) {
                success = board.fromWorkingToTarget(selection.getIndex(), index);
                repaintWorkingIndex1 = selection.getIndex();
            }
            if(success) {
                paintBoard();
                checkWin();
            }
            selection.reset();
        } else {
            selection.setType(Selection.SelectionType.TARGET);
            selection.setIndex(index);
        }

    }

    /**
     * Event handler for hint button clicked
     */
    private void hintClicked() {

        java.util.List<Hint> hints = board.getAllHints();
        int delay = 0;
        for(Hint hint : hints)
        {
            Timer timer = new Timer(delay, actionEvent-> highlightHint(hint));
            timer.start();
            timer.setRepeats(false);
            delay += 1000;
        }
    }

    /**
     * Highlights hint on the board.
     * @param hint to be highlighted
     */
    private void highlightHint(Hint hint) {
        if(hint.getFromSelection() != null){
            highlightSelection(hint.getFromSelection());
        }
        if(hint.getToSelection() != null){
            highlightSelection(hint.getToSelection());
        }
    }

    /**
     * Highlights selected card
     * @param selection to be highlighted
     */
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

    /**
     * Shows JFileChooser dialog for game saving.
     * @see JFileChooser
     */
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

    /**
     * Saves game specified into file specified in path
     * @param path to save file
     * @see BoardSaver
     */
    private void saveGame(String path) {
        BoardSaver saver = new JsonBoardSaver();
        try {
            saver.save(path, board);
        } catch (Exception e) {
            JOptionPane.showMessageDialog(this, "Something wrong happened when saving." + e.getMessage());
        }
    }

    /**
     * Repaints the BoardPanel based on the current board state.
     */
    private void paintBoard() {
        paintDeck();

        paintTargets();

        paintWorkingPacks();

        paintScore();
    }

    /**
     * Paints the current score into score label.
     */
    private void paintScore() {
        this.scoreLabel.setText("Score: "+ board.getScore());
    }

    /**
     * Repaints working stacks based on current state of the games.
     */
    private void paintWorkingPacks() {
        if(repaintWorkingIndex1 == -2 && repaintWorkingIndex2 == -2) {
            for (int i = 0; i < 7; i++) {
                workingStacks[i].setStack(board.getWorkingStack(i));
            }
        } else {
            if(repaintWorkingIndex1 >= 0 && repaintWorkingIndex1 < 7)
                workingStacks[repaintWorkingIndex1].setStack(board.getWorkingStack(repaintWorkingIndex1));
            if(repaintWorkingIndex2 >= 0 && repaintWorkingIndex2 < 7)
                workingStacks[repaintWorkingIndex2].setStack(board.getWorkingStack(repaintWorkingIndex2));
        }
        repaintWorkingIndex1 = -2;
        repaintWorkingIndex2 = -2;
    }

    /**
     * Repaints target stack based on the current state of the game.
     */
    private void paintTargets() {
        for (int i = 0; i < 4; i++) {
            targets[i].changeCard(board.getTargetTop(i));
        }
    }

    /**
     * Repaints the game deck based on the current state of the deck.
     */
    private void paintDeck() {
        deck.changeCard(board.getDeckTop());
        stacker.changeCard(board.getStackerTop());
        repaint();
    }

    /**
     * Event called from working stack when a card has been selected
     * @param card selected card
     * @param index index of the working stack
     */
    @Override
    public void onCardSelected(Card card, int index) {
        if (selection.isValid()) {
            boolean success = false;
            if(selection.getCard().equals(card) && selection.getIndex() == index) return;
            if (selection.getType() == Selection.SelectionType.STACKER) {
                success = board.fromStackerToWorking(index);
            } else if (selection.getType() == Selection.SelectionType.TARGET) {
                success = board.fromTargetToWorking(selection.getIndex(), index);
            } else if (selection.getType() == Selection.SelectionType.WORKING_PACK) {
                success = board.fromWorkingToWorking(selection.getIndex(), index, selection.getCard());
                repaintWorkingIndex2 = selection.getIndex();
            }
            if(success) {
                repaintWorkingIndex1 = index;
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
