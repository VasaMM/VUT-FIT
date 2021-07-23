package src.tests;

import src.model.board.AbstractFactorySolitaire;
import src.model.board.FactoryKlondike;
import src.model.board.ID;
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
 * Základní testy funkčnosti
 *
 * @author xmarti76
 * @version 2017.5.6
 */
public class Test00 {

protected AbstractFactorySolitaire factory;

	@Before
		public void setUp() {
		factory = new FactoryKlondike();
	}

	@After
		public void tearDown() {
	}

	@Test
		public void testInitGame() {
			// Otestuji stock a waste
			int  stockSize = factory.getStockSize();
			Card wasteCard = factory.getWaste();

			Assert.assertTrue("Pocet karet v dobiracim balicku na zacatku musi byt 24.", stockSize == 24);
			Assert.assertTrue("V odhazovacim balicku nesmi byt zatim zadna karta.", wasteCard == null);
		

			// Otestuji foundations
			Card foundation1Card = factory.getFoundation( ID.FoundationID.F1 );
			Card foundation2Card = factory.getFoundation( ID.FoundationID.F2 );
			Card foundation3Card = factory.getFoundation( ID.FoundationID.F3 );
			Card foundation4Card = factory.getFoundation( ID.FoundationID.F4 );

			Assert.assertTrue("V odkladacim balicku nesmi byt zatim zadna karta.", foundation1Card == null);
			Assert.assertTrue("V odkladacim balicku nesmi byt zatim zadna karta.", foundation2Card == null);
			Assert.assertTrue("V odkladacim balicku nesmi byt zatim zadna karta.", foundation3Card == null);
			Assert.assertTrue("V odkladacim balicku nesmi byt zatim zadna karta.", foundation4Card == null);


			// Otestuji piles
			CardStack pile1Card = factory.getPile( ID.PileID.P1 );
			CardStack pile2Card = factory.getPile( ID.PileID.P2 );
			CardStack pile3Card = factory.getPile( ID.PileID.P3 );
			CardStack pile4Card = factory.getPile( ID.PileID.P4 );
			CardStack pile5Card = factory.getPile( ID.PileID.P5 );
			CardStack pile6Card = factory.getPile( ID.PileID.P6 );
			CardStack pile7Card = factory.getPile( ID.PileID.P7 );
			
			Assert.assertTrue("V pile 1 musi byt jen jedna karta.", pile1Card.size() == 1);
			Assert.assertTrue("V pile 1 musi byt jen jedna karta otocena nahoru.", pile1Card.get().isTurnedFaceUp() == true);
			
			Assert.assertTrue("V pile 2 musi byt dve karty.", pile2Card.size() == 2);
			Assert.assertTrue("V pile 2 musi byt horni karta otocena nahoru.", pile2Card.get().isTurnedFaceUp() == true);
			Assert.assertTrue("V pile 2 musi byt prvni karta otocena dolu.", pile2Card.get(0).isTurnedFaceUp() == false);
			Assert.assertTrue("V pile 2 musi byt druha karta otocena nahoru.", pile2Card.get(1).isTurnedFaceUp() == true);
			
			Assert.assertTrue("V pile 3 musi byt 3 karty.", pile3Card.size() == 3);
			Assert.assertTrue("V pile 4 musi byt 4 karty.", pile4Card.size() == 4);
			Assert.assertTrue("V pile 5 musi byt 5 karet.", pile5Card.size() == 5);
			Assert.assertTrue("V pile 6 musi byt 6 karet.", pile6Card.size() == 6);
			Assert.assertTrue("V pile 7 musi byt 7 karet.", pile7Card.size() == 7);
		}

	@Test
		public void testRevealGame() {
			for ( int i = 0 ; i < 24 ; ++i ) {
				factory.reveal();
				Assert.assertTrue("Ve waste musi byt nejaka karta.", factory.getWaste() != null);
			}
			
			factory.reveal();
			Assert.assertTrue("Ve waste nesmy byt zadna karta.", factory.getWaste() == null);
		}

	@Test
		public void testWin() {
			Assert.assertTrue("Jeste jsem nevyhral.", factory.win() == false);
		}

	@Test
		public void testSaveLoadGame() {
			Assert.assertTrue("Hra se nepodarila ulozit.", factory.saveGame( "test.out" ) == true);
			AbstractFactorySolitaire newFactory = new FactoryKlondike();
			Assert.assertTrue("Hra se nepodarila nacist.", newFactory.loadGame( "test.out" ) == true);
			Assert.assertTrue("Hra se nepodarila nacist.", newFactory.saveGame( "test2.out" ) == true);
			// NOTE dopsat testy???
		}
}
