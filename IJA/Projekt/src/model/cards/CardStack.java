package src.model.cards;

import java.util.ArrayList;

/**
 * Trida reprezentujici zasobnik karet.
 *
 * @author xmarti76
 * @version 2017.5.7
 */
public class CardStack extends CardDeck {
	public CardStack ( int size )  { super( size ); }
	public CardStack ( Card card ) {
		super( -1 );
		put( card );
	}


	/**
	 *  Vlozi zasobnik karet
	 *
	 *  @param  stack  Vkladany balicek
	 *
	 *  @return  <tt>True</tt>, jestli se to povedlo
	 */
	public boolean put ( CardStack stack ) {
		assert ( stack != null );

		return deck.addAll( deck.size(), stack.deck );
	}


	/**
	 *  Vlozi zasobnik karet
	 *
	 *  @param  card  Karta, od ktere se bude brat
	 *
	 *  @return  Vraci ziskany zasobik
	 */
	public CardStack pop ( Card card ) {
		assert ( card != null );

		int index = deck.indexOf( card );

		if ( index != -1 ) {
			CardStack output = new CardStack( deck.size() - index );

			// Postupne vytaham karty z baliku
			for ( int i = index ; i < deck.size() ; ++i ) { output.put( get( i ) ); }
			// Smazu je v puvodnim baliku
			for ( int i = deck.size() - 1 ; i >= index ; --i ) { deck.remove( i ); }

			return output;
		}
		else {
			return null;
		}
	}


	/**
	 *  Precte kartu (ta v baliku zustava)
	 *
	 *  @return  Je-li volana bez parametru, tak vraci <b>horni</b> kartu<br>
	 */
	public Card get () {
		if ( deck == null || deck.isEmpty() ) { return null; }

		return deck.get( deck.size() - 1 );
	}
	/**
	 *  Precte kartu na indexu (ta v baliku zustava)
	 *
	 *  @param  index  Index do baliku, kde 0 znaci nejspodnejsi kartu
	 *  a size() - 1 znaci horni (tedy plati <tt>(get() == get(size()-1))</tt>)<br>
	 *  a <b>neplati</b> (<tt>get() == get(0)</tt>)
	 *
	 *  @return Vraci danou kartu nebo <tt>null</tt> pokud je index mimo rozsah
	 */
	public Card get ( int index ) {
		assert ( index < 0 );

		if ( index > deck.size() -1 || deck == null || deck.isEmpty() ) { return null; }

		return deck.get( index );
	}
	/**
	 *  Precte kartu (ta v baliku zustava)
	 *
	 *  @param  card  Karta, ktera se bude hledat
	 *
	 *  @return Vraci zasobnik karet nebo <tt>null</tt> pokud  nebyla nalezena
	 */
	public CardStack get ( Card card ) {
		assert ( card != null );

		int index = deck.indexOf( card );

		if ( index != -1 ) {
			CardStack output = new CardStack( deck.size() - index );

			// Postupne vytaham karty z baliku
			for ( int i = index ; i < deck.size() ; ++i ) { output.put( get( i ) ); }

			return output;
		}
		else {
			return null;
		}
	}


	/**
	 *  Zjisti, zda je dana karta obsazena v baliku
	 *
	 *  @param  card  Hledana karta
	 *
	 *  @return  <tt>True</tt>, pokud byla naleea
	 */
	public boolean find ( Card card ) {
		assert ( card != null );

		return ( deck.indexOf( card ) != -1 );
	}
}