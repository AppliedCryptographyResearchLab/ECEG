#include "ECEG.hpp"

using namespace std;

#define  PKFILE "EC_PK.txt"
#define  SKFILE "EC_SK.txt"
#define  CFILE "ECEG_Cipher.txt"
#define  PFILE "ECEG_Plain.txt"
#define  MSG_SIZE 20
int pky,My,C1y,C2y,CA1y,CA2y,CB1y,CB2y;
big a,pkx,Mx,C1x,C2x,CA1x,CA2x,CB1x,CB2x,sk,m,r,x1,x2,r_crt,d,diff;
ofstream fout;
miracl *mip = mirsys(36,0);
/*#ifndef MR_NOFULLWIDTH   
    mip=mirsys(36,0);
#else
    mip=mirsys(36,MAXBASE);
#endif*/

bool ECEG::initialized = false;
big ECEG::ecA = NULL;
big ECEG::ecB = NULL;
big ECEG::ecP = NULL;
big ECEG::ord = NULL;
big ECEG::bigBuffer[BIG_BUFFER_SIZE];
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
	cout << "----------------------------------Initialization----------------------------------------" << endl;
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
        //convert(-3,a);

	initialized = true;
	
        cout<<bits<<endl<<a<<endl<<Gx<<endl<<ord<<endl;
        ecurve_init(ecA, ecB, ecP, MR_PROJECTIVE);  /* Use PROJECTIVE if possible, else AFFINE coordinates */
        if ( !epoint_set(Gx,Gy,0,G) )     {
                        std::cerr << "Error: 'init': Point (generator) does not lie on the curve!" << std::endl;
            exit(1);
        }
	}

}


void ECEG::keyGen()  {
         
cout << "----------------------------------Key Generation----------------------------------------" << endl;
	if (!initialized)   {
                std::cerr << "Error: 'setPoint': It must first be initialized!" << std::endl;
        exit(1);
    	}
	
	big msgSpace,diff,r_crt,d;
	msgSpace = mirvar(0);
	diff = mirvar(0);
	r_crt = mirvar(0);
	d = mirvar(0);

	convert(pow(2,MSG_SIZE),msgSpace);
	cout<< "msgSpace:"<< msgSpace<< endl;
	subtract(ord,msgSpace,diff);
	cout<< "diff:"<< diff<< endl;
	bigrand(diff,r_crt);
	cout<< "r_crt:"<< r_crt<< endl;
	subtract(ord,r_crt,d);
	cout<< "order:"<< ord << endl;
	mip->IOBASE=10;
	cout<< "d:"<< d << endl;

	std::ostringstream stream;
	stream << "./facter " << d;
	std::string result = stream.str();
	const char * c = result.c_str();
	std::system(c);
	//std::system("./facter");

	mip->IOBASE=16;
        bigrand(ord,sk);      
    	fout.open(SKFILE);
     	cout<<sk<<endl;   
	fout<<sk<<endl;
	fout.close();
        ecurve_mult(sk,G,Q);
	pky=epoint_get(Q,pkx,pkx);
	fout.open(PKFILE);
        fout<<pkx<<endl;
        fout<<pky<<endl;
        fout.close();

}

void ECEG::Enc(std::istream &pKey, std::istream &plain)  {
	          
cout << "----------------------------------Encryption----------------------------------------" << endl;	
	if (!initialized)   {
                std::cerr << "Error: 'setPoint': It must first be initialized!" << std::endl;
        exit(1);
    	}
       
	//Read Public Key
 	pKey>>pkx>>pky;
	epoint_set(pkx,pkx,pky,Q);
	bigrand(ord,r);
        //Read Plaintext
	plain>>m;
        ecurve_mult(m,G,M);
	ecurve_mult(r,G,C1);
	My=epoint_get(M,Mx,Mx);
	cout<<M<<endl;
        cout<<Mx<<endl;
	cout<<My<<endl;
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

 
}



void ECEG::Dec(std::istream &sKey, std::istream &cipher)  {
	cout << "----------------------------------Decryption----------------------------------------" << endl;
	if (!initialized)   {
                std::cerr << "Error: 'setPoint': It must first be initialized!" << std::endl;
        exit(1);
    	}
    	
	//Read Secret Key
 	sKey>>sk;

	//Read Ciphertext
	cipher >> C1x >> C1y >> C2x >> C2y;
	epoint_set(C1x,C1x,C1y,C1);
	epoint_set(C2x,C2x,C2y,C2);

	ecurve_mult(sk,C1,C1);
	ecurve_sub(C1,C2);
	epoint_copy(C2,M);
	ifstream ecSource2("./curves/ec192bits.ecs");
	CECContext::init(ecSource2, MR_PROJECTIVE, 10,M);
	big result = mirvar(0);
	CTimer stopwatch;
	big paramA = mirvar(0);
	big paramB = mirvar(0);
	convert(0,paramA);
	convert(pow(2,MSG_SIZE)-1,paramB);
	cout<<paramA<<endl<<paramB<<endl;
 	CLambdaAlg::setLambdaParams(paramA,paramB);
	if ( CLambdaAlg::computeLogarithm(result)  )        {

		cout << "result = " << result << endl;
		cout << "The Computation of Pollard-Lambda algoritmom lasted: " << stopwatch.getTime() << " seconds" << endl;
	} else      {
		cerr << "The computation failed! it took " << stopwatch.getTime() << " seconds" << endl;
		//exit(EXIT_FAILURE);
	}

    
}

void ECEG::addCiphers(std::istream &cipherA, std::istream &cipherB)   {

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



}

void ECEG::free() {
    if (ecA != NULL)
        mirkill(ecA);
    if (ecB != NULL)
        mirkill(ecB);
    if (ecP != NULL)
        mirkill(ecP);
    if (ord != NULL)
        mirkill(ord);
    if (G != NULL)
        epoint_free(G);
    if (initialized)    {
        for (int i = 0; i < BIG_BUFFER_SIZE; i++)       {
            mirkill(bigBuffer[i]);
        }
    }
    mirexit();
    initialized = false;
}
