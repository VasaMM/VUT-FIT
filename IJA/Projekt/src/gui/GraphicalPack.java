package src.gui;

import src.model.board.ID;
import src.model.cards.Card;
import src.model.cards.CardStack;

import javax.sound.midi.SysexMessage;
import javax.swing.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.Stack;

/**
 * Created by xmlejn04 on 06.05.2017.
 */
public class GraphicalPack extends JLabel{

    protected Gui parent;
    protected CardStack modelCards;



    public GraphicalPack() {
        super();
    }


    public GraphicalPack(Gui parent) {
        super();
        this.parent = parent;

        // udelat on click event listener
        this.addMouseListener(new MouseListener() {
            @Override
            public void mouseClicked(MouseEvent e) {
                packClicked();
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

    public GraphicalPack(Gui parent, CardStack cardStack) {

        super();
        modelCards = cardStack;
        this.parent = parent;

        // udelat on click event listener
        this.addMouseListener(new MouseListener() {
            @Override
            public void mouseClicked(MouseEvent e) {
                packClicked();
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


    public void packClicked() {
//        System.out.print("EMPTY: \n");
        //dodelat pro kazdou zvlast kliknuti na pack
    }

    //
    //  Prekresleni karet
    protected void repainPackCards(){
            //odstrani vsechny karty
        this.removeAll();

        int pileSize = this.modelCards.size();

        for (int j = (pileSize - 1); j >= 0; j--) {
            if (this.modelCards.get(j) != null)
            {
                this.add(new GraphicalCard(this.modelCards.get(j), j, this ));
            }



        }
        this.revalidate();
        this.repaint();

    }
}
