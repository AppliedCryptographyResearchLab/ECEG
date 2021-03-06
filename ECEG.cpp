/**
 * @file ECEG.cpp Definition of class ECEG.
 * @author Mohamed Grissa.
 * @date 03/15/2015.
 **/
#include "ECEG.hpp"
#include "big.h"
using namespace std;

#define  PKFILE "EC_PK.txt"
#define  SKFILE "EC_SK.txt"
#define  CFILE "ECEG_Cipher.txt"
#define  PFILE "ECEG_Plain.txt"
#define  MSG_SIZE 8
int My,C1y,C2y,CA1y,CA2y,CB1y,CB2y;
big a,Mx,C1x,C2x,CA1x,CA2x,CB1x,CB2x,m,r,x1,x2,r_crt,d,diff;
ofstream fout;
miracl *mip = mirsys(500,0);

big ECEG::pkx;
int ECEG::pky; 
big ECEG::sk;
cipher ECEG::ciph;
bool ECEG::initialized = false;
big ECEG::ecA = NULL;
big ECEG::ecB = NULL;
big ECEG::ecP = NULL;
big ECEG::ord = NULL;
big ECEG::C1x = NULL;	
big ECEG::C2x = NULL;
int ECEG::C1y;
int ECEG::C2y;
epoint * ECEG::G = NULL;
epoint * ECEG::Q = NULL;
epoint * ECEG::M = NULL;
epoint * ECEG::C1 = NULL;
epoint * ECEG::C2 = NULL;
epoint * ECEG::CA1 = NULL;
epoint * ECEG::CA2 = NULL;
epoint * ECEG::CB1 = NULL;
epoint * ECEG::CB2 = NULL;


void ECEG::init(std::istream &ecSource)
{

	//cout << "----------------------------------Initialization----------------------------------------" << endl;
	irand( (unsigned int)time(0) );
	if ( ecSource.fail() )      {
        std::cerr << "Error: 'init': Failed to load curves from file!" << std::endl;
        exit(1);
	} else  {
        G = epoint_init();  
	Q = epoint_init();
	M = epoint_init();
	C1 = epoint_init();
	C2 = epoint_init();
	CA1 = epoint_init();
	CA2 = epoint_init();
	CB1 = epoint_init();
	CB2 = epoint_init();

	a=mirvar(0);
        sk=mirvar(0);
        ecA = mirvar(0);
        ecB = mirvar(0);
        ecP = mirvar(0);
        ord = mirvar(0);
        pkx = mirvar(0);
        r = mirvar(0);
	m = mirvar(0);
	ciph.aa = mirvar(0);
	//ciph.bb = mirvar(0);
	ciph.cc = mirvar(0);
	//ciph.dd = mirvar(0);	
	


	m=mirvar(0);
	Mx = mirvar(0);
	C1x = mirvar(0);
	C2x = mirvar(0);
	CA1x = mirvar(0);
	CA2x = mirvar(0);
	CB1x = mirvar(0);
	CB2x = mirvar(0);
	int bits;
	// Generator G coordintes
	big Gx = mirvar(0);
	big Gy = mirvar(0);

	x1 = mirvar(0);
	x2 = mirvar(0);


	mip->IOBASE=10;
	ecSource >> bits;

	mip->IOBASE=16;
	ecSource >> ecP >> ecA >> ecB >> ord >> Gx >> Gy;
	//ecSource >> ecP >> ecB >> ord >> Gy;
        //convert(-3,a);

	initialized = true;
	
        cout<<bits<<endl<<a<<endl<<Gx<<endl<<ord<<endl;
        ecurve_init(ecA, ecB, ecP, MR_PROJECTIVE);  /* Use PROJECTIVE if possible, else AFFINE coordinates */
        if ( !epoint_set(Gx,Gy,0,G) )     {
                        std::cerr << "Error: 'init': Point (generator) does not lie on the curve!" << std::endl;
            exit(1);
        }
	}
	cout << "----------------------------------------------------------------------------------------" << endl;
}


void ECEG::keyGen()  {
         
cout << "----------------------------------Key Generation----------------------------------------" << endl;
	if (!initialized)   {
                std::cerr << "Error: 'setPoint': It must first be initialized!" << std::endl;
        exit(1);
    	}

	mip->IOBASE=16;
        bigrand(ord,sk);      
    	fout.open(SKFILE);
     	cout<<sk<<endl;   
	fout<<sk<<endl;
	fout.close();
	fout.clear();
        ecurve_mult(sk,G,Q);
	pky=epoint_get(Q,pkx,pkx);
	fout.open(PKFILE);
        fout<<pkx<<endl;
        fout<<pky<<endl;
        fout.close();
	fout.clear();
	cout << "----------------------------------------------------------------------------------------" << endl;
}

cipher ECEG::Enc(std::istream &plain)  {
	          
	cout << "----------------------------------Encryption----------------------------------------" << endl;	
	if (!initialized)   {
                std::cerr << "Error: 'setPoint': It must first be initialized!" << std::endl;
        exit(1);
    	}


	epoint_set(pkx,pkx,pky,Q);
	bigrand(ord,r);
        //Read Plaintext
	mip->IOBASE=10;
	plain>>m;
	cout << "plaintext : " << m << endl;
	mip->IOBASE=16;

	

        ecurve_mult(m,G,M);
	ecurve_mult(r,G,C1);
	My=epoint_get(M,Mx,Mx);
	//cout<<M<<endl;
        //cout<<Mx<<endl;
	//cout<<My<<endl;
	ecurve_mult(r,Q,C2);
        ecurve_add(M,C2);
	C1y=epoint_get(C1,C1x,C1x);
	C2y=epoint_get(C2,C2x,C2x);
	fout.open(CFILE);
	fout<<C1x<<endl;
        fout<<C1y<<endl;
        fout<<C2x<<endl;
        fout<<C2y<<endl;
        fout.close();
	fout.clear();
	cout<<C1x<<endl;
        cout<<C1y<<endl;
        cout<<C2x<<endl;
        cout<<C2y<<endl;
	cout << "----------------------------------------------------------------------------------------" << endl;


	copy(C1x,ciph.aa);
	ciph.bb = C1y;
	copy(C2x,ciph.cc);
	ciph.dd = C2y;
	return ciph;
}



big ECEG::Dec()  {
	cout << "----------------------------------Decryption----------------------------------------" << endl;
	if (!initialized)   {
                std::cerr << "Error: 'setPoint': It must first be initialized!" << std::endl;
        exit(1);
    	}

	//Read Secret Key
 	//sKey>>sk;

	//Read Ciphertext
	//cipher >> C1x >> C1y >> C2x >> C2y;
	
	cout << C1x << endl << C1y << endl << C2x << endl << C2y << endl;
	epoint_set(C1x,C1x,C1y,C1);
	epoint_set(C2x,C2x,C2y,C2);
	
	ecurve_mult(sk,C1,C1);
	ecurve_sub(C1,C2);
	epoint_copy(C2,M);

	ifstream ecSource2("./curves/ec224bits.ecs");
	CECContext::init(ecSource2, MR_PROJECTIVE, 10,M);
	ecSource2.close();
	ecSource2.clear();
	big result = mirvar(0);
	CTimer stopwatch;
	big paramA = mirvar(0);
	big paramB = mirvar(0);
	convert(0,paramA);

	//convert(pow(2,MSG_SIZE)-1,paramB);
	convert(150000,paramB);
	cout<<paramA<<endl<<paramB<<endl;
 	CLambdaAlg::setLambdaParams(paramA,paramB);

	if ( CLambdaAlg::computeLogarithm(result)  )        {
		
		cout << "result !!= " << result << endl;
char ptr[200];
		mip->IOBASE=10;
		 cotstr(result,ptr);
		cout << "result !!= " << ptr << endl;
		
		cout << "The Computation of Pollard-Lambda algoritmom lasted: " << stopwatch.getTime() << " seconds" << endl;
	} else      {
		cerr << "The computation failed! it took " << stopwatch.getTime() << " seconds" << endl;

		//exit(EXIT_FAILURE);
	}

	cout << "----------------------------------------------------------------------------------------" << endl;
    	return result;
}

cipher ECEG::addCiphers(std::istream &cipherA, std::istream &cipherB)   {

	cout << "----------------------------------Point Addition----------------------------------------" << endl;	
	if (!initialized)   {
                std::cerr << "Error: 'setPoint': It must first be initialized!" << std::endl;
        exit(1);
    	}
	int y1,y2;
	//Read Ciphertexts
	cipherA >> CA1x >> CA1y >> CA2x >> CA2y;
	epoint_set(CA1x,CA1x,CA1y,CA1);
	epoint_set(CA2x,CA2x,CA2y,CA2);

	cipherB >> CB1x >> CB1y >> CB2x >> CB2y;
	epoint_set(CB1x,CB1x,CB1y,CB1);
	epoint_set(CB2x,CB2x,CB2y,CB2);

	ecurve_add(CA1,CB1);
	ecurve_add(CA2,CB2);

	y1=epoint_get(CB1,x1,x1);
	y2=epoint_get(CB2,x2,x2);

	fout.open(CFILE);
	fout<<x1<<endl;
        fout<<y1<<endl;
        fout<<x2<<endl;
        fout<<y2<<endl;
        fout.close();
	fout.clear();
	copy(x1,ciph.aa);
	ciph.bb = y1;
	copy(x2,ciph.cc);
	ciph.dd = y2;
	return ciph;
	cout << "----------------------------------------------------------------------------------------" << endl;
}

void ECEG::free() {
	if (ecA != NULL) mirkill(ecA);
	if (ecB != NULL) mirkill(ecB);
	if (ecP != NULL) mirkill(ecP);
	if (ord != NULL) mirkill(ord);
	if (G != NULL) epoint_free(G);
	if (Q != NULL) epoint_free(Q);
	if (M != NULL) epoint_free(M);
	if (C1 != NULL) epoint_free(C1);
	if (C2 != NULL) epoint_free(C2);
	if (CA1 != NULL) epoint_free(CA1);
	if (CA2 != NULL) epoint_free(CA2);
	if (CB1 != NULL) epoint_free(CB1);
	if (CB2 != NULL) epoint_free(CB2);
	mirexit();
	initialized = false;
}
