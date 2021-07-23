package src.model.board;

/**
 *  Trida s vyctovymi typy pro indexovani
 *
 *  @author   xmarti76
 *  @version  2017.5.6
 */
public class ID {
	public enum FoundationID {
		F1, F2, F3, F4;
		
		public int toInt ()  { return this.ordinal(); }
	};

	public enum PileID {
		P1, P2, P3, P4, P5, P6, P7;

		public int toInt ()  { return this.ordinal(); }
	};
}
