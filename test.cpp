/**
 * @file test.cpp to show how to use the Elliptic Curve El Gamal ECEG class.
 * @author Mohamed Grissa.
 * @date 03/15/2015.
 **/


#include <iostream>
#include <fstream>
#include <ctime>
#include "miraclExtensions.hpp"
#include "CLambdaAlg.hpp"
#include "ECEG.hpp"
#include <cstdlib>
using namespace std;

//
#define  PKFILE "EC_PK.txt"
#define  SKFILE "EC_SK.txt"
#define  CFILE "ECEG_Cipher.txt"
#define  PFILE "ECEG_Plain.txt"

int main()  {

	// Load the elliptic curve parameters
	ifstream inputFile("./curves/ec224bits.ecs");
	ECEG::init(inputFile);
	inputFile.close();
	
	// Generate the crypto keys for 
	ECEG::keyGen();
	
	
	
	cipher ciphers;

	ifstream publicKey(PKFILE);
	publicKey>>ECEG::pkx>>ECEG::pky;
	publicKey.close();
	publicKey.clear();
	//epoint_set(ECEG::pkx,ECEG::pkx,ECEG::pky,ECEG::Q);
	ifstream secretKey(SKFILE);
	secretKey >> ECEG::sk;
	secretKey.close();
	secretKey.clear();
	
	ifstream plainText(PFILE);


		ciphers = ECEG::Enc(plainText);
		//plainText.close();
		
		ifstream cipherText(CFILE);
		//ifstream cipherText2(CFILE2);
		//ECEG::addCiphers(cipherText,cipherText2);
		cipherText >> ECEG::C1x >> ECEG::C1y >> ECEG::C2x >> ECEG::C2y;
		//cout << "bzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz" << endl << ECEG::C1x << endl << ECEG::C1y << endl << ECEG::C2x << endl << ECEG::C2y << endl;
		//CTimer stopwatch;
		ECEG::Dec();
		//avrg = avrg + stopwatch.getTime();
		//cout<<"Time" << stopwatch.getTime()<<endl;

		cipherText.close();
		cipherText.clear();
		

	plainText.close();
	plainText.clear();
	/*avrg = avrg/b;
	cout << avrg;*/

	//ECEG::free();


	

    	return 0;
}

