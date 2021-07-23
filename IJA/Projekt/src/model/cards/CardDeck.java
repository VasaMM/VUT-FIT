package src.model.cards;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Random;

/**
 * Trida reprezentujici balicek karet.
 *
 * @author xmarti76
 * @version 2017.5.8
 */
public class CardDeck {
	/** Seznam vsech karet */
	protected ArrayList<Card> deck;
	/** Maximalni pocet karet v baliku. -1 znamena neomezenou kapacitu*/
	protected int cardDeckSize;

	/**
	 *  Vytvori novy balicek bez karet
	 *
	 *  @param  size  Velikost vytvareneho baliku. Je-li -1, bude balik neomezeny.
	 *
	 */
	public CardDeck ( int size ) {
		assert ( size > 0 || size != -1 );
		
		cardDeckSize = size;
		deck = new ArrayList<>();
	}


	/** Zamicha balicek karet */
	public void mix () {
		Collections.shuffle( deck, new Random( System.nanoTime() ) );
	}


	/** @return Vraci pocet karet v baliku */
	public int size () { return deck.size(); }


	/** @return Vraci <tt>true</tt> je-li balik prazdny */
	public boolean isEmpty () { return deck.size() == 0; }


	/** @return Vraci horni kart, ta je z baliku <b>odstranena</b>! */
	public Card pop () {
		if ( deck == null || deck.isEmpty() ) { return null; }

		Card output = deck.get( deck.size() - 1 );
		deck.remove( deck.size() - 1 );

		return output;
	}


	/**
	 *  Vlozi kartu
	 *
	 *  @param  card  Vkladana karta
	 *
	 *  @return <tt>True</tt> pokud se to povedlo.
	 */
	public boolean put ( Card card ) {
		assert ( card != null );

		// Otestuji, zda lze put provest
		if ( deck.size() >= cardDeckSize && cardDeckSize != -1 )  { return false; }

		else { return deck.add(card); }
	}


	/**
	 *  Prevod na string
	 */
	@Override
	public String  toString () {
		if ( deck == null || deck.isEmpty() ) { return "empty"; }

		String output = "";
		for ( Card card: deck ) {
			String hidden = "";
			if ( ! card.isTurnedFaceUp() ) { hidden = "!"; }
			output += card + hidden + ", ";
		}

		return output.substring( 0, output.length() - 2 );
	}

	/**
	 *  Porovnani dvou baliku karet
	 */
	@Override
	public boolean equals ( Object object ) {
		assert ( object != null );

		if ( this == object ) { return true; }

		if ( object instanceof CardDeck ) {
			CardDeck deck2 = (CardDeck) object;

			if ( deck2.size() != size() ) { return false; }


			for ( int i = deck2.size() - 1 ; i > 0 ; i-- ) {
				if ( !deck2.pop().equals( deck.get(i) ) ) { return false; }
			}

			return true;
		}

		return false;
	}
}