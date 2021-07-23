package src.model.board;

import src.model.board.ID;
import src.model.board.Undo;
import src.model.cards.Card;
import src.model.cards.CardDeck;
import src.model.cards.CardStack;

import java.util.ArrayList;
import java.io.FileWriter;
import java.io.FileReader;
import java.io.BufferedWriter;
import java.io.BufferedReader;
import java.util.regex.Pattern;
import java.util.regex.Matcher;


/**
 * Tovarna na vyrobu zakladnich instanci hry Solitaire.
 *
 * @author xmarti76
 * @version 2017.5.8
 */
public class FactoryKlondike extends AbstractFactorySolitaire {
	private static final long serialVersionUID = 1L;
	
	/** Dobiraci balicek (<tt>stock</tt>) */
	private CardStack   stock;
	/** Odkladaci balicek (<tt>waste</tt>) */
	private CardStack   waste;
	/** Vyresene karty (<tt>foundation</tt>) */
	private CardStack[] fonds;
	/** Docasne herni sloupce (<tt>pile</tt>) */
	private CardStack[] piles;
	
	/** Zasobnik provedenych tahu */
	protected ArrayList<Undo> undos;


	public FactoryKlondike () {
		// Inicializuji promenne
		CardDeck cardPack = new CardDeck( 52 );
		stock = new CardStack( -1 );
		waste = new CardStack( -1 );
		fonds = new CardStack[4];
		piles = new CardStack[7];
		undos = new ArrayList<>();

		// Nasypu karty do balicku
		for ( int i = 1; i <= 13; i++ ) { cardPack.put( new Card( Card.Color.CLUBS,    i ) ); }
		for ( int i = 1; i <= 13; i++ ) { cardPack.put( new Card( Card.Color.DIAMONDS, i ) ); }
		for ( int i = 1; i <= 13; i++ ) { cardPack.put( new Card( Card.Color.HEARTS,   i ) ); }
		for ( int i = 1; i <= 13; i++ ) { cardPack.put( new Card( Card.Color.SPADES,   i ) ); }

		cardPack.mix();

		// Naplnim piles
		for ( ID.PileID id : ID.PileID.values() ) {
			int i = id.toInt();
			piles[i] = new CardStack( -1 );

			for ( int j = 0 ; true ; ++j ) {
				// Pridam kartu z balicku
				piles[i].put( cardPack.pop() );
				// Zkontroluji zda nemam koncit a otocim horni kartu			
				if ( j == i ) {
					piles[i].get().turnFaceUp();
					break;
				}
			}
		}

		// Vytvorim foundations piles
		for ( ID.FoundationID id : ID.FoundationID.values() ) {
			fonds[ id.toInt() ] = new CardStack( -1 );
		}

		// Zbytek karet presunu do stock
		stock = new CardStack( -1 );
		while ( cardPack.size() > 0 ) {
			stock.put( cardPack.pop() );
		}
	}


	public int  getStockSize ()  { return stock.size(); }
	public Card getWaste     ()  { return waste.get(); }


	public Card getFoundation ( ID.FoundationID id ) {
		assert ( id != null );
		
		return fonds[ id.toInt() ].get();
	}


	public CardStack getPile ( ID.PileID id ) {
		assert ( id != null );

		return piles[ id.toInt() ];
	}


	public boolean reveal () {
		// Mam co otacet -> otocim a vratim true;
		if ( stock.size() != 0 ) {
			waste.put( stock.pop() );

			undos.add( new Undo( Undo.Operation.REVEAL ) );
			return true;
		}

		// Stock je prazdny, ale waste ne
		// Presunu waste do stock a vratim true
		if ( waste.size() != 0 ) {
			while ( waste.size() > 0 ) {
				stock.put( waste.pop() );
			}

			undos.add( new Undo( Undo.Operation.REVEAL ) );
			return true;
		}

		// Stock i waste je prazdny
		// Nic nedelam a vracim false
		return false;
	}


	private int moveCard ( Card from, CardStack to, int type ) {
		assert ( from != null );
		assert ( to   != null );
		assert ( type > -2 || type < 2 );

		CardStack takenCard = null;
		CardStack takenFrom = null;
		int       output    = -1;


		// Zkusim waste
		if ( from.equals( getWaste() ) ) {
			takenCard = new CardStack( waste.pop() );
			takenCard.get().turnFaceUp();
			takenFrom = waste;
		}

		// Zkusim foundations
		for ( ID.FoundationID id : ID.FoundationID.values() ) {
			if ( from.equals( getFoundation( id ) ) && fonds[ id.toInt() ] != to ) {	// Maybe FIXME fonds[ id.toInt() ] != to
				takenCard = new CardStack( fonds[ id.toInt() ].pop() );
				takenFrom  = fonds[ id.toInt() ];
			}
		}

		// Zkusim piles
		for ( ID.PileID id : ID.PileID.values() ) {
			if ( getPile( id ).find( from ) && piles[ id.toInt() ] != to ) {		// MAYBE FIXME
				takenCard = getPile( id ).pop( from );
				takenFrom = piles[ id.toInt() ];
			}
		}

		boolean moveIt = false;
		if ( takenCard != null && ! to.isEmpty() ) {
			output = 0;
			// Ani jedna z karet nesmi byt skryta
			if ( type == - 1) {
				moveIt = true;
			}
			else if ( takenCard.get().isTurnedFaceUp() && to.get().isTurnedFaceUp() ) {
				// Barva musi byt stejne, hodnota se musi lisit o - jedna
				if ( type == 1 ) {
					if ( ( from.color() == to.get().color() ) && ( to.get().compareValue( from ) == -1 ) ) {
						moveIt = true;
					}
				}
				// Barva musi byt rozdilne, hodnota se musi lisit o + jedna
				else if ( type == 0 ) {
					if ( ( ( ! from.similarColorTo( to.get() ) ) && ( to.get().compareValue( from ) == 1 ) ) ) {
						moveIt = true;
					}
				}
			}
		}
		// Presouvam na prazdny zasobnik
		// Pokud je to foundation, musi se jednat o eso
		// Pokud je to pile, musi to byt kral
		else if ( takenCard != null ) {
			output = 0;
			// Nesmim presouvat skrytou kartu
			if ( type == - 1) {
				moveIt = true;
			}
			else if ( takenCard.get().isTurnedFaceUp() ) {
				// Musi to byt eso
				if ( type == 1 ) {
					if ( from.value() == 1 ) {
						moveIt = true;
					}
				}
				// Musi to byt kral
				else if ( type == 0 ) {
					if ( from.value() == 13 ) {
						moveIt = true;
					}
				}
			}
		}

		if ( moveIt && to.put( takenCard ) ) {
			// Spoustim undo -> nepridat ho do seznamu
			if ( type != -1 )  { undos.add( new Undo( Undo.Operation.MOVE, from, takenFrom ) ); }
			output = takenCard.size();
			return takenCard.size();
		}

		// Nepovedlo se, vse vratim zpet
		if ( takenCard != null ) {
			takenFrom.put( takenCard );
		}
		
		return output;
	}

	public int move ( Card from, Card to ) {
		assert ( from != null );
		assert ( to   != null );

		if ( from.equals( to ) )  { return -2; }		

		// Zkusim foundations
		for ( ID.FoundationID id : ID.FoundationID.values() ) {
			if ( to.equals( getFoundation( id ) ) ) {
				return moveCard( from, fonds[ id.toInt() ], 1 );
			}
		}

		// Zkusim piles
		for ( ID.PileID id : ID.PileID.values() ) {
			if ( to.equals( getPile( id ).get() ) ) {
				return moveCard( from, piles[ id.toInt() ], 0 );
			}
		}

		return -3;
	}
	

	public int move ( Card from, ID.PileID to ) {
		assert ( from != null );
		assert ( to   != null );

		return moveCard( from, piles[ to.toInt() ], 0 );
	}
	

	public int move ( Card from, ID.FoundationID to ) {
		assert ( from != null );
		assert ( to   != null );

		return moveCard( from, fonds[ to.toInt() ], 1 );
	}

	
	public boolean turn ( Card card ) {
		assert ( card != null );

		// Otocit lze pouze kartu, ktera je na konci pile a jeste neni otocena
		// U ostatnich je to bud proti pravidlum, nebo to nema smysl
		// Proto projdu jen tech max 7 karet
		for ( ID.PileID id : ID.PileID.values() ) {
			if ( card.equals( piles[ id.toInt() ].get() ) ) {
				if ( piles[ id.toInt() ].get().turnFaceUp() ) {
					undos.add( new Undo( Undo.Operation.TURN, card, piles[ id.toInt() ] ) );
					return true;
				}
			}
		}

		return false;
	}


	public boolean turn ( ID.PileID id ) {
		assert ( id != null );

		if ( piles[ id.toInt() ].get().turnFaceUp() ) {
			undos.add( new Undo( Undo.Operation.TURN, piles[ id.toInt() ].get(), piles[ id.toInt() ] ) );
			return true;
		}

		return false;
	}


	// public boolean win ()  { return ( fonds[1].size() == 13 && fonds[2].size() == 13 && fonds[3].size() == 13 && fonds[4].size() == 13 ); }
	public boolean win () {
		return (
			getFoundation(ID.FoundationID.F1) != null && getFoundation(ID.FoundationID.F1).value() == 13 &&
			getFoundation(ID.FoundationID.F2) != null && getFoundation(ID.FoundationID.F2).value() == 13 &&
			getFoundation(ID.FoundationID.F3) != null && getFoundation(ID.FoundationID.F3).value() == 13 &&
			getFoundation(ID.FoundationID.F4) != null && getFoundation(ID.FoundationID.F4).value() == 13
		);
	}


	public int undo (){
		// Card output = deck.get( deck.size() - 1 );
		// deck.remove( deck.size() - 1 );

		// Otestuji, zda mam jeste nejake operace, co muzu vratit
		if ( undos.isEmpty() )  { return 0; }
		
		Undo operation = undos.get( undos.size() - 1 );
		if ( operation != null ) {
			switch( operation.getOperation() ) {
				case REVEAL:
					// Mam co vracet
					if ( waste.size() != 0 ) {
						stock.put( waste.pop() );

						undos.remove( undos.size() - 1 );
						return 1;
					}

					// Waste je prazdny, ale stock ne
					// Musim prevezt stock zpet do waste 
					if ( stock.size() != 0 ) {
						while ( stock.size() > 0 ) {
							waste.put( stock.pop() );
						}

						undos.remove( undos.size() - 1 );
						return 1;
					}

					// Stock i waste je prazdny => neco je spatne
					return -2;					

				case TURN:
					operation.getFrom().get().turnFaceDown();
					undos.remove( undos.size() - 1 );
					return 1;

				case MOVE:
					undos.remove( undos.size() - 1 );
					// Zkusim presunout
					int ret = moveCard( operation.getCard(), operation.getFrom(), -1 );
					if ( ret > 0 )  { return ret; }

					// Nekde je chyba...
					return -1;
			}

		}

		return -3;
	}


	private String canMoveIt ( Card card, String from ) {
		assert ( card != null );

		boolean toEmptyFoundation = false;
		boolean toEmptyPile       = false;
		
		String output = "";

		// Projdu foundations
		for ( ID.FoundationID id : ID.FoundationID.values() ) {
			// Foundation neni empty
			if ( getFoundation( id ) != null ) {
				// Stejna barva, rozdil hodnot o -1
				if ( card.color() == getFoundation( id ).color() && card.compareValue( getFoundation( id ) ) == 1 ) {
					output += "You can move " + card + " from " + from + " to foundation " + ( id.toInt() + 1 ) + "\n";
				}
			}
			// Zkusim presunout eso
			else if ( ( ! toEmptyFoundation ) && card.value() == 1 ) {
				output += "You can move " + card + " from " + from + " to foundation " + ( id.toInt() + 1 ) + "\n";
				toEmptyFoundation = true;
			}

		}
		// Projdu piles
		for ( ID.PileID id : ID.PileID.values() ) {
			// Pile neni prazdny
			if ( getPile( id ).size() > 0 ) {
					// Karta nesmi byt zakryta
					if ( getPile( id ).get().isTurnedFaceUp() ) {				
						// Rozdilna barva (cerna, cervena), rozdil hodnot o +1
						if ( ( ! card.similarColorTo( getPile( id ).get() ) ) && card.compareValue( getPile( id ).get() ) == -1 ) {
							output += "You can move " + card + " from " + from + " to " + getPile( id ).get() + " in pile " + ( id.toInt() + 1 ) + "\n";
						}
					}
			}
			// Pile je prazdny, zkusim presunout krale
			else if ( ( ! toEmptyPile ) && card.value() == 13 ) {
				output += "You can move " + card + " from " + from + " to foundation " + ( id.toInt() + 1 ) + "\n";
				toEmptyPile = true;
			}
		}

		return output;
	}


	public String help (){
		String output = "";

		// Zkusim nekam presunout waste
		if ( getWaste() != null ) {
			output += canMoveIt( getWaste(), "waste" );
		}

		// Projdu piles
		for ( ID.PileID id : ID.PileID.values() ) {
			// Pile neni prazdny
			if ( getPile( id ).size() > 0 ) {
				// Projdu vsechny otocene karty a pokusim se je nekam presunout
				for ( int i = 0 ; i < getPile( id ).size() ; ++i ) {
					if ( getPile( id ).get( i ).isTurnedFaceUp() ) {
						output += canMoveIt( getPile( id ).get( i ), "P" + ( id.toInt() + 1 ) );
					}
				}
				// Zjistim, zda nahodou nelze otocit kartu
				if ( ! getPile( id ).get().isTurnedFaceUp() ) {
					output += "You can turn card on P"  + ( id.toInt() + 1 ) + "\n";
				}
			}
		}

		if ( output.equals( "" ) ) {
			if ( getStockSize() > 0 ) {
				return "Try turning cart from top of stock.\n";
			}
			else {
				return "There aren't any posible move...\n";
			}
		} 

		return output;
	}


	// Prevede hodnotu karty ze stringu na int
	public int valueToInt ( String value ) {
		switch ( value ) {
			case "A":  return 1;
			case "2":  return 2;
			case "3":  return 3;
			case "4":  return 4;
			case "5":  return 5;
			case "6":  return 6;
			case "7":  return 7;
			case "8":  return 8;
			case "9":  return 9;
			case "10": return 10;
			case "J":  return 11;
			case "Q":  return 12;
			case "K":  return 13;
		}

		throw new IllegalArgumentException( "Invalid card value!" );
	}
	

	// Prevede barvu karty ze Stringu na Enum
	private Card.Color colorToEnum ( String color ) {
		switch ( color ) {
			case "H": return Card.Color.HEARTS;
			case "D": return Card.Color.DIAMONDS;
			case "S": return Card.Color.SPADES;
			case "C": return Card.Color.CLUBS;
		}
		
		throw new IllegalArgumentException( "Invalid card color!" );
	}
	

	// Prevede jeden radek souboru na CardStack
	private CardStack parseLine ( String line ) {
		CardStack output = new CardStack( -1 );

		Pattern cardPattern = Pattern.compile( "([A23456789JQK]|10)\\(([HDSC])\\)(\\!)?|(empty)" );
		Matcher cardMatcher = cardPattern.matcher( line );


		while ( cardMatcher.find() ) {
			// Stack je prazdny
			if ( cardMatcher.group(4) != null )  { return output; }

			int        value = valueToInt(  cardMatcher.group(1) );
			Card.Color color = colorToEnum( cardMatcher.group(2) );
			boolean    show  =            ( cardMatcher.group(3) == null );

			Card card = new Card( color, value );
			if ( show )  { card.turnFaceUp(); }

			output.put( card );
		}
		
		return output;
	}


	public boolean loadGame ( String path ) {
		assert ( path != null );
		
		// Docasne prazdne promenne
		// Neprijdu o data v pripade chyby
		CardStack   tmpStock = null;
		CardStack   tmpWaste = null;
		CardStack[] tmpFonds = new CardStack[4];
		CardStack[] tmpPiles = new CardStack[7];

		Pattern stockPattern = Pattern.compile( "^S:((([A23456789JQK]|10)\\([HDSC]\\)\\!?(, )){0,23}(([A23456789JQK]|10)\\([HDSC]\\)\\!?)|empty)$" );
		Pattern wastePattern = Pattern.compile( "^W:((([A23456789JQK]|10)\\([HDSC]\\)\\!?(, )){0,23}(([A23456789JQK]|10)\\([HDSC]\\)\\!?)|empty)$" );
		Pattern fondsPattern = Pattern.compile( "^F([1-4]):((([A23456789JQK]|10)\\([HDSC]\\)\\!?(, )){0,12}(([A23456789JQK]|10)\\([HDSC]\\)\\!?)|empty)$" );
		Pattern pilesPattern = Pattern.compile( "^P([1-7]):((([A23456789JQK]|10)\\([HDSC]\\)\\!?(, )){0,12}(([A23456789JQK]|10)\\([HDSC]\\)\\!?)|empty)$" );

		try ( BufferedReader input = new BufferedReader( new FileReader( path ) ) ) {
			String line;
			while ( ( line = input.readLine() ) != null ) {
				// Zjistim jako radek prave ctu
				Matcher stockMatcher = stockPattern.matcher( line );
				Matcher wasteMatcher = wastePattern.matcher( line );
				Matcher fondsMatcher = fondsPattern.matcher( line );
				Matcher pilesMatcher = pilesPattern.matcher( line );

				if ( stockMatcher.find() ) {
					if ( tmpStock != null )  { return false; }

					tmpStock = parseLine( line );
					continue;
				}
				if ( wasteMatcher.find() ) {
					if ( tmpWaste != null )  { return false; }

					tmpWaste = parseLine( line );
					continue;
				}
				if ( fondsMatcher.find() ) {
					int pos = Integer.parseInt( fondsMatcher.group(1) ) - 1;
					
					if ( tmpFonds[ pos ] != null )  { return false; }
					tmpFonds[ pos ] = parseLine( line );

					continue;
				}
				if ( pilesMatcher.find() ) {
					int pos = Integer.parseInt( pilesMatcher.group(1) ) - 1;
					
					if ( tmpPiles[ pos ] != null )  { return false; }
					tmpPiles[ pos ] = parseLine( line );

					continue;
				}
			}
		}
		catch ( Exception e ) {
			return false;
		}

		// Vse se povedlo, muzu nahrat data
		stock = tmpStock;
		waste = tmpWaste;
		fonds = tmpFonds;
		piles = tmpPiles;

		return true;
	}


	public boolean saveGame ( String path ){
		assert ( path != null );

		try ( BufferedWriter output = new BufferedWriter( new FileWriter( path ) ) ) {
			output.write( "S:" );
			output.write(  stock.toString() );
			output.newLine();

			output.write( "W:" );
			output.write(  waste.toString() );
			output.newLine();

			for ( ID.FoundationID id : ID.FoundationID.values() ) {
				output.write( "F" + ( id.toInt() + 1 ) + ":" );
				output.write( fonds[ id.toInt() ].toString() );
				output.newLine();
			}

			for ( ID.PileID id : ID.PileID.values() ) {
				output.write( "P" + ( id.toInt() + 1 ) + ":" );
				output.write( piles[ id.toInt() ].toString() );
				output.newLine();
			}

			output.flush();
		}
		catch ( Exception e ) {
			return false;
		}

		return true;
	}
}
