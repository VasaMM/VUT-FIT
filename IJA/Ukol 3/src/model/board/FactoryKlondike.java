package src.model.board;

import src.model.cards.Card;
import src.model.cards.CardDeck;
import src.model.cards.CardStack;

import java.util.ArrayList;

/**
 * Továrna na výrobu základních instancí hry Solitaire.
 *
 * @author xmarti76
 * @version 2017.4.16
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