package src.Log;

import src.gui.Gui;

import src.model.board.FactoryKlondike;

import javax.swing.*;
import javax.swing.filechooser.FileNameExtensionFilter;
import java.awt.*;
import java.io.File;
import java.util.ArrayList;

/**
 * Created by xmlejn04 on 07.05.2017.
 */
public class Game extends JFrame {


    /**
     *  Vytvari JLabel, kterym se nastavuje zobrazeni hraci desky
     */
    private JLabel hraciDeska;

    /**
     *  Promena uchovavajici barvu pozadi
     */
    Color barva_pozadi =  new Color(146, 207, 83);


    /**
     *  Pole obsahujici vsechny aktivni hry
     */
    private ArrayList<Gui> games = new ArrayList<>();

    public JButton buttonExit = new JButton("Exit");

    /**
     *  JFrame okno
     *  inicializace nove hry
     */
    public Game (){
        super("Solitaire");

        hraciDeska = new JLabel();
        hraciDeska.setVisible(true);
        hraciDeska.setLayout(new GridLayout( 1, 1));

        hraciDeska.setBackground(barva_pozadi);

        this.add(hraciDeska, BorderLayout.CENTER);
        this.setVisible(true);

        CreateNewWindow(0);

        revalidate();
        repaint();
    }

    /**
     *  Oteveni vyskakovaciho okna pro vybrani kam hru ulozit
     *  @return File kam se bude ukladat
     */
    public File PopUpFileSaveWindow(){
        JFileChooser chooser = new JFileChooser();
        FileNameExtensionFilter filter = new FileNameExtensionFilter(
                "Saved games (*.save)", "save");
        chooser.setFileFilter(filter);
        chooser.setCurrentDirectory(new File("."));
        int returnVal = chooser.showSaveDialog(this);
        if(returnVal == JFileChooser.APPROVE_OPTION) {
            System.out.println("Saved game: " +
                    chooser.getSelectedFile().getName());

            if(! chooser.getSelectedFile().getAbsolutePath().endsWith( ".save" )) {
                return new File(chooser.getSelectedFile() + ".save");
            }
            return chooser.getSelectedFile();
        }
        return null;
    }

    /**
     *  Oteveni vyskakovaciho okna pro vybrani kam hru nacist
     *  @return File ktery se bude otevirat
     */
    public File PopUpFileLoadWindow(){
        JFileChooser chooser = new JFileChooser();
        FileNameExtensionFilter filter = new FileNameExtensionFilter(
                "Saved games (*.save)", "save");
        chooser.setFileFilter(filter);
        chooser.setCurrentDirectory(new File("."));
        int returnVal = chooser.showOpenDialog(this);
        if(returnVal == JFileChooser.APPROVE_OPTION) {
            System.out.println("Loading game: " +
                    chooser.getSelectedFile().getName());
            return chooser.getSelectedFile();
        }
        return null;
    }


    /**
     *  Inicializace a vykresleni nove hry
     *  @param sg Ulozena hra
     *  @param gameDimension Cislo okna
     *  @return Uspesnost akce
     */
    public boolean CreateNewWindow(FactoryKlondike sg, int gameDimension) {

        if (games.size() >= 4){
            return false;
        }
        Gui moje = new Gui(sg, this, gameDimension);
        printWindow(moje);

        revalidate();
        repaint();
        return true;
//

    }

    /**
     *  Inicializace a vykresleni nove hry
     *  @param gameDimension Cislo okna
     *  @return Uspesnost akce
     */
    public boolean CreateNewWindow(int gameDimension) {
        if (gameDimension == -1) {
            gameDimension = games.size();
        }
        FactoryKlondike sg = new FactoryKlondike();
        if (games.size() >= 4){
            return false;
        }

        Gui moje = new Gui(sg, this, gameDimension);

        printWindow(moje);

        revalidate();
        repaint();
        return true;
    }

    /**
     *  Zavreni hry
     *  @param close Ukazatel na okno, ktere se ma zavrit
     */
    public void CloseGame(Gui close) {
        if (games.size() == 1){
            //FIXME DODELAT ZASEDNUTI TLACITKA EXIT
//            buttonExit.setEnabled(false);
        }
        games.remove(close);

        hraciDeska.remove(close);


        if (games.size() == 1)
        {
            hraciDeska.setLayout(new GridLayout( 1, 1));
            setSize(new Dimension(600,500));
        }
        else if (games.size() == 2)
        {
            hraciDeska.setLayout(new GridLayout(  1, 2));
            setSize(new Dimension(1200,500));
        }
        else if (games.size() >= 3)
        {
            hraciDeska.setLayout(new GridLayout(  2, 2));
            setSize(new Dimension(1200,1000));
        }

        revalidate();
        repaint();

    }


    /**
     *  Zavreni hry
     *  @param gui Ukazatel na okno, ktere se ma zavrit
     *  @param gameDimension Cislo okna
     */
public void WindowNewGame(Gui gui, int gameDimension){
    CloseGame(gui);
    CreateNewWindow(gameDimension);
    //POUZE PRI KLIKNUTI NA POSLEDNI HRU
    if (gameDimension+1 != games.size()) {
        FactoryKlondike tmpGame;

        for ( int i = 1 ; i <= (games.size() - (gameDimension+1)) ; ++i ) {
            tmpGame = games.get(gameDimension).SolitaireGame;
            CloseGame(games.get(gameDimension));
            CreateNewWindow(tmpGame, gameDimension+i);    
        } 
    }

    revalidate();
    repaint();
}

    private void printWindow(Gui tmp) {

        games.add(tmp);
//        buttonExit.setEnabled(true);
        if (games.size() == 1)
        {
//            buttonExit.setEnabled(false);
            hraciDeska.setLayout(new GridLayout( 1, 1));
            setSize(new Dimension(600,500));
        }

        if (games.size() == 2)
        {
            hraciDeska.setLayout(new GridLayout(  1, 2));
            setSize(new Dimension(1200,500));
        }
        if (games.size() >= 3)
        {
            hraciDeska.setLayout(new GridLayout(  2, 2));
            setSize(new Dimension(1200,1000));
        }
        hraciDeska.add(games.get(games.size()-1));
//        System.out.print("adding"+games.get(games.size()-1)); //debug



//        hraciDeska.add(moje);
//        games pridat hru do arraylistu
//        zmenit layout hraci desky if pocet her > 1 pak hraci deska hraciDeska.setLayout(new GridLayout( 2, 2));
//              pridat vytvorenou game do hraci desky

//        revalidate repaint
        revalidate();
        repaint();

    }

    public void WindowRepaintGame(int gameDimension) {

        if (gameDimension != games.size()) {
            FactoryKlondike tmpGame;

            for ( int i = 0 ; i <= (games.size() - (gameDimension+1)) ; ++i ) {
                tmpGame = games.get(gameDimension).SolitaireGame;
                CloseGame(games.get(gameDimension));
                CreateNewWindow(tmpGame, gameDimension+i);
            }
        }

        revalidate();
        repaint();
    }


}
