package ija.ija2016.homework2.model.cards;

import java.util.ArrayList;

/**
 *
 * @author xmarti76
 */
public class CardStack extends CardDeck {
	protected boolean workingPack;	// Pri put lze umistit pouze krale a postupne stridat barvy a snizovat hodnotu

	public CardStack ( int size )  { super( size ); }
	public CardStack ( int size, boolean working )  {
		super( size );

		workingPack = working;
	}


	
	public boolean put ( Card card ) {
		if ( workingPack ) {
			// Prazdny zasobnik
			if ( get() == null ) {
				if ( card.value() == 13 ) { return deck.add(card); }
				else { return false; }
			}
			else if ( ( get().value() - card.value() - 1 == 0 ) && !get().similarColorTo( card ) ) { return deck.add(card); }
			else { return false; }
			
		}
		else { return deck.add(card); }
	}

	public boolean put ( CardStack stack ) {
		if ( workingPack ) {
			// Prazdny zasobnik
			if ( get() == null ) {
				if ( stack.get().value() == 13 ) { return deck.addAll( deck.size(), stack.deck ); }
				else { return false; }
			}
			else if ( ( get().value() - stack.get( 0 ).value() - 1 == 0 ) && !get().similarColorTo( stack.get( 0 ) ) ) { return deck.addAll( deck.size(), stack.deck ); }
			else { return false; }
			
		}
		else { return deck.addAll( deck.size(), stack.deck ); }
	}

	public CardStack pop ( Card card ) {
		int index = deck.indexOf( card );

		if ( index != -1 ) {
			CardStack output = new CardStack( deck.size() - index );

			// Postupne vytaham karty z baliku
			for ( int i = index ; i < deck.size() ; i++ ) { output.put( get( i ) ); }
			// Smazu je v puvodnim baliku
			for ( int i = deck.size() - 1 ; i >= index ; i-- ) { deck.remove( i ); }

			return output;
		}
		else {
			return null;
		}
	}

}