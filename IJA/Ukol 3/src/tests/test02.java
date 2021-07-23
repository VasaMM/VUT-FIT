package src.tests;

import src.model.board.AbstractFactorySolitaire;
import src.model.board.FactoryKlondike;
import src.model.cards.Card;
import src.model.cards.CardDeck;
import src.model.cards.CardStack;
import java.util.HashSet;
import java.util.Set;
import org.junit.After;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

/**
 * IJA 2016/2017: Testovaci trida pro ukol c. 3.
 * @author xmarti76
 * @version 2017.4.16
 */
public class test02 {

    protected AbstractFactorySolitaire factory;

    @Before
    public void setUp() {
        factory = new FactoryKlondike();
    }

    @After
    public void tearDown() {
    }

    @Test
    public void testCardDeck() {
        CardDeck deck = factory.createCardDeck();

        Assert.assertEquals("Pocet karet je 52", 52, deck.size());

        Set<Card> testCards = new HashSet<>();
        for (int i = 1; i <= 13; i++) {
            testCards.add(factory.createCard(Card.Color.CLUBS, i));
        }
        for (int i = 1; i <= 13; i++) {
            testCards.add(factory.createCard(Card.Color.DIAMONDS, i));
        }
        for (int i = 1; i <= 13; i++) {
            testCards.add(factory.createCard(Card.Color.HEARTS, i));
        }
        for (int i = 1; i <= 13; i++) {
            testCards.add(factory.createCard(Card.Color.SPADES, i));
        }

        for (int i = 0; i < 52; i++) {

//            System.out.println(deck.get().value());
//            System.out.println(((i % 13) == 0) ? 13 : (i % 13));
//            System.out.println("i je: ");
//            System.out.println(13-(i % 13));



            Assert.assertEquals("Pocet karet je 52",((i % 13) == 0) ? 13 : 13 - (i % 13) , deck.get().value());

            Card c = deck.pop();
            Assert.assertTrue("Spravna karta.", testCards.remove(c));
        }

        Assert.assertTrue("Testova sada musi byt prazdna", testCards.isEmpty());
    }
}