package src.model.cards;

/**
 * Třída reprezentující jednu kartu.
 *
 * @author xmarti76
 * @version 2017.4.16
 */
public class Card {
	public enum Color {
		CLUBS, DIAMONDS, HEARTS, SPADES;

		@Override public String toString ()  {
			switch ( this ) {
				case CLUBS:    return "C";
				case DIAMONDS: return "D";
				case HEARTS:   return "H";
				case SPADES:   return "S";
			}
			throw new IllegalArgumentException( "Invalid color!" );
		}

		public int valueOf ()  {
			switch ( this ) {
				case CLUBS:    return 1;
				case DIAMONDS: return 2;
				case HEARTS:   return 3;
				case SPADES:   return 4;
			}
			throw new IllegalArgumentException( "Invalid color!" );
		}
	};
	

	// Promenne
	private int        cardValue;
	private Card.Color cardColor;
	private boolean    faceUp;


	// Konstruktor
	public Card ( Card.Color c, int value ) {
		// Vstupni podminka
		if ( value < 1 || value > 13 ) { throw new IllegalArgumentException( "Invalid card value: " + value ); }

		cardColor = c;
		cardValue = value;
		faceUp    = false;
	}


	// Metody pro zjisteni informaci
	public Card.Color color ()          { return cardColor; }
	public int        value ()          { return cardValue; }
	public boolean    isTurnedFaceUp () { return faceUp; }


	// Nastavovaci metody
	public boolean turnFaceUp () {
		if ( isTurnedFaceUp() ) { return false; }
		
		faceUp = true;
		return true;
	}


	// Porovnavaci metody
	public boolean similarColorTo ( Card c ) {
		return (
			( (    color() == Card.Color.HEARTS ||   color() == Card.Color.DIAMONDS )
			&& ( c.color() == Card.Color.HEARTS || c.color() == Card.Color.DIAMONDS ) )

			|| ( ( color() == Card.Color.SPADES ||   color() == Card.Color.CLUBS )
			&& ( c.color() == Card.Color.SPADES || c.color() == Card.Color.CLUBS ) )
		);
	}

	public int compareValue (Card c) {
		return ( value() - c.value() );
	}


	// Obecne metody (equlas, toString)
	@Override public boolean equals ( Object object ) {
		if ( this == object ) { return true; }

		boolean output = false;
		if ( object != null && object instanceof Card ) {
			Card card = (Card) object;
			output = ( card.color() == cardColor && card.value() == cardValue );
		}
		return output;
	}

	@Override public int hashCode () {
		return cardValue * 10 + cardColor.valueOf();
	}

	@Override public String  toString () {
		String output;
		switch ( cardValue ) {
			case 1:  output = "A";  break;
			case 11: output = "J";  break;
			case 12: output = "Q";  break;
			case 13: output = "K";  break;
			default: output = Integer.toString(cardValue);
		}

		output += "(";
		output += cardColor.toString();
		output += ")";

		return output;
	}

}
