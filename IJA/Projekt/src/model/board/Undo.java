package src.model.board;

import src.model.board.ID;
import src.model.cards.Card;
import src.model.cards.CardStack;

/**
 * Trida reprezentujici jeden krok pro implementaci undo
 *
 * @author xmarti76
 * @version 2017.5.8
 */
public class Undo {
	public Undo ( Operation operation_ )                              { operation = operation_; }
	// public Undo ( Operation operation_, Card card_ )                  { operation = operation_; card = card_; }
	public Undo ( Operation operation_, Card card_, CardStack from_ ) { operation = operation_; card = card_; from = from_; }

	/** Vyctovy typ operaci */
	public enum Operation { MOVE, REVEAL, TURN; }

	/** Operace, kterou jsem prováděl */
	private Operation       operation;
	/** Zasobnik, z ktereho jsem odebiral kartu */
	private CardStack       from = null;
	/** Karta, kterou jsem přesouval */
	private Card            card = null;

	/** @return Operace*/
	public Operation getOperation ()  { return operation; }
	/** @return Místo odkud operace probehla*/
	public CardStack getFrom ()  { return from; }
	/** @return Karta*/
	public Card      getCard ()  { return card; }
}
