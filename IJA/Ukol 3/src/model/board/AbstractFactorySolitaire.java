package src.model.board;

import src.model.cards.Card;
import src.model.cards.CardDeck;
import src.model.cards.CardStack;

/**
 * Tady by měl být popis. TODO
 *
 * @author xmarti76
 * @version 2017.4.16
 */
public abstract class AbstractFactorySolitaire extends Object {
	public AbstractFactorySolitaire () {}
	
	public abstract CardDeck  createCardDeck    ();
	public abstract Card      createCard        ( Card.Color color, int value );
	public abstract CardDeck  createTargetPack  ( Card.Color color );
	public abstract CardStack createWorkingPack ();
}