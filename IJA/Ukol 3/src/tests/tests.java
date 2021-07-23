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

public class tests {

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



    @Test
    public void testWorkingPack() {

        CardStack pack = factory.createWorkingPack();

        Card c1 = factory.createCard(Card.Color.DIAMONDS, 11);
        Card c2 = factory.createCard(Card.Color.DIAMONDS, 13);
        Card c3 = factory.createCard(Card.Color.HEARTS, 12);
        Card c4 = factory.createCard(Card.Color.CLUBS, 12);
        Card c5 = factory.createCard(Card.Color.SPADES, 11);
        Card c6 = factory.createCard(Card.Color.HEARTS, 11);
        Card c7 = factory.createCard(Card.Color.CLUBS, 10);

        Assert.assertEquals("Pracovni balicek je prazdny.", 0, pack.size());
        Assert.assertFalse("Na prazdny pracovni balicek lze vlozit pouze krale.", pack.put(c1));
        Assert.assertEquals("Pracovni balicek je prazdny.", 0, pack.size());

        Assert.assertTrue("Na prazdny pracovni balicek vkladame krale.", pack.put(c2));
        Assert.assertNotEquals("Pracovni balicek je prazdny.", 0, pack.size());

        Assert.assertFalse("Na cerveneho krale lze vlozit pouze cernou damu.", pack.put(c3));
        Assert.assertEquals("Pracovni balicek obsahuje 1 kartu.", 1, pack.size());


        Assert.assertTrue("Na cerveneho krale vkladame cernou damu.", pack.put(c4));
        Assert.assertEquals("Pracovni balicek obsahuje 2 karty.", 2, pack.size());

        Assert.assertFalse("Na cernou damu lze vlozit pouze cerveneho kluka.", pack.put(c5));
        Assert.assertEquals("Pracovni balicek obsahuje 2 karty.", 2, pack.size());
        Assert.assertTrue("Na cernou damu vkladame cerveneho kluka.", pack.put(c6));
        Assert.assertEquals("Pracovni balicek obsahuje 3 karty.", 3, pack.size());


        Assert.assertFalse("Na cernou damu lze vlozit pouze cerveneho kluka.", pack.put(c5));


        Assert.assertTrue("Na cerveneho kluka vkladame cernou 10.", pack.put(c7));
        Assert.assertEquals("Pracovni balicek obsahuje 4 karty.", 4, pack.size());

        //Assert.assertTrue("Pop 10.", pack.pop(factory.createCard(Card.Color.CLUBS, 10)));



        CardStack s = pack.pop(factory.createCard(Card.Color.CLUBS, 12));
        Assert.assertEquals("Pracovni balicek obsahuje 1 kartu.", 1, pack.size());
        Assert.assertEquals("Pocet odebranych karet je 3.", 3, s.size());


        Assert.assertEquals("Na vrcholu je H(11).", factory.createCard(Card.Color.CLUBS, 10), s.pop());
        Assert.assertEquals("Na vrcholu je H(11).", factory.createCard(Card.Color.HEARTS, 11), s.pop());
        Assert.assertEquals("Na vrcholu je C(12).", factory.createCard(Card.Color.CLUBS, 12), s.pop());
        Assert.assertEquals("Odebrany balicek je prazdny.", 0, s.size());
    }

    @Test
    public void testWorkingPack2() {

        CardStack pack1 = factory.createWorkingPack();
        CardStack pack2 = factory.createWorkingPack();
        CardStack pack3 = factory.createWorkingPack();

        pack1.put(factory.createCard(Card.Color.DIAMONDS, 13));
        pack1.put(factory.createCard(Card.Color.CLUBS, 12));
        pack1.put(factory.createCard(Card.Color.HEARTS, 11));

        CardStack s = pack1.pop(factory.createCard(Card.Color.CLUBS, 12));

        Assert.assertFalse("Nelze vlozit odebranou mnozinu (pracovni balicek je prazdny)", pack2.put(s));

        Assert.assertTrue("Vkladame cerveneho krale na prazdny balicek.", pack2.put(factory.createCard(Card.Color.HEARTS, 13)));

        Assert.assertTrue("Vkladame cerveneho krale na prazdny balicek.", pack3.put(factory.createCard(Card.Color.CLUBS, 13)));
        Assert.assertFalse("Nevkladame cernou damu.", pack3.put(factory.createCard(Card.Color.CLUBS, 12)));
        Assert.assertTrue("Vkladame cerveneho damu na krale.", pack3.put(factory.createCard(Card.Color.HEARTS, 12)));

        Assert.assertTrue("Vkladame odebranou mnozinu.", pack2.put(s));
        Assert.assertFalse("Vkladame odebranou mnozinu.", pack2.put(s));

        CardStack s2 = pack2.pop(factory.createCard(Card.Color.CLUBS, 12));

        Assert.assertEquals("Pracovni balicek c. 2 obsahuje 3 karty.", 1, pack2.size());

        Assert.assertFalse("Vkladame cerveneho krale na prazdny balicek.", pack1.put(factory.createCard(Card.Color.CLUBS, 13)));
        Assert.assertTrue("Nevkladame cernou damu.", pack2.put(factory.createCard(Card.Color.CLUBS, 12)));
        Assert.assertFalse("Vkladame cerveneho damu na krale.", pack3.put(factory.createCard(Card.Color.HEARTS, 12)));
    }
}