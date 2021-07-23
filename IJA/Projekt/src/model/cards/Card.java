package src.model.cards;

/**
 * Trida reprezentujici jednu kartu.
 *
 * @author xmarti76
 * @version 2017.5.8
 */
public class Card {
	/**
	 *  Vyctovy typ pro barvu
	 *
	 *  Obsahuje dve metody:<br>
	 *  <tt>toSring()</tt> pro prevod na retezec<br>
	 *  <tt>valueOf()</tt> pro prevod na \c int
	 */
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

		public static Color fromString( String value ) {
			switch ( value ) {
				case "C": return CLUBS;
				case "D": return DIAMONDS;
				case "H": return HEARTS;
				case "S": return SPADES;
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
	

	/** Hodnota karty */
	private int        cardValue;
	/** Barva karty */
	private Card.Color cardColor;
	/** Je karta utucena licem nahoru */
	private boolean    faceUp;


	// Konstruktor
	public Card ( Card.Color c, int value ) {
		assert ( c != null );
		assert ( value > 0 && value < 13 );

		cardColor = c;
		cardValue = value;
		faceUp    = false;
	}
	public Card ( Card.Color c, int value , boolean faceUp_ ) {
		faceUp = faceUp_;
	}


	// Metody pro zjisteni informaci
	/** @return Vraci barvu karty */
	public Card.Color color ()          { return cardColor; }
	/** @return Vraci hodnotu karty */
	public int        value ()          { return cardValue; }
	/** @return Vraci, zda je karta otocena */
	public boolean    isTurnedFaceUp () { return faceUp; }


	/**
	 *  Otoci kartu licem nahoru
	 *
	 *  @return <tt>True</tt>, pokud se to povedlo
	 */
	public boolean turnFaceUp () {
		if ( isTurnedFaceUp() ) { return false; }
		
		faceUp = true;
		return true;
	}


	/** Otoci kartu licem dolu */
	public void turnFaceDown ()  { faceUp = false; }


	/**
	 *  Zjisti, zda maji karty stejnou barvu (cervena, cerna)
	 *
	 *  @param card Ocekava kartu na provnani
	 *
	 *  @return <tt>True</tt>, pokud jsou stejne
	 */
	public boolean similarColorTo ( Card card ) {
		assert ( card != null );

		return (
			( (       color() == Card.Color.HEARTS ||      color() == Card.Color.DIAMONDS )
			&& ( card.color() == Card.Color.HEARTS || card.color() == Card.Color.DIAMONDS ) )

			|| ( (    color() == Card.Color.SPADES ||      color() == Card.Color.CLUBS )
			&& ( card.color() == Card.Color.SPADES || card.color() == Card.Color.CLUBS ) )
		);
	}

	/**
	 *  Porovna hodnoty karet
	 *
	 *  @param  card  Ocekava kartu na porovnani
	 *
	 *  @return  Vraci <i>0</i>, pokud jsou <b>stejne</b>
	 *  @return  Vraci <i>-x</i>, pokud je karta <b>vetsi</b>, kde <i>x</i> urcuje rozdil
	 *  @return  Vraci <i>+x</i>, pokud je karta <b>mensi</b>, kde <i>x</i> urcuje rozdil
	 */
	public int compareValue ( Card card ) {
		assert ( card != null );

		return ( value() - card.value() );
	}


	/**
	 *  Porovna karty
	 */
	@Override public boolean equals ( Object object ) {
		if ( object == null )  { return false; }

		if ( this == object )  { return true; }

		boolean output = false;
		if ( object instanceof Card ) {
			Card card = (Card) object;
			output = ( card.color() == cardColor && card.value() == cardValue );
		}
		return output;
	}

	/**
	 *  Spocita hash karty
	 */
	@Override public int hashCode () {
		return cardValue * 10 + cardColor.valueOf();
	}

	/**
	 *  Prevede kartu na string
	 */
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
