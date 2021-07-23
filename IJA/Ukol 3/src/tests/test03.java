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
public class test03 {

    protected AbstractFactorySolitaire factory;

    @Before
    public void setUp() {
        factory = new FactoryKlondike();
    }

    @After
    public void tearDown() {
    }

    @Test
    public void testTargetPack() {

        CardDeck packClubs = factory.createTargetPack(Card.Color.CLUBS);

        Card c1 = factory.createCard(Card.Color.HEARTS, 11);
        Card c2 = factory.createCard(Card.Color.HEARTS, 1);
        Card c3 = factory.createCard(Card.Color.CLUBS, 11);
        Card c4 = factory.createCard(Card.Color.CLUBS, 1);
        Card c5 = factory.createCard(Card.Color.CLUBS, 2);

        Assert.assertEquals("Pocet karet v baliku je 0.", 0, packClubs.size());
        Assert.assertFalse("Nelze vlozit kartu", packClubs.put(c1));
        Assert.assertEquals("Pocet karet v baliku je 0.", 0, packClubs.size());
        Assert.assertFalse("Nelze vlozit kartu", packClubs.put(c2));
        Assert.assertEquals("Pocet karet v baliku je 0.", 0, packClubs.size());
        Assert.assertFalse("Nelze vlozit kartu", packClubs.put(c3));
        Assert.assertEquals("Pocet karet v baliku je 0.", 0, packClubs.size());


        Assert.assertTrue("Lze vlozit kartu", packClubs.put(c4));
        Assert.assertEquals("Pocet karet v baliku je 1.", 1, packClubs.size());
        Assert.assertFalse("Pocet karet v baliku je 0.", packClubs.isEmpty());

        Assert.assertFalse("Nelze vlozit kartu", packClubs.put(c3));
        Assert.assertEquals("Pocet karet v baliku je 1.", 1, packClubs.size());
        Assert.assertTrue("Lze vlozit kartu", packClubs.put(c5));
        Assert.assertEquals("Pocet karet v baliku je 2.", 2, packClubs.size());



        Card c55 = factory.createCard(Card.Color.CLUBS, 2);
        Card c44 = factory.createCard(Card.Color.CLUBS, 1);

        Assert.assertEquals("Na vrcholu je karta c55", c55, packClubs.get());
        Assert.assertEquals("Pocet karet v baliku je 2.", 2, packClubs.size());
        Assert.assertEquals("Vyber karty z vrcholu", c55, packClubs.pop());

        Assert.assertEquals("Pocet karet v baliku je 1.", 1, packClubs.size());
        Assert.assertEquals("Na vrcholu je karta c44", c44, packClubs.get());
        Assert.assertEquals("Pocet karet v baliku je 1.", 1, packClubs.size());
        Assert.assertEquals("Vyber karty z vrcholu", c44, packClubs.pop());

        CardDeck packHearts = factory.createTargetPack(Card.Color.HEARTS);


        Assert.assertFalse("Nelze vlozit kartu", packHearts.put(c1));
        Assert.assertEquals("Pocet karet v baliku je 0.", 0, packHearts.size());

        Assert.assertTrue("Nelze vlozit kartu", packHearts.put(c2));
        Assert.assertFalse("Nelze vlozit kartu", packHearts.put(c1));


        Assert.assertNotEquals("Pocet karet v baliku je 0.", 2, packHearts.size());


    }
}