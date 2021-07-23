package ija.ija2016.homework1.cardpack;

import java.util.ArrayList;

/**
 *
 * @author xmarti76
 */
public class CardDeck {
	// Promenne
	protected ArrayList<Card> deck;
	protected int cardDeckSize;

	// Konstruktor
	CardDeck (int size) {
		// Vstupni podminka
		if (size <= 0)  throw new IllegalArgumentException("Invalid size of CardDeck: " + size);

		cardDeckSize = size;
		deck = new ArrayList<Card>();
	}

	// Vlastni verejne metody
	public static CardDeck createStandardDeck ()  {
		CardDeck output = new CardDeck(52);

		for (int i = 1; i <= 13; i++) { output.put(new Card(Card.Color.CLUBS,    i)); }
		for (int i = 1; i <= 13; i++) { output.put(new Card(Card.Color.DIAMONDS, i)); }
		for (int i = 1; i <= 13; i++) { output.put(new Card(Card.Color.HEARTS,   i)); }
		for (int i = 1; i <= 13; i++) { output.put(new Card(Card.Color.SPADES,   i)); }

		return output;
	}
	public int size () { return cardDeckSize; }
	public void put (Card card) {
		if (deck.size() >= cardDeckSize)  throw new Error("CardDesk is full!");
		
		deck.add(card);
	}
	public Card pop () {
		if (deck.size() == 0)  throw new Error("CardDesk is empty!");
		Card output = deck.get(deck.size() - 1);
		deck.remove(deck.size() - 1);		// FIXME jiny prikaz?

		return output;

	}

	// Prepsane metody
	@Override
	public String  toString () {
		String output = "";
		for (Card card: deck) {
			output += card + ", ";
		}

		return output.substring(0, output.length() - 2);
	}
	@Override
	public boolean equals (Object object) {
		if (this == object) return true;

		if (object != null && object instanceof CardDeck) {
			CardDeck deck2 = (CardDeck) object;

			if (deck2.size() != this.size())
				return false;


			for (int i = deck2.size() - 1 ; i > 0 ; i--) {
				if (!deck2.pop().equals(deck.get(i)))
					return false;
			}

			return true;
		}

		return false;
	}

	// Vnitrni metody
	protected Card readCard (int index) {
		if (index < 0 || index > cardDeckSize)
			throw new IllegalArgumentException("Invalid index of CardDeck: " + index);	
		return deck.get(index);
	}
}