/**
 * @file parallel_heat_solver.cpp
 * @author  xlogin00 <xlogin00@stud.fit.vutbr.cz>
 *
 * @brief   Course: ARC 2018/2019 - Project 2
 *
 * @date 06. 05. 2019 (21:15)
 */

#include <cmath>
#include <iomanip>
#include <sstream>
#include "omp.h"


#include "parallel_heat_solver.h"


using namespace std;


/// Je číslo sudé
#define IS_EVEN( number ) ( static_cast< int >( number ) % 2 == 0 )

/// Je číslo liché
#define IS_ODD( number ) ( static_cast< int >( number ) % 2 != 0 )

//============================================================================//
//                            *** BEGIN: NOTE ***
//
// Implement methods of your ParallelHeatSolver class here.
// Freely modify any existing code in ***THIS FILE*** as only stubs are provided 
// to allow code to compile.
//
//                             *** END: NOTE ***
//============================================================================//

ParallelHeatSolver::ParallelHeatSolver( SimulationProperties &simulationProps, MaterialProperties &materialProps )
	: BaseHeatSolver ( simulationProps, materialProps ),
	m_fileHandle( H5I_INVALID_HID, static_cast< void (*) ( hid_t ) >( nullptr ) )
	{

	MPI_Comm_size( MPI_COMM_WORLD, &m_size );
	MPI_Comm_rank( MPI_COMM_WORLD, &m_rank );


	// Rozeslu velikost hrany
	if ( m_rank == 0 ) {
		edgeSize = materialProps.GetEdgeSize();
	}

	err = MPI_Bcast(
		&edgeSize,    // Zdrojova data
		1,            // Jejich velikost
		MPI_SIZE_T,   // Datovy typ
		0,            // kdo odesila
		MPI_COMM_WORLD
	);


	// Spocitam velikost domeny
	if ( IS_EVEN( log2( m_size ) ) ) {
		// Je suda -> tile budou ctverce
		tile.setWidth( edgeSize / sqrt( m_size ) );
		tile.setHeight( tile.width( false ) );
	}
	else {
		tile.setWidth( edgeSize / sqrt( m_size >> 1 ) );
		tile.setHeight( tile.width( false ) >> 1 );	// ( << 1 ) == ( * 2 )
	}


	tile.setOffsetTop(  m_rank / getCols() );
	tile.setOffsetLeft( m_rank % getCols() );


	// Otevru soubor
	openFile( simulationProps.GetOutputFileName( "par" ) );


	// Pripravim si datove typy
	prepareTypes();

	// Rozešlu data
	distributeData( materialProps );

}



void ParallelHeatSolver::openFile( string fileName ) {
	// Paralelní I/O
	if ( m_simulationProperties.IsUseParallelIO() ) {
		if ( ! fileName.empty() ) {
			hid_t prop = H5Pcreate( H5P_FILE_ACCESS );
			H5Pset_fapl_mpio( prop, MPI_COMM_WORLD, MPI_INFO_NULL );

			m_fileHandle.Set( H5Fcreate( fileName.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, prop ), H5Fclose);

			H5Pclose( prop );

			if( m_fileHandle < 0 ) {
				throw runtime_error( "Cannot create output file" );
			}

			isOutputFile = true;
		}
	}
	else {
		if ( ! fileName.empty() ) {
			isOutputFile = true;
		
			if ( m_rank == 0 ) {
				m_fileHandle.Set( H5Fcreate( fileName.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT ), H5Fclose);

				if( m_fileHandle < 0 ) {
					throw runtime_error( "Cannot create output file" );
				}
			}
		}
	}
}



void ParallelHeatSolver::StoreDataIntoFile( hid_t fileHandle, size_t iteration, const float *data ) {
	hsize_t gridSize[] = { edgeSize, edgeSize };

	string groupName = "Timestep_" + to_string( static_cast< unsigned long long >( iteration / m_simulationProperties.GetDiskWriteIntensity() ) );

	AutoHandle< hid_t > groupHandle( H5Gcreate( fileHandle, groupName.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT ), H5Gclose );
	
	static string dataSetName( "Temperature" );
	AutoHandle<hid_t> dataTempHandle( H5Screate_simple( 2, gridSize, NULL ), H5Sclose );
	AutoHandle<hid_t> dataSetHandle( H5Dcreate( groupHandle, dataSetName.c_str(), H5T_NATIVE_FLOAT, dataTempHandle, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT), H5Dclose );



	static string attributeName("Time");
	AutoHandle<hid_t> dataTimeHandle( H5Screate( H5S_SCALAR ), H5Sclose );



	if ( m_simulationProperties.IsUseParallelIO() ) {
	 	// FIXME nejaka chyba
		hid_t dataSpace = H5Dget_space( dataSetHandle );
		
		hsize_t tileOffset[] = { tile.offsetTop() * tile.height( false ), tile.offsetLeft() * tile.width( false ), iteration };
		hsize_t tileSize[] = { tile.height( false ), tile.width( false ), 1 };
		H5Sselect_hyperslab( dataSpace, H5S_SELECT_SET, tileOffset, NULL, tileSize, NULL );
		
		hid_t writeProps = H5Pcreate( H5P_DATASET_XFER );
		H5Pset_dxpl_mpio( writeProps, H5FD_MPIO_COLLECTIVE );

		H5Dwrite( dataSetHandle, H5T_NATIVE_FLOAT, dataTempHandle, dataSpace, writeProps, data );


		H5Pclose(writeProps);
		H5Sclose(dataSpace);
	}
	else {
		H5Dwrite( dataSetHandle, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data );
	}


	if ( m_rank == ROOT ) {
		double snapshotTime = double( iteration );
        AutoHandle<hid_t> attributeHandle(H5Acreate2(groupHandle, attributeName.c_str(), H5T_IEEE_F64LE, dataTimeHandle, H5P_DEFAULT, H5P_DEFAULT), H5Aclose);

        H5Awrite( attributeHandle, H5T_IEEE_F64LE, &snapshotTime );
	}
}



void ParallelHeatSolver::prepareTypes() {
	// Puvodni rozmery pole
	int dimensions[2];
	
	// Rozmery podpole
	int subDimensions[2];

	// Odsazeni
	int offset[2] = { 0, 0 };



	// Jedna dlazdice s halo zonou
	dimensions[0] = tile.height( true );
	dimensions[1] = tile.width(  true );

	subDimensions[0] = tile.height( false );
	subDimensions[1] = tile.width(  false );
	

	MPI_Type_create_subarray( 2, dimensions, subDimensions, offset, MPI_ORDER_C, MPI_FLOAT, &tileT );
	MPI_Type_commit( &tileT );
	MPI_Type_create_resized( tileT, 0, sizeof( float ), &tileT);
	MPI_Type_commit( &tileT );
	


	// Jeden sloupec
	dimensions[0] = edgeSize;
	dimensions[1] = edgeSize;
	
	MPI_Type_create_subarray( 2, dimensions, subDimensions, offset, MPI_ORDER_C, MPI_FLOAT, &domainT );
	MPI_Type_commit( &domainT );
	MPI_Type_create_resized( domainT, 0, sizeof( float ), &domainT );
	MPI_Type_commit( &domainT );



	// Horizontalni halo zona
	dimensions[0] = tile.height( true );
	dimensions[1] = tile.width(  true );

	subDimensions[0] = tile.height( false );
	subDimensions[1] = HALO_SIZE;

	MPI_Type_create_subarray( 2, dimensions, subDimensions, offset, MPI_ORDER_C, MPI_FLOAT, &horizontalHaloT );
	MPI_Type_commit( &horizontalHaloT );
	MPI_Type_create_resized( horizontalHaloT, 0, sizeof( float ), &horizontalHaloT );
	MPI_Type_commit( &horizontalHaloT );



	// Vertikalni halo zona
	subDimensions[0] = HALO_SIZE;
	subDimensions[1] = tile.width( false );

	MPI_Type_create_subarray( 2, dimensions, subDimensions, offset, MPI_ORDER_C, MPI_FLOAT, &verticalHaloT );
	MPI_Type_commit( &verticalHaloT );
	MPI_Type_create_resized( verticalHaloT, 0, sizeof( float ), &verticalHaloT );
	MPI_Type_commit( &verticalHaloT );
}



void ParallelHeatSolver::distributeData( MaterialProperties &materialProps ) {
	float *initTempPtr     = nullptr;
	int   *domainMapPtr    = nullptr;
	float *domainParamsPtr = nullptr;

	newTempArray.resize( tile.size( true ) );
	oldTempArray.resize( tile.size( true ) );
	domainMap.resize   ( tile.size( true ) );
	domainParams.resize( tile.size( true ) );


	oldTempPtr = oldTempArray.data();
	newTempPtr = newTempArray.data();


	// Pouze 0 ma pristup k datum
	if ( m_rank == 0 ) {
		initTempPtr     = materialProps.GetInitTemp().data();
		domainMapPtr    = materialProps.GetDomainMap().data();
		domainParamsPtr = materialProps.GetDomainParams().data();


		// Pripravim si data k odeslani
		sendcounts = vector< int >( m_size, 1 );
		displs     = vector< int >( m_size, 0 );
	
		for ( int i = 0 ; i < getRows() ; i++ ) {
			for (int j = 0 ; j < getCols() ; j++ ) {
				displs[ i * getCols() + j ] = ( tile.size( false ) * getCols() * i ) + ( tile.width( false ) * j );
			}
		}
	}

	err = MPI_Scatterv(
		initTempPtr,        // Zdrojova data
		sendcounts.data(),  // Jejich velikost
		displs.data(),      // Pozice v datech
		domainT,            // Datovy typ
		&oldTempPtr[ 2 * tile.width( true ) + HALO_SIZE ],  // Cilova data
		1,                  // Jejich velikost
		tileT,              // Datovy typ
		ROOT,               // Kdo posila
		MPI_COMM_WORLD      // Komunikator
	);


	err = MPI_Scatterv(
		domainMapPtr,       // Zdrojova data
		sendcounts.data(),  // Jejich velikost
		displs.data(),      // Pozice v datech
		domainT,            // Datovy typ
		&domainMap[ 2 * tile.width( true ) + HALO_SIZE ],     // Cilova data
		1,                  // Jejich velikost
		tileT,              // Datovy typ
		ROOT,               // Kdo posila
		MPI_COMM_WORLD      // Komunikator
	);


	err = MPI_Scatterv(
		domainParamsPtr,    // Zdrojova data
		sendcounts.data(),  // Jejich velikost
		displs.data(),      // Pozice v datech
		domainT,            // Datovy typ
		&domainParams[ 2 * tile.width( true ) + HALO_SIZE ],  // Cilova data
		1,                  // Jejich velikost
		tileT,              // Datovy typ
		ROOT,               // Kdo posila
		MPI_COMM_WORLD      // Komunikator
	);


	sendHaloZones();
}



void ParallelHeatSolver::sendHaloZones() {
	size_t dest;
	size_t offset;


	// Odesila leva halo zona
	if( tile.offsetLeft() != getCols() - 1 ) {
		offset = HALO_SIZE * tile.width( true ) + tile.width( false );
		dest = tile.offsetTop() * getCols() + tile.offsetLeft() + 1;
		
		MPI_Send( &oldTempPtr[ offset ], 1, horizontalHaloT, dest, TEMP_TAG( LEFT ),  MPI_COMM_WORLD );
		MPI_Send( &domainParams[ offset ], 1, horizontalHaloT, dest, PARAM_TAG( LEFT ), MPI_COMM_WORLD );
		MPI_Send( &domainMap[    offset ], 1, horizontalHaloT, dest, MAP_TAG( LEFT ),   MPI_COMM_WORLD );
	}
	// Prijem od leve halo zony
	if( tile.offsetLeft() != 0 ) {
		offset = HALO_SIZE * tile.width( true );
		dest = tile.offsetTop() * getCols() + tile.offsetLeft() - 1;
	
		MPI_Recv( &oldTempPtr[ offset ], 1, horizontalHaloT, dest, TEMP_TAG( LEFT ),  MPI_COMM_WORLD, statuses );
		MPI_Recv( &domainParams[ offset ], 1, horizontalHaloT, dest, PARAM_TAG( LEFT ), MPI_COMM_WORLD, statuses );
		MPI_Recv( &domainMap[    offset ], 1, horizontalHaloT, dest, MAP_TAG( LEFT ),   MPI_COMM_WORLD, statuses );
	}

	// Odesila prava halo zona
	if( tile.offsetLeft() != 0 ) {
		offset = HALO_SIZE * tile.width( true ) + HALO_SIZE;
		dest = tile.offsetTop() * getCols() + tile.offsetLeft() - 1;
		
		MPI_Send( &oldTempPtr[ offset ], 1, horizontalHaloT, dest, TEMP_TAG( RIGHT ),  MPI_COMM_WORLD );
		MPI_Send( &domainParams[ offset ], 1, horizontalHaloT, dest, PARAM_TAG( RIGHT ), MPI_COMM_WORLD );
		MPI_Send( &domainMap[    offset ], 1, horizontalHaloT, dest, MAP_TAG( RIGHT ),   MPI_COMM_WORLD );
	}
	// Prijem od prave halo zony
	if( tile.offsetLeft() != getCols() - 1 ) {
		offset = HALO_SIZE * tile.width( true ) + tile.width( false ) + HALO_SIZE;
		dest = tile.offsetTop() * getCols() + tile.offsetLeft() + 1;
	
		MPI_Recv( &oldTempPtr[ offset ], 1, horizontalHaloT, dest, TEMP_TAG( RIGHT ),  MPI_COMM_WORLD, statuses );
		MPI_Recv( &domainParams[ offset ], 1, horizontalHaloT, dest, PARAM_TAG( RIGHT ), MPI_COMM_WORLD, statuses );
		MPI_Recv( &domainMap[    offset ], 1, horizontalHaloT, dest, MAP_TAG( RIGHT ),   MPI_COMM_WORLD, statuses );
	}

	
	// Odesila horni halo zona
	if( tile.offsetTop() != getRows() - 1 ) {
		offset = tile.height( false ) * tile.width( true ) + HALO_SIZE;
		dest = ( tile.offsetTop() + 1 ) * getCols() + tile.offsetLeft();
		
		MPI_Send( &oldTempPtr[ offset ], 1, verticalHaloT, dest, TEMP_TAG( TOP ),  MPI_COMM_WORLD );
		MPI_Send( &domainParams[ offset ], 1, verticalHaloT, dest, PARAM_TAG( TOP ), MPI_COMM_WORLD );
		MPI_Send( &domainMap[    offset ], 1, verticalHaloT, dest, MAP_TAG( TOP ),   MPI_COMM_WORLD );
	}
	// Prijem od horni halo zony
	if( tile.offsetTop() != 0 ) {
		offset = HALO_SIZE;
		dest = ( tile.offsetTop() - 1 ) * getCols() + tile.offsetLeft();
		
		MPI_Recv( &oldTempPtr[ offset ], 1, verticalHaloT, dest, TEMP_TAG( TOP ),  MPI_COMM_WORLD, statuses );
		MPI_Recv( &domainParams[ offset ], 1, verticalHaloT, dest, PARAM_TAG( TOP ), MPI_COMM_WORLD, statuses );
		MPI_Recv( &domainMap[    offset ], 1, verticalHaloT, dest, MAP_TAG( TOP ),   MPI_COMM_WORLD, statuses );
	}

	
	// Odesila spodni halo zona
	if( tile.offsetTop() != 0 ) {
		offset = HALO_SIZE * tile.width( true ) + HALO_SIZE;
		dest = ((tile.offsetTop() - 1) * getCols()) + (tile.offsetLeft());
		
		MPI_Send( &oldTempPtr[ offset ], 1, verticalHaloT, dest, TEMP_TAG( BOTTOM ),  MPI_COMM_WORLD );
		MPI_Send( &domainParams[ offset ], 1, verticalHaloT, dest, PARAM_TAG( BOTTOM ), MPI_COMM_WORLD );
		MPI_Send( &domainMap[    offset ], 1, verticalHaloT, dest, MAP_TAG( BOTTOM ),   MPI_COMM_WORLD );
	}
	// Prijem od spodni halo zony
	if( tile.offsetTop() != getRows() - 1 ) {
		offset = ( tile.height( false ) + HALO_SIZE ) * tile.width( true ) + HALO_SIZE;
		dest = ( tile.offsetTop() + 1 ) * getCols() + tile.offsetLeft();
		
		MPI_Recv( &oldTempPtr[ offset ], 1, verticalHaloT, dest, TEMP_TAG( BOTTOM ),  MPI_COMM_WORLD, statuses );
		MPI_Recv( &domainParams[ offset ], 1, verticalHaloT, dest, PARAM_TAG( BOTTOM ), MPI_COMM_WORLD, statuses );
		MPI_Recv( &domainMap[    offset ], 1, verticalHaloT, dest, MAP_TAG( BOTTOM ),   MPI_COMM_WORLD, statuses );
	}



	// Musim udelat kopii do druheho pomocneho pole
	if ( newTempPtr == newTempArray.data() ) {
		copy( oldTempArray.begin(), oldTempArray.end(), newTempArray.begin() );
	}
	else if ( newTempPtr == oldTempArray.data() ) {
		copy( newTempArray.begin(), newTempArray.end(), oldTempArray.begin() );
	}
}



void ParallelHeatSolver::IsendHaloTemps() {
	sendRequestID = 0;

	// Odesila leva halo zona
	if ( tile.offsetLeft() != getCols() - 1 ) {
		static const size_t offset = HALO_SIZE * tile.width( true ) + tile.width( false );
		static const size_t dest = tile.offsetTop() * getCols() + tile.offsetLeft() + 1;
		
		MPI_Isend( &newTempPtr[ offset ], 1, horizontalHaloT, dest, TEMP_TAG( LEFT ), MPI_COMM_WORLD, &sendRequests[ sendRequestID++ ] );
	}

	// Odesila prava halo zona
	if ( tile.offsetLeft() != 0 ) {
		static const size_t offset = HALO_SIZE * tile.width( true ) + HALO_SIZE;
		static const size_t dest = tile.offsetTop() * getCols() + tile.offsetLeft() - 1;
		
		MPI_Isend( &newTempPtr[ offset ], 1, horizontalHaloT, dest, TEMP_TAG( RIGHT ), MPI_COMM_WORLD, &sendRequests[ sendRequestID++ ] );
	}

	
	// Odesila horni halo zona
	if ( tile.offsetTop() != getRows() - 1 ) {
		static const size_t offset = tile.height( false ) * tile.width( true ) + HALO_SIZE;
		static const size_t dest = ( tile.offsetTop() + 1 ) * getCols() + tile.offsetLeft();
		
		MPI_Isend( &newTempPtr[ offset ], 1, verticalHaloT, dest, TEMP_TAG( TOP ), MPI_COMM_WORLD, &sendRequests[ sendRequestID++ ] );
	}

	
	// Odesila spodni halo zona
	if ( tile.offsetTop() != 0 ) {
		static const size_t offset = HALO_SIZE * tile.width( true ) + HALO_SIZE;
		static const size_t dest = ((tile.offsetTop() - 1) * getCols()) + (tile.offsetLeft());
		
		MPI_Isend( &newTempPtr[ offset ], 1, verticalHaloT, dest, TEMP_TAG( BOTTOM ), MPI_COMM_WORLD, &sendRequests[ sendRequestID++ ] );
	}
}




void ParallelHeatSolver::IrecvHaloTemps() {
	recvRequestID = 0;

	// Prijem od leve halo zony
	if( tile.offsetLeft() != 0 ) {
		static const size_t offset = HALO_SIZE * tile.width( true );
		static const size_t dest = tile.offsetTop() * getCols() + tile.offsetLeft() - 1;
	
		MPI_Irecv( &newTempPtr[ offset ], 1, horizontalHaloT, dest, TEMP_TAG( LEFT ), MPI_COMM_WORLD, &recvRequests[ recvRequestID++ ] );
	}

	// Prijem od prave halo zony
	if( tile.offsetLeft() != getCols() - 1 ) {
		static const size_t offset = HALO_SIZE * tile.width( true ) + tile.width( false ) + HALO_SIZE;
		static const size_t dest = tile.offsetTop() * getCols() + tile.offsetLeft() + 1;
	
		MPI_Irecv( &newTempPtr[ offset ], 1, horizontalHaloT, dest, TEMP_TAG( RIGHT ),  MPI_COMM_WORLD, &recvRequests[ recvRequestID++ ] );
	}

	// Prijem od horni halo zony
	if( tile.offsetTop() != 0 ) {
		static const size_t offset = HALO_SIZE;
		static const size_t dest = ( tile.offsetTop() - 1 ) * getCols() + tile.offsetLeft();
		
		MPI_Irecv( &newTempPtr[ offset ], 1, verticalHaloT, dest, TEMP_TAG( TOP ),  MPI_COMM_WORLD, &recvRequests[ recvRequestID++ ] );
	}

	// Prijem od spodni halo zony
	if( tile.offsetTop() != getRows() - 1 ) {
		static const size_t offset = ( tile.height( false ) + HALO_SIZE ) * tile.width( true ) + HALO_SIZE;
		static const size_t dest = ( tile.offsetTop() + 1 ) * getCols() + tile.offsetLeft();
		
		MPI_Irecv( &newTempPtr[ offset ], 1, verticalHaloT, dest, TEMP_TAG( BOTTOM ),  MPI_COMM_WORLD, &recvRequests[ recvRequestID++ ] );
	}
}



void ParallelHeatSolver::RunSolver( vector< float, AlignedAllocator< float > > &outResult ) {
	float middleColAvgTemp = 0.0;

	// Dlazdice se nachazi v prostrednim sloupci
	bool isMiddleCol = ( m_rank % getCols() ) == getCols() / 2;

	
	// Vytvorim novy komunikator pro prostredni sloupec
	MPI_Comm middleColComm;
	err = MPI_Comm_split( MPI_COMM_WORLD, isMiddleCol, m_rank, &middleColComm );
	int middleColRootRank = getCols() / 2;
	int isMiddleColRoot;
	MPI_Comm_rank( middleColComm, &isMiddleColRoot );
	isMiddleColRoot = !isMiddleColRoot;



	float *outResultPtr = nullptr;
	if ( m_rank == 0 ) {
		outResult.resize( edgeSize * edgeSize );
		outResultPtr = outResult.data();
	}


	// Spocitam rozsahy pro vypocet
	static const size_t FROM_Y = HALO_SIZE << 1;
	static const size_t TO_Y   = tile.height( false );
	static const size_t FROM_X = HALO_SIZE << 1;
	static const size_t TO_X   = tile.width( false );

	static const size_t HALO_FROM_X = tile.offsetLeft() > 0 ? HALO_SIZE : FROM_X;
	static const size_t HALO_TO_X   = tile.offsetLeft() < getCols() - 1 ? tile.width( true ) - HALO_SIZE : TO_X;


	// Spustim stopky
	double startTime;
	if ( m_rank == 0 ) {
		startTime = MPI_Wtime();
	}

	// #pragma omp parallel
	{
		for ( size_t iter = 0 ; iter < m_simulationProperties.GetNumIterations() ; ++iter ) {
			// Zahajim prijem novych dat
	    	// #pragma omp master
	    	IrecvHaloTemps();

	    	// Spocitam halo zony
	    	// Horni
	    	// ********
	    	// --------
	    	// --------
	    	if ( tile.offsetTop() > 0 ) {
				ComputeFromTo( HALO_FROM_X, HALO_TO_X, HALO_SIZE, FROM_Y );
	    	}

			// Dolni
	    	// --------
	    	// --------
	    	// ********
			if ( tile.offsetTop() < getRows() - 1 ) {
				ComputeFromTo( HALO_FROM_X, HALO_TO_X, TO_Y, TO_Y + HALO_SIZE );
			}

			// Prava
	    	// -------*
	    	// -------*
	    	// --------
			if ( tile.offsetLeft() < getCols() - 1 ) {
				ComputeFromTo( TO_X, TO_X + HALO_SIZE, FROM_Y, TO_Y );
			}


			// Leva
	    	// *-------
	    	// *-------
	    	// --------
			if ( tile.offsetLeft() > 0 ) {
				ComputeFromTo( FROM_X - HALO_SIZE, FROM_X, FROM_Y, TO_Y );
			}

			// #pragma omp barrier

			// Odeslu halo zony
			// #pragma omp master
			IsendHaloTemps();


			// Spocitam zbytek
			// 4. Compute new temperature for each point in the domain (except borders)
			// border temperatures should remain constant (plus our stencil is +/-2 points).
			ComputeFromTo( FROM_X, TO_X, FROM_Y, TO_Y );

			// Pockam na prijem/odeslani dat
			// #pragma omp master
	        MPI_Waitall( sendRequestID, sendRequests, statuses);

			// #pragma omp master
	        MPI_Waitall( recvRequestID, recvRequests, statuses);


	        // #pragma omp barrier


			// 6. Store the simulation state if appropriate (ie. every N-th iteration)
			// #pragma omp single nowait
			if ( isOutputFile && ( iter % m_simulationProperties.GetDiskWriteIntensity() ) == 0 ) {
				if ( m_simulationProperties.IsUseParallelIO() ) {
					StoreDataIntoFile( m_fileHandle, iter, outResultPtr );
				}
				else {
					collectData( &newTempPtr[ HALO_SIZE * tile.width( true ) + HALO_SIZE ], outResultPtr );
					
					if ( m_rank == ROOT ) {
						StoreDataIntoFile( m_fileHandle, iter, outResultPtr );
					}
				}
			}


			// 5. Compute average temperature in the middle column of the domain.
			// #pragma omp single nowait
			{
				if ( ShouldPrintProgress( iter ) || iter + 1 == m_simulationProperties.GetNumIterations() ) {
					if ( isMiddleCol ) {
						float tmpMiddleColAvgTemp = ComputeMiddleColAvgTemp( newTempPtr );
						err = MPI_Reduce( &tmpMiddleColAvgTemp, &middleColAvgTemp, 1, MPI_FLOAT, MPI_SUM, ROOT, middleColComm );

						if ( isMiddleColRoot ) {
							middleColAvgTemp /= getRows();
							err = MPI_Send( &middleColAvgTemp, 1, MPI_FLOAT, ROOT, MIDDLE_COL, MPI_COMM_WORLD );
						}
					}

					// 8. Print current progress (prints progress only every 10% of the simulation).
					if ( m_rank == ROOT ) {
						err = MPI_Recv( &middleColAvgTemp, 1, MPI_FLOAT, middleColRootRank, MIDDLE_COL, MPI_COMM_WORLD, statuses );
						PrintProgressReport( iter, middleColAvgTemp );
					}
				}
			}
			// #pragma omp barrier


			// 7. Swap source and destination buffers
			// #pragma omp single
			{
				swap( newTempPtr, oldTempPtr );
	
				err = MPI_Barrier( MPI_COMM_WORLD );
			}


			// #pragma omp single
			// {
			// 		collectData( &oldTempPtr[ HALO_SIZE * tile.width( true ) + HALO_SIZE ], outResultPtr );


		 //        std::stringstream out;

		 //        out << "\n\n------------------\n";
		 //        for (int y = 0; y < edgeSize; ++y) {
		 //            for (int x = 0; x < edgeSize; ++x) {
		 //                out << std::setw(6) << std::setprecision(4) << outResultPtr[ y * edgeSize + x ];
		 //            }
		 //            out << std::endl;
		 //        }

		 //        std::cout << out.str() << std::endl;

			// }
		}
	}

	if ( m_rank == 0 ) {
		double elapsedTime = MPI_Wtime() - startTime;
		// PrintFinalReport( elapsedTime, middleColAvgTemp, "par" );
	}





	float *fromPtr = &oldTempPtr[ HALO_SIZE * tile.width( true ) + HALO_SIZE ];
	// 10. Copy result over if necessary (even/odd number of buffer swaps).
	// Nefunguje, nejspis chyba 
	// if ( m_simulationProperties.GetNumIterations() & 1 ) {
	// 	fromPtr = &newTempPtr[ HALO_SIZE * tile.width( true ) + HALO_SIZE ];
	// }
	// else {
	// 	fromPtr = &oldTempPtr[ HALO_SIZE * tile.width( true ) + HALO_SIZE ];
	// }

	collectData( fromPtr, outResultPtr );
}



float ParallelHeatSolver::ComputeMiddleColAvgTemp( const float *data ) {
	float middleColAvgTemp = 0.0;

	for (
		size_t pointer = tile.width( true ) * HALO_SIZE + HALO_SIZE;
		pointer < tile.size( true ) - tile.width( true ) * HALO_SIZE;
		pointer += tile.width( true )
	) {
		middleColAvgTemp += data[ pointer ];
	}

	return middleColAvgTemp / float( tile.height( false ) );
}

