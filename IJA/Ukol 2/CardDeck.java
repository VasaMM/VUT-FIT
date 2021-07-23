package ija.ija2016.homework2.model.cards;

import java.util.ArrayList;

/**
 *
 * @author xmarti76
 */
public class CardDeck {
	// Promenne
	protected ArrayList<Card> deck;
	protected int cardDeckSize;		// Pokud je nastaveno -1 -> neresit
	protected Card.Color allowedColor = null;	// Pri put lze umistit pouze eso a postupne pouze o jedna vetsi
	// protected boolean targetPack = false;	// Pri put lze umistit pouze eso a postupne pouze o jedna vetsi


	// Konstruktor
	public CardDeck ( int size ) {
		// Vstupni podminka
		if ( size <= 0 && size != -1) { throw new IllegalArgumentException( "Invalid size of CardDeck: " + size ); }

		cardDeckSize = size;
		deck = new ArrayList<>();
	}
	// public CardDeck ( int size, boolean target ) {
	// 	this( size );
	// 	targetPack = target;
	// }
	public CardDeck ( int size, Card.Color color ) {
		this( size );
		allowedColor = color;
	}



	// Metody pro zjisteni informaci
	public int size () { return deck.size(); }
	public boolean isEmpty () { return ( deck == null || deck.isEmpty() ); }


	// Metody pro praci s balickem
	public boolean put ( Card card ) {
		// Otestuji, zda lze put provest
		if ( deck.size() >= cardDeckSize && cardDeckSize != -1 )  { return false; }

		// if ( targetPack ) {
		if ( allowedColor != null ) {
			// Prazdny zasobnik
			if ( get() == null ) {
				// if ( card.value() == 1 ) { return deck.add(card); }
				if ( card.value() == 1 && card.color() == allowedColor ) { return deck.add(card); }
				else { return false; }
			}
			// else if ( ( get().value() - card.value() + 1 == 0 ) && get().color() == card.color() ) { return deck.add(card); }
			else if ( ( get().value() - card.value() + 1 == 0 ) && allowedColor == card.color() ) { return deck.add(card); }
			else { return false; }
			
		}
		else { return deck.add(card); }
	}

	public Card pop () {
		if ( deck == null || deck.isEmpty() ) { return null; }

		Card output = deck.get( deck.size() - 1 );
		deck.remove( deck.size() - 1 );

		return output;
	}

	public Card get () {
		if ( deck == null || deck.isEmpty() ) { return null; }

		return deck.get( deck.size() - 1 );
	}
	public Card get ( int index ) {
		if ( index > deck.size() -1 || index < 0 || deck == null || deck.isEmpty() ) { return null; }

		return deck.get( index );
	}


	// public static CardDeck createStandardDeck ()  {
	// 	CardDeck output = new CardDeck( 52 );

	// 	for ( int i = 1; i <= 13; i++ ) { output.put( new Card( Card.Color.CLUBS,    i ) ); }
	// 	for ( int i = 1; i <= 13; i++ ) { output.put( new Card( Card.Color.DIAMONDS, i ) ); }
	// 	for ( int i = 1; i <= 13; i++ ) { output.put( new Card( Card.Color.HEARTS,   i ) ); }
	// 	for ( int i = 1; i <= 13; i++ ) { output.put( new Card( Card.Color.SPADES,   i ) ); }

	// 	return output;
	// }


	// Obecne metody (equlas, toString)
	public String  toString () {
		String output = "";
		for ( Card card: deck ) {
			output += card + ", ";
		}

		return output.substring( 0, output.length() - 2 );
	}
	@Override
	public boolean equals ( Object object ) {
		if ( this == object ) { return true; }

		if ( object != null && object instanceof CardDeck ) {
			CardDeck deck2 = (CardDeck) object;

			if ( deck2.size() != size() ) { return false; }


			for ( int i = deck2.size() - 1 ; i > 0 ; i-- ) {
				if ( !deck2.pop().equals( deck.get(i) ) ) { return false; }
			}

			return true;
		}

		return false;
	}


	// // Vnitrni metody
	// protected Card readCard ( int index ) {
	// 	if ( index < 0 || index > cardDeckSize ) { throw new IllegalArgumentException( "Invalid index of CardDeck: " + index ); }

	// 	return deck.get( index );
	// }
}