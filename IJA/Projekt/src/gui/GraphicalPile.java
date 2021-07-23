package src.gui;

import src.Log.Game;
import src.model.board.ID;
import src.model.cards.Card;
import src.model.cards.CardStack;

import javax.swing.*;
import javax.swing.border.Border;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.ArrayList;

import static java.awt.Color.black;
import static java.awt.Color.red;

/**
 * Created by xmlejn04 on 06.05.2017.
 */
public class GraphicalPile extends GraphicalPack {

    private ID.PileID pileId;

    public ArrayList<GraphicalCard> Karty = new ArrayList<>();

    public GraphicalPile(Gui rodic, CardStack pile, ID.PileID piId) {

        super(rodic, pile);
        pileId = piId;
//        System.out.print(pile + "\n");
        int pileSize = pile.size();

//        this.setBackground(red);
        this.setPreferredSize(new Dimension(75, 280));
//        Border border = BorderFactory.createLineBorder(Color.GRAY, 2);
//        this.setBorder(border);

        // vykresleni karet
        super.repainPackCards();


    }





    @Override
    public void packClicked() {
//        System.out.print("Kliknuto na pile\n");
        //dodelat pro kazdou zvlast
       // super.packClicked();



        if ((parent.SelectedCard != null) && (this.Karty.size() == 0)) {
//            System.out.print("prazdnej pile");

//                    System.out.print(parent.SolitaireGame.getPile(ID.PileID.P1)+ "\n");
//                    System.out.print(parent.SolitaireGame.getPile(ID.PileID.P2)+ "\n");
//                    System.out.print(parent.SolitaireGame.getPile(ID.PileID.P3)+ "\n");
//                    System.out.print(parent.SolitaireGame.getPile(ID.PileID.P4)+ "\n");
//                    System.out.print(parent.SolitaireGame.getPile(ID.PileID.P5)+ "\n");
//                    System.out.print(parent.SolitaireGame.getPile(ID.PileID.P6)+ "\n");
//                    System.out.print(parent.SolitaireGame.getPile(ID.PileID.P7)+ "\n");


            //FIXME DODELAT PRESUN
            parent.SelectedCard.highliteCard(false);
            parent.SelectedCard.repaint();
            parent.SolitaireGame.move(parent.SelectedCard.card, this.pileId);
                //FIXME DODELAT PREKRESLENI
            repainPackCards();
            parent.SelectedCard.pack.repainPackCards();


//            System.out.print(parent.SolitaireGame.getPile(ID.PileID.P1)+ "\n");
//            System.out.print(parent.SolitaireGame.getPile(ID.PileID.P2)+ "\n");
//            System.out.print(parent.SolitaireGame.getPile(ID.PileID.P3)+ "\n");
//            System.out.print(parent.SolitaireGame.getPile(ID.PileID.P4)+ "\n");
//            System.out.print(parent.SolitaireGame.getPile(ID.PileID.P5)+ "\n");
//            System.out.print(parent.SolitaireGame.getPile(ID.PileID.P6)+ "\n");
//            System.out.print(parent.SolitaireGame.getPile(ID.PileID.P7)+ "\n");
        }


    }
}





