/**
 * @file parallel_heat_solver.h
 * @author  xmarti76 <xmarti76@vutbr.cz>
 *
 * @brief   Course: ARC 2018/2019 - Project 2
 *
 * @date 06. 05. 2019 (20:55)
 */


#ifndef PARALLEL_HEAT_SOLVER_H
#define PARALLEL_HEAT_SOLVER_H

#include <stdint.h>
#include <limits.h>

#include "base_heat_solver.h"


// https://stackoverflow.com/questions/40807833/sending-size-t-type-data-with-mpi
#if SIZE_MAX == UCHAR_MAX
   #define MPI_SIZE_T MPI_UNSIGNED_CHAR
#elif SIZE_MAX == USHRT_MAX
   #define MPI_SIZE_T MPI_UNSIGNED_SHORT
#elif SIZE_MAX == UINT_MAX
   #define MPI_SIZE_T MPI_UNSIGNED
#elif SIZE_MAX == ULONG_MAX
   #define MPI_SIZE_T MPI_UNSIGNED_LONG
#elif SIZE_MAX == ULLONG_MAX
   #define MPI_SIZE_T MPI_UNSIGNED_LONG_LONG
#else
   #error "what is happening here?"
#endif


/// Přiřadí tag k temperature array
#define TEMP_TAG( x )    ( x )

/// Přiřadí tag k params array
#define PARAM_TAG( x )   ( x ) | 0x10

/// Přiřadí tag k map array
#define MAP_TAG( x )     ( x ) | 0x20


using namespace std;


/// Velikost \c halo zóny
const uint HALO_SIZE = 2;


class Tile {
private:

	size_t _width      = 0;  ///< Šířka dlaždice
	size_t _height     = 0;  ///< Výška dlaždice
	size_t _offsetTop  = 0;  ///< Poloha dlaždice zvrchu
	size_t _offsetLeft = 0;  ///< Poloha dlaždice zleva

public:
	/**
	 * @brief Nastavi šířku
	 * @param w Šířka
	 */
	void setWidth( size_t w ) { _width = w; }


	/**
	 * @brief Nastavi výšku
	 * @param w Výška
	 */
	void setHeight( size_t h ) { _height = h; }


	/**
	 * @brief Nastavi pozici z vrchu
	 * @param w Pozice
	 */
	void setOffsetTop( size_t t ) { _offsetTop = t; }
	

	/**
	 * @brief Nastavi pozici z leva
	 * @param w Pozice
	 */
	void setOffsetLeft( size_t l ) { _offsetLeft = l; }
	

	/**
	 * @brief Získá šířku
	 * @param withHalo Včetně halo zóny
	 * @return Šířka
	 */
	size_t width( bool withHalo ) {
		if ( withHalo )
			return _width + ( HALO_SIZE << 1 );
		else
			return _width;
	}
	

	/**
	 * @brief Získá výšku
	 * @param withHalo Včetně halo zóny
	 * @return Výška
	 */
	size_t height(     bool withHalo ) {
		if ( withHalo )
			return _height + ( HALO_SIZE << 1 );
		else
			return _height;
	}
	

	/**
	 * @brief Získá pozici zvrchu
	 * @param withHalo Včetně halo zóny
	 * @return Pozice
	 */
	size_t offsetTop() { return _offsetTop; }
	


	/**
	 * @brief Získá pozici zleva
	 * @param withHalo Včetně halo zóny
	 * @return Pozice
	 */
	size_t offsetLeft() { return _offsetLeft; }


	/**
	 * @brief Získá rozměr dlaždice
	 * @param withHalo Včetně halo zóny
	 * @return Rozměr
	 */
	size_t size( bool withHalo ) {
		if ( withHalo )
			return width( true ) * height( true );
		else
			return width( false ) * height( false );
	}
};



/**
 * @brief The ParallelHeatSolver class implements parallel MPI based heat
 *        equation solver in 2D using 2D block grid decomposition.
 */
class ParallelHeatSolver : public BaseHeatSolver
{
	
public:
	/**
	 * @brief Constructor - Initializes the solver. This should include things like:
	 *        - Construct 2D grid of tiles.
	 *        - Create MPI datatypes used in the simulation.
	 *        - Open SEQUENTIAL or PARALLEL HDF5 file.
	 *        - Allocate data for local tile.
	 *        - Initialize persistent communications?
	 *
	 * @param simulationProps Parameters of simulation - passed into base class.
	 * @param materialProps   Parameters of material - passed into base class.
	 */
	ParallelHeatSolver( SimulationProperties &simulationProps, MaterialProperties &materialProps );
	virtual ~ParallelHeatSolver() {}

	/**
	 * @brief Run main simulation loop.
	 * @param outResult Output array which is to be filled with computed temperature values.
	 *                  The vector is pre-allocated and its size is given by dimensions
	 *                  of the input file (edgeSize*edgeSize).
	 * @note The vector is allocated (and should be used) *ONLY* by master process (rank 0 in MPI_COMM_WORLD)!
	 */
	virtual void RunSolver( vector< float, AlignedAllocator< float > > &outResult );

protected:
	/// Rank root procesu
	const int ROOT = 0;

	/// Jednotlivé tagy pro zasílání zpráv
	/// Lze obalit makry \c TEMP_TAG, \c PARAM_TAG a \c MAP_TAG
	enum Tags {
		LEFT,
		RIGHT,
		TOP,
		BOTTOM,
		MIDDLE_COL
	};

	/// Proměnná pro ukládání chyb
	int err;

	/// Process rank in global (MPI_COMM_WORLD) communicator.
	int m_rank;

	/// Total number of processes in MPI_COMM_WORLD.
	int m_size;

	/// Specifikace jedné dlaždice
	Tile tile;

	/// Ukazatel na soubor
	AutoHandle<hid_t> m_fileHandle;

	/// Datový typ popisující jednu dlaždici
	MPI_Datatype tileT;

	/// Datový typ popisující jednu dlaždici včetně \c halo zóny
	MPI_Datatype domainT;

	/// Datový typ popisující horizontální \c halo zónu
	MPI_Datatype horizontalHaloT;

	/// Datový typ popisující vertikální \c halo zónu
	MPI_Datatype verticalHaloT;

	/// Informace, zda existuje výstupní soubor
	bool isOutputFile = false;

	/// Vektory pro vektorové zasílání zpráv
	vector< int > sendcounts, displs;

	/// Velikost hrany
	size_t edgeSize = 0;

	/// Pomocná pole pro výpočet
	vector < float, AlignedAllocator< float > > newTempArray, oldTempArray;

	/// Pole popisující doménová data
	vector < int  , AlignedAllocator< int   > > domainMap;

	/// Pole s materiálními vlatnstmi
	vector < float, AlignedAllocator< float > > domainParams;

	/// Ukazatel na výsledky předchozí iterace
	float *oldTempPtr;

	/// Ukazatel na výsledky aktuální iterace
	float *newTempPtr;


	/**
	 * @brief Ziska celkový počet sloupců s dlaždicemi
	 * @return Počet sloupců
	 */
	size_t getCols() { return edgeSize / tile.width( false ); }


	/**
	 * @brief Ziska celkový počet řádků s dlaždicemi
	 * @return Počet řádků
	 */
	size_t getRows() { return edgeSize / tile.height( false ); }


	/**
	 * @brief Funkce spočítá průměrné teploty ve středním sloupci
	 * @param data Ukazatel na data
	 * @return Průměrná teplota
	 */
	float ComputeMiddleColAvgTemp( const float *data );


	/**
	 * @brief Otevře výstupní soubor
	 * 
	 * @param fileName Cesta k souboru, je-li prazdna, soubor se neotevře
	 */
	void openFile( string fileName );


	/// Uzavře výstupní soubor
	void closeFile() { if ( m_fileHandle != H5I_INVALID_HID ) H5Fclose( m_fileHandle ); }


	/// Připraví datové typy
	void prepareTypes();


	/**
	 * @brief Reozešle data
	 * @param materialProps Vstupní data
	 */
	void distributeData( MaterialProperties &materialProps );


	/// Odešle halo zóny
	/// Volat před hlavní smyčkou
	void sendHaloZones();

	/// Neblokující odeslání teplot z halo zón
	void IsendHaloTemps();

	/// Neblokující příjem teplot z halo zón
	void IrecvHaloTemps();

	/// Uloží data do souboru
    void StoreDataIntoFile( hid_t fileHandle, size_t iteration, const float *data );


	/// Status prijate zpravy
	MPI_Status statuses[4];

	/// Žádosti pro příjem zpráv
	MPI_Request recvRequests[4];

	/// Žádosti pro odeslání zpráv
	MPI_Request sendRequests[4];

	/// Počet odeslaných zpráv
	size_t sendRequestID;

	/// Počet přijatých zpráv
	size_t recvRequestID;


	/**
	 * @brief Spočítá teploty v zadaném obdélníku
	 * 
	 * @param from_x Rozsah souřadnic X (od)
	 * @param to_x Rozsah souřadnic X (do)
	 * @param y Rozsah souřadnic Y (od)
	 * @param to_y Rozsah souřadnic Y (do)
	 */
	void ComputeFromTo( size_t from_x, size_t to_x, size_t y, size_t to_y ) {
		// #pragma omp for
		for ( ; y < to_y ; ++y ) {
			for ( size_t x = from_x ; x < to_x ; ++x ) {
				ComputePoint( oldTempPtr, newTempPtr, domainParams.data(), domainMap.data(), y, x, tile.width( true ), m_simulationProperties.GetAirFlowRate(), m_materialProperties.GetCoolerTemp() );
			}
		}
	}


	/**
	 * @brief Sesbírá data od všech
	 * 
	 * @param from Ukazatel na čtená data
	 * @param to Ukazatel na ukládaná data
	 */
	void collectData( float *from, float *to ) {
		err = MPI_Gatherv(
			from,               // Odesilana data
			1,                  // Pocet polozek k odeslani
			tileT,              // Datovy typ
			to,                 // Cilova data
			sendcounts.data(),  // Jejich velikost
			displs.data(),      // Jejich poloha
			domainT,            // Datovy typ
			ROOT,               // kdo sbira
			MPI_COMM_WORLD
	);

	}
};

#endif // PARALLEL_HEAT_SOLVER_H
