package net.mrnustik.university.solitaire.view.panels;

import net.mrnustik.university.solitaire.model.board.Board;
import net.mrnustik.university.solitaire.model.board.Selection;
import net.mrnustik.university.solitaire.model.board.factory.SolitaireFactory;
import net.mrnustik.university.solitaire.model.collections.CardStack;
import net.mrnustik.university.solitaire.presenter.BoardPresenter;
import net.mrnustik.university.solitaire.view.BoardView;
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
import java.lang.reflect.Executable;

/**
 * JPanel used to describe layout of the single game
 * @author Mrnda (Michal Mrnuštík, xmrnus01)
 */
public class BoardPanel extends JPanel implements BoardView, CardStackPanel.CardSelected {


    private final BoardPresenter presenter;

    /**
     * View representing the deckView of cards
     */
    private CardView deckView;

    /**
     * View representing the put-down stackView of cards
     */
    private CardView stackView;

    /**
     * View representing the target stackView
     */
    private CardView[] targets;

    /**
     * View representing the working stacks
     */
    private CardStackPanel[] workingStacks;

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
        setMinimumSize(new Dimension(200, 150));
        setBackground(Color.decode("#146a2c"));
        initViews();
        this.presenter = new BoardPresenter(this, board);
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
            CardStackPanel panel = new CardStackPanel(i);
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
            targets[i].addActionListener(l -> presenter.targetClicked(index));
            add(targets[i]);
        }
    }

    /**
     * Initializes the deck and put-down stack view
     */
    private void initDeck() {
        deckView = new CardView(null);
        add(deckView);
        stackView = new CardView(null);
        add(stackView);
        deckView.addActionListener(l -> presenter.deckClicked());
        stackView.addActionListener(l -> presenter.stackerClicked());
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

        //score label
        scoreLabel = new JLabel("Score: 0", SwingConstants.CENTER);
        scoreLabel.setForeground(Color.white);
        gameMenu.add(scoreLabel, constraints);

        //save button
        JButton saveBtn = new JButton(new ImageIcon(getClass().getResource("/net/mrnustik/university/solitaire/view/images/SAVE_ICON.png")));
        saveBtn.addActionListener(l -> showSaveGameDialog());
        constraints.gridy = 1;
        constraints.gridx = 0;
        constraints.gridwidth = 1;
        gameMenu.add(saveBtn, constraints);

        //undo button
        JButton undoBtn = new JButton(new ImageIcon(getClass().getResource("/net/mrnustik/university/solitaire/view/images/UNDO_ICON.png")));
        undoBtn.addActionListener(l -> presenter.undoClicked());
        constraints.gridx = 1;
        gameMenu.add(undoBtn, constraints);

        //end button
        JButton endBtn = new JButton(new ImageIcon(getClass().getResource("/net/mrnustik/university/solitaire/view/images/DELETE_ICON.png")));
        endBtn.addActionListener(l -> {
            MainFrame topFrame = (MainFrame) SwingUtilities.getWindowAncestor(this);
            topFrame.removeGame(this);
        });
        constraints.gridy = 2;
        constraints.gridx = 0;
        gameMenu.add(endBtn, constraints);

        //hint button
        JButton hintButton = new JButton(new ImageIcon(getClass().getResource("/net/mrnustik/university/solitaire/view/images/HINT_ICON.png")));
        hintButton.addActionListener(l -> presenter.hintClicked());
        constraints.gridy = 2;
        constraints.gridx = 1;
        gameMenu.add(hintButton, constraints);
    }


    /**
     * Highlights selected card
     * @param selection to be highlighted
     */
    @Override
    public void highlightSelection(Selection selection) {
        if(selection.getType() == Selection.SelectionType.DECK){
            deckView.highlight();
        } else if(selection.getType() == Selection.SelectionType.STACKER) {
            stackView.highlight();
        } else if(selection.getType() == Selection.SelectionType.TARGET) {
            int index = selection.getIndex();
            targets[index].highlight();
        } else if(selection.getType() == Selection.SelectionType.WORKING_PACK) {
            int index = selection.getIndex();
            workingStacks[index].highlight(selection.getCard());
        }
    }

    @Override
    public void showWin() {
        JOptionPane.showMessageDialog(this,
                "You have successfully won this game!",
                "Congratulations",
                JOptionPane.PLAIN_MESSAGE);
    }

    @Override
    public void showScore(int score) {
        scoreLabel.setText(String.format("Score: %d", score));
    }

    @Override
    public void repaintDeck(Card deck, Card stacker) {
        deckView.changeCard(deck);
        stackView.changeCard(stacker);
    }

    @Override
    public void repaintTargets(Card card, int index) {
        targets[index].changeCard(card);
    }

    @Override
    public void repaintWorking(CardStack stack, int index) {
        workingStacks[index].setStack(stack);
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
            try {
                presenter.saveBoard(file.getPath());
            } catch (Exception ignored) {
                JOptionPane.showMessageDialog(this, "An error occurred during saving.");
            }
        }
    }

    /**
     * Event called from working stack when a card has been selected
     * @param card selected card
     * @param index index of the working stack
     */
    @Override
    public void onCardSelected(Card card, int index) {
        presenter.workingClicked(index, card);
    }


}
