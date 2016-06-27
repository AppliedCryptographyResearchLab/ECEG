/**
 * @file CECContext.cpp Definition of class CECContext.
 * @author Mohamed Grissa.
 * @date 03/15/2015.
 **/


#include <iostream>
#include <fstream>
#include <ctime>
#include "CTimer.hpp"
#include "miraclExtensions.hpp"
#include "CLambdaAlg.hpp"
#include "ECEG.hpp"
#include <cstdlib>
using namespace std;

#define  PKFILE "EC_PK.txt"
#define  SKFILE "EC_SK.txt"
#define  CFILE "ECEG_Cipher.txt"
#define  CFILE2 "ECEG_Cipher2.txt"
#define  PFILE "ECEG_Plain.txt"

int main()  {

    // Initialization curves and libraries miracle -> before the steering wheel must be no variables allocation big or ePoint !!!

	//double avrg = 0;
	//int b = 0;
	
	ifstream inputFile("./curves/ec224bits.ecs");
	ECEG::init(inputFile);
	inputFile.close();
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

