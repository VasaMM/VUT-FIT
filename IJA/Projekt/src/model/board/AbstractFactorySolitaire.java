package src.model.board;

import src.model.board.ID;
import src.model.cards.Card;
import src.model.cards.CardStack;

/**
 *  @author   xmarti76
 *  @version  2017.5.8
 */
public abstract class AbstractFactorySolitaire extends Object {
	
	/** TODO list
	*  FIXME  undo => Nefunguje pro MOVE!!!
	*/

	/** Inicializuje novou hru */
	public AbstractFactorySolitaire () {}

	/**  @return Zjisti pocet karet v odebiracim balicku */
	public abstract int getStockSize ();


	/**
	 *  Vraci horni kartu z odhazovaciho balicku
	 *
	 *  Pro jednoduchost nema karta spravne nastaveny <tt>isTurnedFaceUp()</tt>.
	 *  Proto <b>nelze</b> tuto funkci vyuzit (respetive nevraci relativni vysledky).
	 *
	 *  @return  Horni kartu z odhazovaciho balicku.
	 */
	public abstract Card getWaste ();


	/**
	 *  Vraci horni kartu z daneho vyreseneho balicku (foundation)
	 *
	 *  Pro jednoduchost nemaji karty spravne nastaveny <tt>isTurnedFaceUp()</tt>.
	 *  Proto <b>nelze</b> tuto funkci vyuzit (respetive nevraci relativni vysledky).
	 *
	 *  @param  id  ID bloku, z ktereho chci cist.
	 *
	 *  @return  Horni kartu z daneho vyreseneho balicku (foundation).
	 */
	public abstract Card getFoundation ( ID.FoundationID id );


	/**
	 *  Vraci zasobnik karet z daneho sloupce (pile)
	 *
	 *  Zasobnik obsahuje <b>vsechny</b> karty, i ty jeste neotocene.
	 *  Pro spravne vykresleni je nutne volat <tt>isTurnedFaceUp()</tt> nad kazdou kartou.
	 *  Priklad pro vykresleni:<br><br><tt>
	 *  CardStack pile = getPile( ID.PileID.P2 );<br>
	 *  for ( int i = 0 ; i &lt; pile.size() ; ++i ) {<br>
	 *      Card card = pile.get(i);<br>
	 *      if ( card.isTurnedFaceUp ) {<br>
	 *          drawCard( card.value(), card.color() );   // drawCard neni implementovan!<br>
	 *      }<br>
	 *      else {<br>
	 *          drawHiddenCard();    // drawHiddenCard neni implementovan!<br>
	 *      }<br>
	 *  }</tt>
	 *
	 *  @param  id  ID bloku, z ktereho chci cist.
	 *
	 *  @return  Zasobnik karet z daneho sloupce (pile).
	 */
	public abstract CardStack getPile ( ID.PileID id );


	/**
	 *  Otoci jednu kartu ze <tt>stock</tt> do <tt>waste</tt>
	 *
	 *  @return  <tt>True</tt> v pripade, ze doslo k otoceni karty.
	 *  @return  <tt>False</tt> by nemel nastat.
	 */
	public abstract boolean reveal ();


	/**
	 *  Presune kartu na danou kartu
	 *
	 *  @param  from  urcuje kartu, ktera se bude presouvat.
	 *  @param  to    urcuje kartu, na kterou se bude presouvat.
	 *
	 *  @return  <b>0&lt;</b> v pripade, ze doslo k presunu karty, hodnota urcuje pocet presunutych karet
	 *  @return  <b>0</b> v pripade, ze se kartu presunout nepodarilo, protoze tah neodpovidal pravidlum
	 *  napr. kvuli barve atd.
	 *  @return  <b>&gt;0</b> v pripade chyby, tedy presouvala se karta, ktera je uprostred balicku atd.<br>
	 *  Tato situace by nikdy nemela nastat, a znamena bud chybu v navrhu GUI nebo samotne funkce
	 *  Napr. protoze se jedna o nepovoleny presun.
	 */
	public abstract int move ( Card from, Card to );
	

	/**
	 *  Presune kartu na danou pozici
	 *
	 *  @param  from  urcuje kartu, ktera se bude presouvat.
	 *  @param  to    urcuje pile, na ktery se bude presouvat
	 *
	 *  @return  <b>0&lt;</b> v pripade, ze doslo k presunu karty, hodnota urcuje pocet presunutych karet
	 *  @return  <b>0</b> v pripade, ze se kartu presunout nepodarilo, protoze tah neodpovidal pravidlum
	 *  napr. kvuli barve atd.
	 *  @return  <b>&gt;0</b> v pripade chyby, tedy presouvala se karta, ktera je uprostred balicku atd.<br>
	 *  Tato situace by nikdy nemela nastat, a znamena bud chybu v navrhu GUI nebo samotne funkce
	 *  Napr. protoze se jedna o nepovoleny presun.
	 */
	public abstract int move ( Card from, ID.PileID to );
	

	/**
	 *  Presune kartu na danou pozici
	 *
	 *  Je vhodne po presunu volat <tt>win()</tt> viz dokumentace k funkci <tt>win()</tt>.
	 *
	 *  @param  from  urcuje foundation, na ktere se bude presouvat.
	 *  @param  to    urcuje kartu, na kterou se bude presouvat.
	 *
	 *  @return  <b>0&lt;</b> v pripade, ze doslo k presunu karty, hodnota urcuje pocet presunutych karet
	 *  @return  <b>0</b> v pripade, ze se kartu presunout nepodarilo, protoze tah neodpovidal pravidlum
	 *  napr. kvuli barve atd.
	 *  @return  <b>&gt;0</b> v pripade chyby, tedy presouvala se karta, ktera je uprostred balicku atd.<br>
	 *  Tato situace by nikdy nemela nastat, a znamena bud chybu v navrhu GUI nebo samotne funkce
	 *  Napr. protoze se jedna o nepovoleny presun.
	 */
	public abstract int move ( Card from, ID.FoundationID to );

	/**
	 *  Otoci kartu licem (predkem) nahoru
	 *
	 *  <b>Pozor</b>, nelze to volat na karty ve <tt>stock</tt>!
	 *  Jedine karty, na ktere ma smysl tuto funkci pouzit jsou karty v piles, a to jen ty,
	 *  ktere jsou b uplne na konci a jeste nejsou otocene.
	 *  U ostatnich to bud nedava smysl, nebo to odporuje pravidlum a proto se vraci \ false.
	 *
	 *  @param  card  Karta ktera se bude otacet
	 *
	 *  @return  <tt>True</tt> v pripade, ze se karta otocila.
	 *  @return  <tt>False</tt> v pripade, ze kartu otocit nelze.
	 *  Napr. protoze je jiz otocena, nebo ji nelze podle pravidel otocit.
	 */
	public abstract boolean turn ( Card card );


	/**
	 *  Otoci kartu licem (predkem) nahoru
	 *
	 *  @param  id  ID pile, v kterem se ma karta otocit
	 *
	 *  @return  <tt>True</tt> v pripade, ze se karta otocila.
	 *  @return  <tt>False</tt> v pripade, ze kartu nelze otocit, protoze je jiz otocena.
	 */
	public abstract boolean turn ( ID.PileID id );


	/**
	 *  Zjisti, zda jiz neskoncila hra
	 *
	 *  Ma cenu volat jen v nekterych situacich. Napr. pri presunu karty na i foundation
	 *  nebo pri nacteni hry. Naopak napr. pri kroku zpet hra zkoncit nemuze...
	 *
	 *  @return   <tt>True</tt> v pripade konce hry.
	 *  @return   <tt>False</tt> v pripade, ze hra stale pokracuje.
	 */
	public abstract boolean win ();


	/**
	 *  Vrati hru o krok zpet
	 *
	 *  @return  <b>Kladne cislo</b>, pokud se povedlo.
	 *  @return  <b>0</b> znaci, ze uz neni zadny tah zpet v zasobniku - neni potreba nic prekreslovat.
	 *  @return  <b>Zaporne cislo</b> zanci chybu a nemelo by nikdy nastat.
	 */
	public abstract int undo ();


	/**
	 *  Vrati seznam moznych tahu
	 *
	 *  @return  String se seznamem moznych tahu.
	 *  Jedna se o anglicke vety oddelene pomoci <tt>\n</tt>.
	 */
	public abstract String help ();


	/**
	 *  Nacte hru
	 *
	 *  @param path  String s cestou k souboru (absulutni i relativni)
	 *  <b>vcetne</b> koncovky (doporucuji pouzivat .save)
	 *
	 *  @return  <tt>True</tt> v pripade uspesneho nacteni hry.
	 *  @return  <tt>False</tt> v pripade, ze se hru nepodarilo nacist,
	 *  protoze napr. nebylo mozne nacist soubor
	 *  nebo se nejednalo o validni soubor.
	 */
	public abstract boolean loadGame ( String path );


	/**
	 *  Ulozi hru
	 *
	 *  @param path  String s cestou k souboru (absulutni i relativni)
	 *  <b>vcetne</b> koncovky (doporucuji pouzivat .save)
	 *
	 *  @return  <tt>True</tt> v pripade uspesneho ulozeni hry.
	 *  @return  <tt>False</tt> v pripade, ze se hru nepodarilo ulozit,
	 *  protoze napr. nebylo mozne vytvorit soubor.
	 */
	public abstract boolean saveGame ( String path );


	/**
	 * Prevede string s hodńotou karty na odpovidajici <tt>int</tt>
	 *
	 * @param value String s hodnotou A, 2 - 10, J, Q, K
	 *
	 * @return Cislo 1 - 13
	 */
	public abstract int valueToInt ( String value );
}