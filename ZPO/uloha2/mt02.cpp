#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>



/*  Funkce pro šedotónový obraz (0-255) spočítá konvoluci se zadaným jádrem o velikost 3x3 typu float (CV_32FC1).
	Krajní hodnoty výsledného obrazu ponechte 0.
	Implementujte ručně průchodem obrazem, pro každý pixel projděte jeho okolí a proveďte konvoluci s jádrem.
	Výslednou hodnotu je nutno před uložením do výsledného obrazu normalizovat.

	Povolené metody a funkce OpenCV pro realizaci úkolu jsou:
		Mat:: rows, cols, step(), size(), at<>(), zeros(), ones(), eye()
	
*/
void convolution( cv::Mat& gray, const cv::Mat& kernel, cv::Mat& dst )
{
	dst = cv::Mat::zeros( gray.size(), CV_32FC1 );

	if( kernel.rows != 3 || kernel.cols != 3 )
		return;

	/*  Working area - begin */

	// Normaliazcni koeficient
	const int norm =
		//                       y  x
		abs( kernel.at< float >( 0, 0 ) ) +
		abs( kernel.at< float >( 0, 1 ) ) +
		abs( kernel.at< float >( 0, 2 ) ) +
		abs( kernel.at< float >( 1, 0 ) ) +
		abs( kernel.at< float >( 1, 1 ) ) +
		abs( kernel.at< float >( 1, 2 ) ) +
		abs( kernel.at< float >( 2, 0 ) ) +
		abs( kernel.at< float >( 2, 1 ) ) +
		abs( kernel.at< float >( 2, 2 ) );

	// Preskocim okraje, proto 1 pro x a y
	for ( int y = 1 ; y < gray.rows - 1 ; ++y ) {
		for ( int x = 1 ; x < gray.cols - 1 ; ++x ) {
			// Obrazek:
			// -1,-1  0,-1  1,-1
			// -1,0   x,y   1,0
			// -1,1   0,1   1,1

			// Ale musim prehodit kvuli masce
			// a b c    i g h
			// d e f => f e d
			// g h i    c b a
			uchar neighbors[3][3] = {
				{
					gray.at< uchar >( y + 1, x + 1 ),
					gray.at< uchar >( y + 1, x ),
					gray.at< uchar >( y + 1, x - 1 ),
				},

				{
					gray.at< uchar >( y, x + 1 ),
					gray.at< uchar >( y, x ),
					gray.at< uchar >( y, x - 1 ),
				},
				
				{
					gray.at< uchar >( y - 1, x + 1 ),
					gray.at< uchar >( y - 1, x ),
					gray.at< uchar >( y - 1, x - 1 )
				}
			};

			// Vysledek rovna se suma vsech nasobku
			float value = 0.0;
			for ( auto mask_y = 0 ; mask_y < 3 ; ++mask_y ) {
				for ( auto mask_x = 0 ; mask_x < 3 ; ++mask_x ) {
					value += kernel.at< float >( mask_y, mask_x ) * neighbors[ mask_y ][ mask_x ];
				}
			}

			dst.at< float >( y, x ) = value / norm;	// Jeste musim normalizovat
		}
	}


	/*  Working area - end */
	
}

/*  
		Funkce provede geometrickou transformaci obrazu s využitím interpolace nejbližším sousedem.
		
		Vstupní transformace popisuje transformaci pixelů ze vstupního obrazu do výstupního.
		Využijeme inverzního postupu (viz přednáška), tedy vypočteme inverzní transformaci a
		pro každý bod výstupní matice budeme hledat jeho pozici ve vstupním obrazu a použijeme 
		interpolace nejbližším sousedem k nalezení hodnoty výstupního pixelu. 
		
*/
void geometricalTransform( const cv::Mat& src, cv::Mat& dst, const cv::Mat& transformation )
{
	// inverzní transformace
	cv::Mat T = transformation.inv();
   
	// výsledná matice/obraz
	dst = cv::Mat::zeros(src.size(), CV_8UC1 );
	
	/*
		Pro každý pixel výstupního obrazu
			1. najděte jeho polohu ve zdrojovém obrazu (pomocí připravené inverzní transformace v matici T)
				viz http://docs.opencv.org/modules/imgproc/doc/geometric_transformations.html?highlight=warpaffine#warpaffine
			2. zkontrolujte, není-li souřadnice mimo zdrojový obraz
			3. využijte interpolaci pomocí nejbližšího souseda k výpočtu výsledného jasu cílového pixelu (využijte funkci cvRound())
			
	Povolené metody a funkce OpenCV pro realizaci úkolu jsou:
		Mat:: rows, cols, step(), size(), at<>(), zeros(), ones(), eye(), cvRound()
			
	*/

	/*  Working area - begin */


	// Postupne iteruji vystupnim obrazkem
	for ( int y = 0 ; y < dst.rows ; ++y ) {
		for ( int x = 0 ; x < dst.cols ; ++x ) {
			dst.at< uchar >( y, x ) = src.at< uchar >( y, x );
			// Vynasobim matice
			float src_x =
				//           y  x
				T.at<float>( 0, 0 ) * x +
				T.at<float>( 0, 1 ) * y +
				T.at<float>( 0, 2 ); // * 1

			float src_y =
				//           y  x
				T.at<float>( 1, 0 ) * x +
				T.at<float>( 1, 1 ) * y +
				T.at<float>( 1, 2 ); // * 1

			// float W =
			// 	//           y  x
			// 	T.at<float>( 2, 0 ) * x +
			// 	T.at<float>( 2, 1 ) * y +
			// 	T.at<float>( 2, 2 ); // * 1

			// Pixel ukazuje mimo rozsah -> cerna
			if ( src_x < 0 || src_y < 0 || src_x >= src.cols || src_y >= src.rows ) {
				dst.at< uchar >( y, x ) = 0;
			}
			else {
				dst.at< uchar >( y, x ) = src.at< uchar >( cvRound( src_y ), cvRound( src_x ) );
			}
		}
	}

	/*  Working area - end */
	
	return;
}




//---------------------------------------------------------------------------
void checkDifferences( const cv::Mat test, const cv::Mat ref, std::string tag, bool save = false);
//---------------------------------------------------------------------------

//
// Examples of input parameters
//
// mt-02 image_path [rotation in degrees] [scale]


int main(int argc, char* argv[])
{
	std::string img_path = "";
	float s = 1.f;
	float r = 0.f;

	// check input parameters
	if( argc > 1 ) img_path = std::string( argv[1] );
	if( argc > 2 ) r = atof( argv[2] );
	if( argc > 3 ) s = atof( argv[3] );


	// load testing images
	cv::Mat src_rgb = cv::imread( img_path );

	// check testing images
	if( src_rgb.empty() ) {
		std::cout << "Failed to load image: " << img_path << std::endl;
		return -1;
	}

	// budeme pracovat s šedotónovým obrazem
	cv::Mat src_gray;
	cvtColor( src_rgb, src_gray, CV_BGR2GRAY );

	//---------------------------------------------------------------------------

	// konvoluce
	float ker[9] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };
	cv::Mat kernel( 3, 3, CV_32FC1, ker );
	cv::Mat conv_res, conv_ref;

	// naprogramované vaše řešení
	convolution( src_gray, kernel, conv_res );

	// referenční řešení
	cv::flip( kernel, kernel, -1 );
	cv::filter2D( src_gray, conv_ref, CV_32F, kernel );
	conv_ref *= 1.f/(cv::sum(abs(kernel)).val[0] + 0.000000001);
	// jelikož filter2D funkce počítá i hodnoty na okrajích výstupního obrazu (a my pro jednoduchost ne)
	// před srovnáním vymažeme krajní hodnoty obrazu
	cv::rectangle( conv_ref, cv::Point(0,0), cv::Point(conv_ref.cols-1,conv_ref.rows-1), cv::Scalar::all(0), 1 );

	//---------------------------------------------------------------------------

	// geometrická transformace
	
	// střed obrazu
	cv::Point2f c = cv::Point2f(0.5f*src_gray.cols,0.5f*src_gray.rows);
	
	// matice translace, rotace, změny měřítka a výsledná transformace 
	cv::Mat T = cv::Mat::eye(3,3,CV_32FC1);
	cv::Mat R = cv::Mat::eye(3,3,CV_32FC1);
	cv::Mat S = cv::Mat::eye(3,3,CV_32FC1);
	cv::Mat M = cv::Mat::eye(3,3,CV_32FC1);	
	
	/* 
		Nastavte koeficienty matice translace, rotace a změny měřítka 
		a transformační matice vynásobte ve správném pořadí tak,
	 	aby výsledný obraz byl orotován a přeškálován kolem svého středu.
	 	Hodnoty jsou v proměnných 'c', 'r' a 's'.
	 	Pozor: 
	 		- úhel je ve stupních (ne v radiánech), 
	 		- chceme rotovat proti směru hodinových ručiček (matice rotace je třeba před použitím invertovat),
	 		- násobení matic A*B*C je v c++ zleva, nutno použít závorky (A*(B*C)), chceme-li násobit zprava   
	 	 
	Povolené metody, makra a funkce OpenCV pro realizaci úkolu jsou:
		Mat:: rows, cols, at<>(), zeros(), ones(), eye(), inv(), cos, sin, CV_PI 	
	*/

	/*  Working area - begin */

	// Posunuti
	//           y  x
	T.at<float>( 0, 2 ) = -c.x * s;
	T.at<float>( 1, 2 ) = -c.y * s;

	// Posunuti zpet (iverzni matice)
	// Ulozim do M (uspora pameti)
	//           y  x
	M.at<float>( 0, 2 ) = -c.x;
	M.at<float>( 1, 2 ) = -c.y;
	M = M.inv();


	// Rotace
	double cos_r = cos( -r * CV_PI / 180.0 );
	double sin_r = sin( -r * CV_PI / 180.0 );
	//           y  x
	R.at<float>( 0, 0 ) =  cos_r;
	R.at<float>( 0, 1 ) = -sin_r;
	R.at<float>( 1, 0 ) =  sin_r;
	R.at<float>( 1, 1 ) =  cos_r;


	// Meritko
	S.at<float>( 0, 0 ) = s;
	S.at<float>( 1, 1 ) = s;


	// Operace je nunte provest v opacnem poradi
	// ! NUTNE cist zleva (meritko, posun, rotace, posun zpet)
	M = M * R * T * S;

	/*  Working area - end */

	// std::cout << M << std::endl;

	// výsledný obraz po transformaci
	cv::Mat tran_res, tran_ref;
	
	// vaše naprogramované vaše řešení
	geometricalTransform( src_gray, tran_res, M );
	cv::imwrite("tran_res.png", tran_res);

	// referenční řešení
	cv::Mat Mref = cv::getRotationMatrix2D( c, r, s );
	// warpAffine funkce interně invertuje transformační matici a interpoluje výstupní hodnoty pixelů ze vstupního obrazu	
	cv::warpAffine( src_gray, tran_ref, Mref, src_gray.size(), cv::INTER_NEAREST, cv::BORDER_CONSTANT, cv::Scalar::all(0) );
	cv::imwrite("tran_ref.png", tran_ref);
	//std::cout << Mref << std::endl;

	//---------------------------------------------------------------------------


	// vyhodnocení
	checkDifferences( conv_res, conv_ref, "convolution", true );
	checkDifferences( tran_res, tran_ref, "geometry", true );
	std::cout << std::endl;

	return 0;
}
//---------------------------------------------------------------------------




void checkDifferences( const cv::Mat test, const cv::Mat ref, std::string tag, bool save )
{
	double mav = 255., err = 255., nonzeros = 1000.;

	if( !test.empty() ) {
		cv::Mat diff;
		cv::absdiff( test, ref, diff );
		cv::minMaxLoc( diff, NULL, &mav );
		nonzeros = 1. * cv::countNonZero( diff ); // / (diff.rows*diff.cols);
		err = (nonzeros > 0 ? ( cv::sum(diff).val[0] / nonzeros ) : 0);

		if( save ) {
			diff *= 255;
			cv::imwrite( (tag+".png").c_str(), test );
			cv::imwrite( (tag+"_err.png").c_str(), diff );
		}
	}

	printf( "%s %.2f %.2f %.2f ", tag.c_str(), err, nonzeros, mav );

	return;
}

