//package src.gui;
//
//import src.model.board.FactoryKlondike;
//
//import javax.swing.*;
//import javax.swing.border.Border;
//import java.awt.*;
//
//import static java.awt.Color.gray;
//
///**
// * Created by xmlejn04 on 07.05.2017.
// */
//public class GuiWindow  extends JFrame{
//
//    public GuiWindow(FactoryKlondike fk, int gridPosition) throws HeadlessException {
//        super("Solitaire");
//
//        GridLayout layout = new GridLayout(2, 2);
//        setLayout(layout);
//        this.setComponentOrientation(ComponentOrientation.LEFT_TO_RIGHT);
//
//
//        Gui okno = new Gui(fk);
//        if (gridPosition == 1)
//            add(okno);
//
//
//
//
//
//        Color barva_pozadi = new Color(146, 207, 83);
//        getContentPane().setBackground(barva_pozadi);
////        okno.setBackground(barva_pozadi);
//
//
//        okno.setVisible(true);
//
////        okno.setSize(650, 500);
//
////        this.setPreferredSize(new Dimension( 650, 500));
////        this.setSize(new Dimension( 650, 500));
////        this.setMaximumSize(new Dimension( 650, 500));
//
////        okno.setBounds(0,0, 75, 100);
//
//
//
//
////        okno.setPreferredSize( new Dimension( 650, 500) );
//        setResizable(false);
//        revalidate();
//        repaint();
//    }
//}
