package src.gui;

import javax.swing.*;
import javax.swing.border.Border;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import static java.awt.Color.black;
import static java.awt.Color.gray;

/**
 * Created by xmlejn04 on 06.05.2017.
 */


public class GraphicalStock extends GraphicalPack{

    int stockSize;

    public GraphicalStock(Gui rodic, int stockSize) {


        super(rodic);

        this.stockSize = stockSize;

        repainPackCards();

        this.addMouseListener(new MouseListener() {
            @Override
            public void mouseClicked(MouseEvent e) {
                stockClicked();
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

    @Override
    protected void repainPackCards() {
        stockSize = parent.SolitaireGame.getStockSize();
//        if (ss)
        this.removeAll();
        if (stockSize > 0)
        {
//            this.setBounds(0, 0, 75, 100);
            this.setBackground(gray);
            this.setPreferredSize(new Dimension(75, 100));
            Border border = BorderFactory.createLineBorder(Color.BLACK, 1);
            this.setBorder(border);
//            this.setIcon(new ImageIcon("C:\\Users\\olovo\\IdeaProjects\\ija_gui\\src\\src\\gui\\images\\back.png"));
            this.setIcon(new ImageIcon(getClass().getResource("/lib/images/back.png")));
            revalidate();
            repaint();


        }
        else {
            this.setBackground(black);
            this.setPreferredSize(new Dimension(75, 100));
            Border border = BorderFactory.createLineBorder(Color.GRAY, 2);
            this.setIcon(new ImageIcon(getClass().getResource("/lib/images/blank.png")));
            this.setBorder(border);
            revalidate();
            repaint();
        }
    }

    public void stockClicked() {

//        System.out.print("Stock \n");

        parent.SolitaireGame.reveal();
        parent.tmpGWaste.repaintGwaste();

        this.repainPackCards();
        if (parent.SelectedCard != null) {
            parent.SelectedCard.highliteCard(false);
            parent.SelectedCard.repaint();
            parent.SelectedCard = null;
        }
//        parent.SelectedCard.card, this);

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
