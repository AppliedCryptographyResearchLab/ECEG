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

	// Loads the elliptic curve parameters
	ifstream inputFile("./curves/ec224bits.ecs");
	ECEG::init(inputFile);
	inputFile.close();
	
	// Generates the crypto keys 
	ECEG::keyGen();
	
	cipher ciphers;

	// Loads the generated public key
	ifstream publicKey(PKFILE);
	publicKey>>ECEG::pkx>>ECEG::pky;
	publicKey.close();
	publicKey.clear();
	
	// Loads the generated secert key
	ifstream secretKey(SKFILE);
	secretKey >> ECEG::sk;
	secretKey.close();
	secretKey.clear();
	
	// Loads the plaintext to be encrypted
	ifstream plainText(PFILE);
	
	// Encryption of the plaintext
	ciphers = ECEG::Enc(plainText);
	plainText.close();
	plainText.clear();
	
	// Loads the ciphertext to be decrypted
	ifstream cipherText(CFILE);
	cipherText >> ECEG::C1x >> ECEG::C1y >> ECEG::C2x >> ECEG::C2y;
	cipherText.close();
	cipherText.clear();
	
	// Decription of the ciphertext
	ECEG::Dec();
	
	// Frees memory
	ECEG::free();

    	return 0;
}

