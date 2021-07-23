package ija.ija2016.homework1.cardpack;

import java.util.ArrayList;
import java.util.Collections;


// TODO predelat na array list
/**
 *
 * @author xmarti76
 */
public class CardStack extends CardDeck {
	// protected ArrayList<Card> deck;
	// protected int cardDeckSize;

	// Zdedim konstruktor
	public CardStack (int size)  { super(size); }

	// Vlastni verejne metody
	public boolean isEmpty ()          { return (deck.size() == 0); }
	public int     size    ()          { return deck.size(); }

	public CardStack takeFrom (Card card) {
		int index = deck.indexOf(card);

		if (index != -1) {
			CardStack output = new CardStack(deck.size() - index);

			for (int i = deck.size() ; i > index; i-- ) {
				output.put(pop());
			}

			Collections.reverse(output.deck);
			return output;
		}
		else
			return null;
	}

}