package src.gui;

import src.Log.Game;
import src.model.board.FactoryKlondike;
import src.model.board.ID;
import src.model.cards.CardStack;

import javax.swing.*;
import javax.swing.border.Border;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.File;
import java.util.ArrayList;


/**
 * Created by olovo on 05.05.2017.
 */
public class Gui extends JLabel {



    public GraphicalCard SelectedCard = null;
   public FactoryKlondike SolitaireGame = null;
    public GraphicalWaste tmpGWaste = null;
    public Game ParentGame = null;

    String TimeDateYear = null;
    boolean insideActionlistenerSave = false;

    private JButton buttonNewGame;
    private JButton buttonExit;
    private JButton buttonSaveGame;
    private JButton buttonNewWindow;
    private JComboBox comboBox1;
    private JLabel labelLoad;
    private int gameDimension;
//    private JPanel panel1;
//    private JPanel menuBar;


    private JPanel panel1 = new JPanel();
    private JPanel panel2 = new JPanel();

    //bude potreba na vice her
    public Gui(FactoryKlondike sg, Game game, int gd) {
        super();
        ParentGame = game;
        gameDimension = gd;
        SolitaireGame = sg;
//        buttonExit = ParentGame.buttonExit;

        Border border = BorderFactory.createLineBorder(Color.GRAY, 2);
        setBorder(border);
        this.setPreferredSize(new Dimension( 650, 500));
        this.setSize(new Dimension( 650, 500));
        this.setMaximumSize(new Dimension( 650, 500));


        Color barva_pozadi = new Color(146, 207, 83);
        this.setBackground(barva_pozadi);

        // getContentPane().setBackground(Color.green.darker() );
//        cislo1Field = new JTextField("1. číslo", 5);
//        cislo2Field = new JTextField("2. číslo", 5);
//        sectiBut = new JButton("Sečti");
//        vysledekLab = new JLabel("Výsledek je: ");
//    }

        //System.out.print("Hello World");

//        frame = new JFrame("Frame1");
//        frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
//
//        frame.setLocation(200, 200);


//        GraphicsBoard gb;
//        JMenu menu;
//        JMenuItem menuItem;
//        JMenuBar menuBar;


//        JMenuItem menuItem = new JMenuItem("game 1");
//        frame.add(menuItem);
//        add(frame);


        //*************************************************************************************************
        //************  vykresleni menu ************
        //*************************************************************************************************

        JPanel panelMenu = new JPanel();

        FlowLayout layout = new FlowLayout();
        setLayout(layout);



        //*************************************************************************************************
        JButton buttonNewGame = new JButton("NewGame");
        panelMenu.add(buttonNewGame);
        buttonNewGame.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent e)
            {

//                    System.out.print(SolitaireGame.getPile(ID.PileID.P1)+ "\n");
//                    System.out.print(SolitaireGame.getPile(ID.PileID.P2)+ "\n");
//                    System.out.print(SolitaireGame.getPile(ID.PileID.P3)+ "\n");
//                    System.out.print(SolitaireGame.getPile(ID.PileID.P4)+ "\n");
//                    System.out.print(SolitaireGame.getPile(ID.PileID.P5)+ "\n");
//                    System.out.print(SolitaireGame.getPile(ID.PileID.P6)+ "\n");
//                    System.out.print(SolitaireGame.getPile(ID.PileID.P7)+ "\n");

                ParentGame.WindowNewGame(Gui.this, gameDimension);
//                    game.CloseGame(Gui.this, gameDimension);
//                    game.CreateNewWindow(gameDimension);
//                System.out.print("dimenze byla:" + gameDimension);

                    repaint();
                    revalidate();

//                    System.out.print(SolitaireGame.getPile(ID.PileID.P1)+ "\n");
//                    System.out.print(SolitaireGame.getPile(ID.PileID.P2)+ "\n");
//                    System.out.print(SolitaireGame.getPile(ID.PileID.P3)+ "\n");
//                    System.out.print(SolitaireGame.getPile(ID.PileID.P4)+ "\n");
//                    System.out.print(SolitaireGame.getPile(ID.PileID.P5)+ "\n");
//                    System.out.print(SolitaireGame.getPile(ID.PileID.P6)+ "\n");
//                    System.out.print(SolitaireGame.getPile(ID.PileID.P7)+ "\n");

            }

        });


        //*********************************************Undo****************************************************
        JButton buttonUndo = new JButton("Undo");
        panelMenu.add(buttonUndo);

        buttonUndo.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent e)
            {
//                System.out.print("KARTA NA VRCHOLU VASTE JE: " + SolitaireGame.getWaste() + "\n");

                if (SolitaireGame.undo() > 0){
//                    System.out.print("KARTA NA VRCHOLU VASTE JE: " +  SolitaireGame.getWaste() + "\n");
//                    repainPackCards();

//                    System.out.print(SolitaireGame.getPile(ID.PileID.P1)+ "\n");
//                    System.out.print(SolitaireGame.getPile(ID.PileID.P2)+ "\n");
//                    System.out.print(SolitaireGame.getPile(ID.PileID.P3)+ "\n");
//                    System.out.print(SolitaireGame.getPile(ID.PileID.P4)+ "\n");
//                    System.out.print(SolitaireGame.getPile(ID.PileID.P5)+ "\n");
//                    System.out.print(SolitaireGame.getPile(ID.PileID.P6)+ "\n");
//                    System.out.print(SolitaireGame.getPile(ID.PileID.P7)+ "\n");

                    ParentGame.WindowRepaintGame(gameDimension);

//                    tmpGWaste.repaintGwaste();
//                    tmpGWaste.repainPackCards();
//                    tmpGWaste.revalidate();
//                    tmpGWaste.repaint();
                    repaint();
                    revalidate();

//                    System.out.print(SolitaireGame.getPile(ID.PileID.P1)+ "\n");
//                    System.out.print(SolitaireGame.getPile(ID.PileID.P2)+ "\n");
//                    System.out.print(SolitaireGame.getPile(ID.PileID.P3)+ "\n");
//                    System.out.print(SolitaireGame.getPile(ID.PileID.P4)+ "\n");
//                    System.out.print(SolitaireGame.getPile(ID.PileID.P5)+ "\n");
//                    System.out.print(SolitaireGame.getPile(ID.PileID.P6)+ "\n");
//                    System.out.print(SolitaireGame.getPile(ID.PileID.P7)+ "\n");

                }
            }
        });



        //*********************************************SAVE GAME****************************************************
        JButton buttonSaveGame = new JButton("Save");
        panelMenu.add(buttonSaveGame);

        buttonSaveGame.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent e)
            {

                File temp = ParentGame.PopUpFileSaveWindow();
                if (temp != null){
                    SolitaireGame.saveGame(temp.toString());
                }

                revalidate();
                repaint();
            }
        });


        //*********************************************SAVE GAME****************************************************
        JButton buttonLoadGame = new JButton("Load");
        panelMenu.add(buttonLoadGame);

        buttonLoadGame.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent e)
            {

                File temp = ParentGame.PopUpFileLoadWindow();
                if (temp != null){
                    SolitaireGame.loadGame(temp.toString());
                }

                ParentGame.WindowRepaintGame(gameDimension);
                revalidate();
                repaint();
            }
        });

        JButton buttonHelp = new JButton("Help");
        panelMenu.add(buttonHelp);

        buttonHelp.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent e)
            {
                JOptionPane.showMessageDialog(ParentGame, SolitaireGame.help());
            }
        });


//        JButton buttonLoad = new JButton("LoadGame");
//        panelMenu.add(buttonLoad);
        JButton buttonNewWindow = new JButton("NewWindow");
        panelMenu.add(buttonNewWindow);
        buttonNewWindow.addActionListener(new ActionListener()
        {
            public void actionPerformed(ActionEvent e)
            {
                ParentGame.CreateNewWindow(-1);
                repaint();
                revalidate();

//                CreateNewWindow();
//        hraciDeska.add(games.get(1));
//        CreateNewWindow();
//        hraciDeska.add(games.get(2));
//        CreateNewWindow();
//        hraciDeska.add(games.get(3));
            }
        });




        JButton buttonExit = new JButton("Exit");
        panelMenu.add(buttonExit);
        add(panelMenu);
        buttonExit.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e)
            {
                ParentGame.CloseGame(Gui.this);
                repaint();
                revalidate();
            }
        });





        //*************************************************************************************************
        //************  vykresleni karet ************
        //*************************************************************************************************

        JPanel panelCards = new JPanel();
//        JLabel label = new JLabel("Test");
//        panelCards.add(label);



            //vykresleni stock balicku
        panelCards.add(new GraphicalStock(this, SolitaireGame.getStockSize()));

            //vykresleni waste balicku
        tmpGWaste = new GraphicalWaste(this, new CardStack(SolitaireGame.getWaste()));
        tmpGWaste.setVisible(true);
//        Border rder = BorderFactory.createLineBorder(Color.ORANGE, 3);
//        tmpGWaste.setBorder(rder);
        panelCards.add(tmpGWaste);

        tmpGWaste.setSize(new Dimension(75, 100));
        tmpGWaste.setMinimumSize(new Dimension(75, 100));
        tmpGWaste.setPreferredSize(new Dimension(75, 100));
        revalidate();
        repaint();





        JLabel label2 = new JLabel("");
        panelCards.add(label2);


        panelCards.add(new GraphicalFoundation(this, ID.FoundationID.F1));
        panelCards.add(new GraphicalFoundation(this, ID.FoundationID.F2));
        panelCards.add(new GraphicalFoundation(this, ID.FoundationID.F3));
        panelCards.add(new GraphicalFoundation(this, ID.FoundationID.F4));


        //        label.setOpaque(false);





        label2.setPreferredSize(new Dimension(75, 100));


//        panelCards.setBackground(barva_pozadi);
        add(panelCards);


        //************  vykresleni sloupcu karet ************
//        label.setText();
        JPanel panelCardStacks = new JPanel();
//        panelCardStacks.add(label);


        //private ArrayList<FactoryKlondike> solitaire = new ArrayList<>();
        ArrayList<JLabel> labelList = new ArrayList<>();

//        System.out.print(Game.solitaireGame.getPile(ID.PileID.P1)+ "\n");
//        System.out.print(Game.solitaireGame.getPile(ID.PileID.P2)+ "\n");
//        System.out.print(Game.solitaireGame.getPile(ID.PileID.P3)+ "\n");
//        System.out.print(Game.solitaireGame.getPile(ID.PileID.P4)+ "\n");
//        System.out.print(Game.solitaireGame.getPile(ID.PileID.P5)+ "\n");
//        System.out.print(Game.solitaireGame.getPile(ID.PileID.P6)+ "\n");
//        System.out.print(Game.solitaireGame.getPile(ID.PileID.P7)+ "\n");


        ArrayList<GraphicalPile> piles = new ArrayList<>();
        CardStack tmp;
        for (int i = 0; i < 7; ++i) {
//            System.out.print(Game.solitaireGame.getPile(ID.PileID.valueOf("P" + (i + 1))) + "\n");
//            System.out.print(Game.solitaireGame.getPile(ID.PileID.valueOf("P"+ (i+1))).size() + "\n");

            piles.add(new GraphicalPile(this, SolitaireGame.getPile(ID.PileID.valueOf("P" + (i + 1))), ID.PileID.valueOf("P" + (i + 1)) ));
            panelCardStacks.add(piles.get(i));

//            panelCardStacks.add(piles.get(i));
//              panelCardStacks.add(labelList.get(i));

//            panelCardStacks.setBackground(barva_pozadi);


        }
        add(panelCardStacks);
//        add(panelCardStacks);
        repaint();
//
//            public void paintComponent(Graphics g) {
//                g.drawImage(icon.getImage(), 0, 0, null);
//                super.paintComponent(g);
//            }

//        panelCards.add( label );


//        FlowLayout layout = new FlowLayout();
//        setLayout(layout);

//        panel1 = new JPanel();
//        add(panel1);
//
//        Color barva = new Color(0, 150, 0);
//        panel2 = new JPanel();
//        panel2.setBackground(barva);
//        add(panel2);
//
//        JTextField cislo1Field = new JTextField("1. èíslo", 5);
//        panel1.add(cislo1Field);
//
//        JTextField cislo2Field = new JTextField("2. èíslo", 5);
//        panel1.add(cislo2Field);
//
//        JButton sectiBut = new JButton("Seèti");
//        panel1.add(sectiBut);
//
//        JLabel vysledekLab = new JLabel("Výsledek je ");
//        panel2.add(vysledekLab);
    }





    // Karta se posunula, prekresleni dvou balicku
    public void graphicalMoveCards(GraphicalCard card, GraphicalCard card1) {
//        System.out.print("GRAPHICAL CARD MOVE \n");
//        System.out.print(card);
        if ((card.pack != null) && (card1.pack != null))
        {
            if (card.pack == card1.pack)
            {
                return; //chyba v logice????
            }

            // Prekresleni dvou balicku
            card.pack.repainPackCards();
            card1.pack.repainPackCards();

            // Zkontroluji, zda jsem nedohral
            if (SolitaireGame.win()) {
                PopUpWin();
            }
        }
    }

    public void PopUpWin() {
        JOptionPane.showMessageDialog(ParentGame, "!!!Contugratulation you have won this Solitaire game!!!");
    }

}