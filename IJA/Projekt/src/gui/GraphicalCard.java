package src.gui;

import src.model.board.ID;
import src.model.cards.Card;

import javax.swing.*;
import javax.swing.border.Border;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import static java.awt.Color.gray;

/**
 * Created by xmlejn04 on 06.05.2017.
 */
public class GraphicalCard extends JLabel{

    public Card card;
//    public GraphicalPile pile = null;     //prejmenovat vse z5 ma pile pokud nefunguje pack
        public GraphicalPack pack = null;


    //waste karta prijde sem
    public GraphicalCard(Card karta, GraphicalWaste GWaste) {
        super();
        pack = GWaste;
        card = karta;
        String s =  karta.value() + "" + karta.color();
        card.turnFaceUp();

//        this.setIcon(new ImageIcon("C:\\Users\\olovo\\IdeaProjects\\ija_gui\\src\\src\\gui\\images\\" + s + ".png"));
        this.setIcon(new ImageIcon(getClass().getResource("/lib/images/" + s + ".png")));

        paintCard(GWaste);

        this.addMouseListener(new MouseListener() {
            @Override
            public void mouseClicked(MouseEvent e) {
                mouseClick();
            }
            @Override
            public void mousePressed(MouseEvent mouseEvent) {}
            @Override
            public void mouseReleased(MouseEvent mouseEvent) {}
            @Override
            public void mouseEntered(MouseEvent mouseEvent) {}
            @Override
            public void mouseExited(MouseEvent mouseEvent) {}
        });
    }


    public GraphicalCard(Card karta, int index, GraphicalPack pileLabel) {
        super();
        card = karta;
        pack = pileLabel;

        String s = karta.value() + "" + karta.color();

        if (card.isTurnedFaceUp())
//            this.setIcon(new ImageIcon("C:\\Users\\olovo\\IdeaProjects\\ija_gui\\src\\src\\gui\\images\\" + s + ".png"));
            this.setIcon(new ImageIcon(getClass().getResource("/lib/images/" + s + ".png")));
        else
//            this.setIcon(new ImageIcon("C:\\Users\\olovo\\IdeaProjects\\ija_gui\\src\\src\\gui\\images\\back.png"));
            this.setIcon(new ImageIcon(getClass().getResource("/lib/images/back.png")));


        paintCard(pileLabel, index);

        this.addMouseListener(new MouseListener() {
            @Override
            public void mouseClicked(MouseEvent e) {
                mouseClick();
            }
            @Override
            public void mousePressed(MouseEvent mouseEvent) {}
            @Override
            public void mouseReleased(MouseEvent mouseEvent) {}
            @Override
            public void mouseEntered(MouseEvent mouseEvent) {}
            @Override
            public void mouseExited(MouseEvent mouseEvent) {}
        });
    }

    public void highliteCard(boolean highlight){
        if (highlight)
        {
            Border border = BorderFactory.createLineBorder(Color.ORANGE, 3);
            this.setBorder(border);
            this.repaint();
        }
        else{
            Border border = BorderFactory.createLineBorder(Color.BLACK, 1);
            this.setBorder(border);
            this.repaint();
        }
    }



    //
    //  Kliknuti
    public void mouseClick() {
//        System.out.print("HIGHLIGHT CARD BYL ZAVOLAN\n");
        if (pack != null){
            if (!this.card.isTurnedFaceUp()) {
                pack.parent.SolitaireGame.turn(this.card);
                pack.repainPackCards();

                pack.parent.SelectedCard = null;
            }

            if (pack.parent.SelectedCard == null) {
                if (this.card.isTurnedFaceUp()) {
                    pack.parent.SelectedCard = this;
                    highliteCard(true);
                }
            }
            else{
                if ((pack.parent.SelectedCard == this) || (!pack.parent.SelectedCard.card.isTurnedFaceUp())) {
                    pack.parent.SelectedCard.highliteCard(false);
                    pack.parent.SelectedCard.repaint();
                    pack.parent.SelectedCard = null;
                    return;
                }

                // TODO dodelat pohyby
    //

                //FIXME UDELAT MOVE
//                System.out.print(pack.parent.SolitaireGame.getPile(ID.PileID.P1)+ "\n");
//                System.out.print(pack.parent.SolitaireGame.getPile(ID.PileID.P2)+ "\n");
//                System.out.print(pack.parent.SolitaireGame.getPile(ID.PileID.P3)+ "\n");
//                System.out.print(pack.parent.SolitaireGame.getPile(ID.PileID.P4)+ "\n");
//                System.out.print(pack.parent.SolitaireGame.getPile(ID.PileID.P5)+ "\n");
//                System.out.print(pack.parent.SolitaireGame.getPile(ID.PileID.P6)+ "\n");
//                System.out.print(pack.parent.SolitaireGame.getPile(ID.PileID.P7)+ "\n");

//                System.out.print("Selected card exists\n");

                int moving = 0;
                if ((pack.parent.SelectedCard.card != null) && (this.card != null) && (pack.parent.SelectedCard.card != this.card))
                {
                    pack.parent.SelectedCard.highliteCard(false);
                    moving = pack.parent.SolitaireGame.move(pack.parent.SelectedCard.card, this.card);
//                    System.out.print(moving + "\n");
                }

//                System.out.print(pack.parent.SolitaireGame.getPile(ID.PileID.P1)+ "\n");
//                System.out.print(pack.parent.SolitaireGame.getPile(ID.PileID.P2)+ "\n");
//                System.out.print(pack.parent.SolitaireGame.getPile(ID.PileID.P3)+ "\n");
//                System.out.print(pack.parent.SolitaireGame.getPile(ID.PileID.P4)+ "\n");
//                System.out.print(pack.parent.SolitaireGame.getPile(ID.PileID.P5)+ "\n");
//                System.out.print(pack.parent.SolitaireGame.getPile(ID.PileID.P6)+ "\n");
//                System.out.print(pack.parent.SolitaireGame.getPile(ID.PileID.P7)+ "\n");


                pack.parent.SelectedCard.highliteCard(false);


                if (moving > 0)
                    pack.parent.graphicalMoveCards(pack.parent.SelectedCard , this);


                pack.parent.SelectedCard = null;


//                pack.parent.SelectedCard.remove();
//                    pack.parent graphicalPack a tam reknu ze odkud karta byla odtamtud remove
//                System.out.print("PACK JE: " + pack +"\n");

//                pack.parent.revalidate();
//                pack.parent.repaint();
            }
        }
        else {

        }
    }
    public void wasteClicked (GraphicalPack tmpPack) {
        pack = tmpPack;
        mouseClick();
    }

//    public void paintCard(GraphicalCard label){
//        this.setBounds(0, 0, 75, 100);
//        this.setBackground(gray);
//        Border border = BorderFactory.createLineBorder(Color.BLACK, 1);
//        this.setBorder(border);
//
//        if (pile != null)
//            pile.remove(this);
//
//
//            label.add(this);
//
////        pile = (GraphicalPile)label;
//
//    }

    public void paintCard(GraphicalWaste label){

        // TEST ZDA SE VYKRESLUJI DALSI KARTY
//        Random rand = new Random();
//
//        int n = rand.nextInt(15) + 1;
//
//        this.setBounds(n, 0, 75, 100);
        // TEST ZDA SE VYKRESLUJI DALSI KARTY

        this.setBounds(0, 0, 75, 100);
        this.setBackground(gray);
        Border border = BorderFactory.createLineBorder(Color.BLACK, 1);
        this.setBorder(border);

        if (pack != null)
            pack.remove(this);

        label.add(this);

        pack = label;

    }


    public void paintCard(GraphicalPack label, int index){
        int odsazeni = 15;
        this.setBounds(0, index*odsazeni, 75, 100);
        this.setBackground(gray);
        Border border = BorderFactory.createLineBorder(Color.BLACK, 1);
        this.setBorder(border);

        if (pack != null)
            pack.remove(this);

        label.add(this);

        pack = label;

    }



}

