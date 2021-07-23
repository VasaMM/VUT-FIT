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
public class test01 {

    protected AbstractFactorySolitaire factory;

    @Before
    public void setUp() {
        factory = new FactoryKlondike();
    }

    @After
    public void tearDown() {
    }

    @Test
    public void testCard() {
        Card c1 = factory.createCard(Card.Color.CLUBS, 0);
        Assert.assertNull("(Card.Color.CLUBS, 0) nelze", c1);

        Card c2 = factory.createCard(Card.Color.CLUBS, 1);
        Assert.assertFalse("Karta c2 nema byt otocena licem nahoru.", c2.isTurnedFaceUp());
        Assert.assertTrue("Otoceni karty c2.", c2.turnFaceUp());
        Assert.assertTrue("Karta c2 ma byt otocena licem nahoru.", c2.isTurnedFaceUp());
        Assert.assertFalse("Otoceni karty c2.", c2.turnFaceUp());

        Card c3 = factory.createCard(Card.Color.SPADES, 5);
        Card c4 = factory.createCard(Card.Color.CLUBS, 6);

        Assert.assertTrue("Stejny typ barvy u karet", c3.similarColorTo(c4));
        Assert.assertTrue("Stejny typ barvy u karet", c3.similarColorTo(c2));
        Assert.assertNotEquals("Shoda objektu.", c3, c4);

        Card c5 = factory.createCard(Card.Color.CLUBS, 6);
        Assert.assertEquals("Shoda objektu.", c4, c5);

        Assert.assertTrue("Rozdil hodnot karet.", c4.compareValue(c3) == 1);


    }
}