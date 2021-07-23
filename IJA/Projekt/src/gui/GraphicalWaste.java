package src.gui;

import src.model.cards.Card;
import src.model.cards.CardStack;

import javax.swing.*;
import javax.swing.border.Border;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

/**
 * Created by xmlejn04 on 06.05.2017.
 */
public class GraphicalWaste extends GraphicalPack{

    public GraphicalCard topWasteCard = null;

    public GraphicalWaste(Gui rodic, CardStack cardS) {
        super(rodic, cardS);

        repainPackCards();

        this.addMouseListener(new MouseListener() {
            @Override
            public void mouseClicked(MouseEvent e) {
                wasteClicked();
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


    private CardStack getCard() {
        return new CardStack(parent.SolitaireGame.getWaste());
    }

    @Override
    protected void repainPackCards() {
        repaintGwaste();

    }

    public void repaintGwaste (){
        Card karta = parent.SolitaireGame.getWaste();
        this.removeAll();
        if (karta == null)
        {
//            System.out.print("KARTA VE WASTE NENI LEGIT\n");
//            this.setIcon(new ImageIcon("C:\\Users\\olovo\\IdeaProjects\\ija_gui\\src\\src\\gui\\images\\blank.png"));
            this.setIcon(new ImageIcon(getClass().getResource("/lib/images/blank.png")));
            Border border = BorderFactory.createLineBorder(Color.BLACK, 1);
            this.setBorder(border);
            this.repaint();
            return;
        }
        else {
//            System.out.print("Waste ma kartu\n");
            if ( topWasteCard != null ){
                this.remove(topWasteCard);
            }

            parent.SelectedCard = topWasteCard = new GraphicalCard(karta, this);

            parent.SelectedCard.wasteClicked(this);

            this.add(topWasteCard);
//            System.out.print("DO LABELU SEM PRIDAL KARTU \n");
        }
            this.repaint();



    }

    public void wasteClicked() {
        Card karta;
        this.removeAll();
        parent.SelectedCard.highliteCard(false);
        parent.SelectedCard.repaint();
        if ( (karta = parent.SolitaireGame.getWaste()) == null  ) {
//            System.out.print("Waste je prazdny\n");
//            this.setIcon(new ImageIcon("C:\\Users\\olovo\\IdeaProjects\\ija_gui\\src\\src\\gui\\images\\blank.png"));
            this.setIcon(new ImageIcon(getClass().getResource("/lib/images/blank.png")));
            Border border = BorderFactory.createLineBorder(Color.BLACK, 1);
            this.setBorder(border);
            this.repaint();
        }
        else{
//            System.out.print("Waste ma kartu\n");


            //waste top card
            if (parent.SelectedCard == null) {
                if ( topWasteCard != null)
                {
                    remove(topWasteCard);
                }

                parent.SelectedCard = topWasteCard = new GraphicalCard(karta, this);

                parent.SelectedCard.wasteClicked(this);
//                Border border = BorderFactory.createLineBorder(Color.ORANGE, 3);
//                this.setBorder(border);

            }
            else{
                // TODO dodelat pohyby
//            move(pile.parent.SelectedCard)
//            pile.parent.SolitaireGame.move(pile.parent.SelectedCard.card, this);

                parent.SelectedCard.highliteCard(false);
                parent.SelectedCard = null;
//                Border border = BorderFactory.createLineBorder(Color.BLACK, 1);
//                this.setBorder(border);
            }
            this.add(topWasteCard);

//            if (2 > 0)
//            {
////            this.setBounds(0, 0, 75, 100);
//                this.setBackground(gray);
//                this.setPreferredSize(new Dimension(75, 100));
//                Border border = BorderFactory.createLineBorder(Color.BLACK, 1);
//                this.setBorder(border);
//
//                //VYKRESLENI BLANK POUZE PRI ZAPNUTI PROGRAMU
//
//                //            this.setIcon(new ImageIcon("C:\\Users\\olovo\\IdeaProjects\\ija_gui\\src\\src\\gui\\images\\blank.png"));
//                this.setIcon(new ImageIcon(getClass().getResource("/src/gui/images/blank.png")));
//
//            }
//            else {
//                this.setBackground(black);
//                this.setPreferredSize(new Dimension(75, 100));
//                Border border = BorderFactory.createLineBorder(Color.GRAY, 2);
//                this.setBorder(border);
//            }



        }
        this.repaint();
        //parent.();\

        //dodelat pro kazdou zvlast kliknuti na pack
    }
}