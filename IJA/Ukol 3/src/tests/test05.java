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
public class test05
{

    protected AbstractFactorySolitaire factory;

    @Before
    public void setUp() {
        factory = new FactoryKlondike();
    }

    @After
    public void tearDown() {
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
