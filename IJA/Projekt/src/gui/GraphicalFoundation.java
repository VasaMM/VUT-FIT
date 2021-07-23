package src.gui;

import src.model.board.ID;
import src.model.cards.Card;
import src.model.cards.CardStack;

import javax.swing.*;
import javax.swing.border.Border;
import java.awt.*;

import static java.awt.Color.black;
import static java.awt.Color.gray;

/**
 * Created by xmlejn04 on 06.05.2017.
 */
public class GraphicalFoundation extends GraphicalPack{


    private ID.FoundationID foundId;
    private GraphicalCard topFoundationCard = null;

    //    public GraphicalPile(Gui rodic, CardStack pile) {
//
//        super(rodic);
    public GraphicalFoundation(Gui rodic, ID.FoundationID founId) {

//        parent = rodic;
        // Zalozime stack o jedne karte z foundation
        super(rodic, new CardStack(rodic.SolitaireGame.getFoundation(founId)));

        this.foundId = founId;

        Card karta = rodic.SolitaireGame.getFoundation(founId);

        if (karta == null)
        {
//            this.setBounds(0, 0, 75, 100);
            this.setBackground(gray);
            this.setPreferredSize(new Dimension(75, 100));
            Border border = BorderFactory.createLineBorder(Color.BLACK, 1);
            this.setBorder(border);
//            this.setIcon(new ImageIcon("C:\\Users\\olovo\\IdeaProjects\\ija_gui\\src\\src\\gui\\images\\blank.png"));
            this.setIcon(new ImageIcon(getClass().getResource("/lib/images/blank.png")));

        }
        else {
            String s = karta.value() + "" + karta.color();
            this.setBackground(black);
            this.setPreferredSize(new Dimension(75, 100));
            Border border = BorderFactory.createLineBorder(Color.BLACK, 1);
//            this.setIcon(new ImageIcon(getClass().getResource("/lib/images/" + s + ".png")));
            this.setBorder(border);
        }
        repainPackCards();
        revalidate();
        repaint();
    }


    private CardStack getCard() {
        return new CardStack(parent.SolitaireGame.getFoundation(foundId));
    }



    @Override
    public void packClicked() {
//        System.out.print("Kliknuto na foundation\n");
        //dodelat pro kazdou zvlast
        // super.packClicked();

            //FIXME DODELAT KLIKNUTI
        if ( parent.SelectedCard != null){
            parent.SelectedCard.highliteCard(false);
            parent.SelectedCard.repaint();
            parent.SolitaireGame.move(parent.SelectedCard.card, this.foundId);
            // if (parent.SolitaireGame.win()) {
            //     parent.PopUpWin();
            // }

        //FIXME DODELAT GRAFIKU

            repainPackCards();

            parent.SelectedCard.pack.repainPackCards();
            parent.SelectedCard = null;
        }
    }

    @Override
    protected void repainPackCards() {
        this.modelCards = getCard(); // Museime nacist novou kartu

        super.repainPackCards();

        Card karta = parent.SolitaireGame.getFoundation(foundId);
        if (karta == null){
//            this.setBounds(0, 0, 75, 100);
            this.setBackground(gray);
            this.setPreferredSize(new Dimension(75, 100));
            Border border = BorderFactory.createLineBorder(Color.BLACK, 1);
            this.setBorder(border);
//            this.setIcon(new ImageIcon("C:\\Users\\olovo\\IdeaProjects\\ija_gui\\src\\src\\gui\\images\\back.png"));
            this.setIcon(new ImageIcon(getClass().getResource("/lib/images/blank.png")));
            revalidate();
            repaint();


        }
        else {
            String s = karta.value() + "" + karta.color();
            this.setBackground(black);
            this.setPreferredSize(new Dimension(75, 100));
            Border border = BorderFactory.createLineBorder(Color.BLACK, 1);
            this.setIcon(new ImageIcon(getClass().getResource("/lib/images/" + s + ".png")));
            this.setBorder(border);

            revalidate();
            repaint();
    }


    }








    //        JLabel label = new JLabel("") {
//            public void paintComponent(Graphics g) {
//                g.drawImage(icon.getImage(), 0, 0, null);
//                super.paintComponent(g);
//            }
//        };


//        panelCards.add(label);
//        label.setMinimumSize(new Dimension(75, 100));
//        label.setPreferredSize(new Dimension(75, 100));
//        label.setMaximumSize(new Dimension(75, 100));

}
