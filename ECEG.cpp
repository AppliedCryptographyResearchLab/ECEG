#include "ECEG.hpp"

using namespace std;

#define  PKFILE "EC_PK.txt"
#define  SKFILE "EC_SK.txt"
#define  CFILE "ECEG_Cipher.txt"
#define  PFILE "ECEG_Plain.txt"

int pky,My,C1y,C2y;
big a,pkx,Mx,C1x,C2x,sk,m,r;
ofstream fout;
miracl *mip;
#ifndef MR_NOFULLWIDTH   
    mip=mirsys(36,0);
#else
    mip=mirsys(36,MAXBASE);
#endif

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

void ECEG::init(std::istream &ecSource)
{
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

	a=mirvar(0);
        sk=mirvar(0);
        ecA = mirvar(0);
        ecB = mirvar(0);
        ecP = mirvar(0);
        ord = mirvar(0);
        pkx = mirvar(0);
        r=mirvar(0);
	m=mirvar(0);
	Mx = mirvar(0);
	C1x = mirvar(0);
	C2x = mirvar(0);
	int bits,pky;
	big x = mirvar(0);
	big y = mirvar(0);
	mip->IOBASE=10;
	ecSource >> bits;
	mip->IOBASE=16;
	ecSource >> ecP >> ecA >> ecB >> ord >> x >> y;
        //convert(-3,a);


        cout<<bits<<endl<<a<<endl<<x<<endl<<ord<<endl;
        ecurve_init(ecA, ecB, ecP, MR_BEST);  /* Use PROJECTIVE if possible, else AFFINE coordinates */
        if ( !epoint_set(x,y,0,G) )     {
                        std::cerr << "Error: 'init': Point (generator) does not lie on the curve!" << std::endl;
            exit(1);
        }
	}

}


void ECEG::keyGen()  {
         

	if (!initialized)   {
                std::cerr << "Error: 'setPoint': It must first be initialized!" << std::endl;
        exit(1);
    	}
        bigrand(ord,sk);      
    	fout.open(SKFILE);
     	cout<<sk<<endl;   
	fout<<sk<<endl;
	fout.close();
	H = epoint_init();
	big *H;
        ecurve_mult(sk,G,H);
	pky=epoint_get(H,pkx,pkx);
	fout.open(PKFILE);
        fout<<pkx<<endl;
        fout<<pky<<endl;
        fout.close();

}

void ECEG::Enc(std::istream &pKey, std::istream &plain)  {
	          
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
	ifstream ecSource2("./curves/ec50bits.ecs");
	CECContext::init(ecSource2, MR_PROJECTIVE, 10,M);
	big result = mirvar(0);
	CTimer stopwatch;
	if ( CLambdaAlg::computeLogarithm(result)  )        {
        cout << "result = " << result << endl;
        cout << "The Computation of Pollard-Rho (improved brent) algoritmom lasted: " << stopwatch.getTime() << " seconds" << endl;
    } else      {
        cerr << "The computation failed! it took " << stopwatch.getTime() << " seconds" << endl;
        //exit(EXIT_FAILURE);
    }

    
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
    if (H != NULL)
        epoint_free(H);
    if (initialized)    {
        for (int i = 0; i < BIG_BUFFER_SIZE; i++)       {
            mirkill(bigBuffer[i]);
        }
    }
    mirexit();
    initialized = false;
}
