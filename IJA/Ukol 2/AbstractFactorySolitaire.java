package ija.ija2016.homework2.model.board;

import ija.ija2016.homework2.model.cards.Card;
import ija.ija2016.homework2.model.cards.CardDeck;
import ija.ija2016.homework2.model.cards.CardStack;

/**
 *
 * @author xmarti76
 */
public abstract class AbstractFactorySolitaire extends Object {
	public AbstractFactorySolitaire () {}
	
	public abstract CardDeck  createCardDeck    ();
	public abstract Card      createCard        ( Card.Color color, int value );
	public abstract CardDeck  createTargetPack  ( Card.Color color );
	public abstract CardStack createWorkingPack ();
}