package src;

import src.Log.Game;

import javax.swing.*;
import java.awt.*;


/**
 * Created by xmlejn04 on 04.05.2017.
 */
public class SolitaireGameMain extends JFrame {

    public static void main(String argv[]) {
        //vytvoreni nove hry
        Game hra = new Game();
        hra.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
    }

}

