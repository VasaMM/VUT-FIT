package ija.ija2016.homework2.model.board;

import ija.ija2016.homework2.model.cards.Card;
import ija.ija2016.homework2.model.cards.CardDeck;
import ija.ija2016.homework2.model.cards.CardStack;

import java.util.ArrayList;

/**
 *
 * @author xmarti76
 */
public class FactoryKlondike extends AbstractFactorySolitaire {
	public FactoryKlondike () {}

	public CardDeck createCardDeck ()  {
		CardDeck output = new CardDeck ( 52 );

		for ( int i = 1; i <= 13; i++ ) { output.put( new Card( Card.Color.CLUBS,    i ) ); }
		for ( int i = 1; i <= 13; i++ ) { output.put( new Card( Card.Color.DIAMONDS, i ) ); }
		for ( int i = 1; i <= 13; i++ ) { output.put( new Card( Card.Color.HEARTS,   i ) ); }
		for ( int i = 1; i <= 13; i++ ) { output.put( new Card( Card.Color.SPADES,   i ) ); }

		return output;
	}

	public Card createCard( Card.Color color, int value ) {
		if ( value < 1 || value > 13 ) { return null; }

		return new Card ( color, value );
	}

	public CardDeck createTargetPack(Card.Color color) {
		return new CardDeck ( 13, color );
	}

	public CardStack createWorkingPack() {
		return new CardStack ( -1, true );
	}
}