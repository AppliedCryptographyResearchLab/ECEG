/**
 * @file CECContext.cpp Definition of class CECContext.
 * @author Mohamed Grissa.
 * @ref: adapted from Martin Kosorinský's implementation of Pollard Lambda algorithm.
 * @date 03/15/2015.
 **/

#include "CECContext.hpp"

bool CECContext::initialized = false;
big CECContext::ecA = NULL;
big CECContext::ecB = NULL;
big CECContext::ecP = NULL;
big CECContext::ord = NULL;
big CECContext::bigBuffer[BIG_BUFFER_SIZE];
epoint * CECContext::G = NULL;
epoint * CECContext::H = NULL;


void CECContext::init(std::istream &ecSource, int mrPointRep, int mrPrecision, const epoint * point)  {

    /*if (initialized)    {
                std::cerr << "Error: 'init': Context is already initialized!" << std::endl;
        return;
    }*/

    miracl *mip = mirsys(mrPrecision,0);
    irand( (unsigned int)time(0) );

    ecA = mirvar(0);
    ecB = mirvar(0);
    ecP = mirvar(0);
    ord = mirvar(0);
    G = epoint_init();
    H = epoint_init();
	
    if ( ecSource.fail() )      {
        std::cerr << "Error: 'init': Failed to load curves from file!" << std::endl;
        exit(1);
    } else      {
        int bits;
        big x = mirvar(0);
        big y = mirvar(0);

        mip->IOBASE=10;
        ecSource >> bits;
        mip->IOBASE=16;
        ecSource >> ecP >> ecA >> ecB >> ord >> x >> y;
	//ecSource >> ecP >> ecB >> ord >> y;
        mip->IOBASE=10;
	
        for (int i = 0; i < BIG_BUFFER_SIZE; i++)       {
            bigBuffer[i] = mirvar(0);
        }

        ecurve_init(ecA, ecB, ecP, mrPointRep);
        if ( !epoint_set(x,y,0,G) )     {
                        std::cerr << "Error: 'init': Point (generator) does not lie on the curve!" << std::endl;
            exit(1);
        }
                initialized = true;
               
                setPoint(point);     // Search Settings item on random in the group

        tracer << "a   = " << ecA << " (" << logb2(ecA) << " bits)\n";
        tracer << "b   = " << ecB << " (" << logb2(ecB) << " bits)\n";
        tracer << "p   = " << ecP << " (" << logb2(ecP) << " bits)\n";
        tracer << "ord = " << ord << " (" << logb2(ord) << " bits)" << std::endl;
        tracer << "G   = " << G << std::endl;
        tracer << "H   = " << H << " index=" << bigBuffer[0] << std::endl;

                mirkill(x); mirkill(y);
		mip->IOBASE=16;

    }
}

void CECContext::init(std::istream &ecSource, std::istream &dlSource, int mrPointRep, int mrPrecision)  {
	CECContext::init(ecSource, mrPointRep, mrPrecision);
	big x = mirvar(0);
	big y = mirvar(0);
	miracl *mip = get_mip();
	mip->IOBASE=16;
	dlSource >> x;
	dlSource >> y;
	mip->IOBASE=10;
	if ( !epoint_set(x,y,0,H) ) {
		std::cerr << "Error: 'init': Point  (DL assignment) does not lie on the curve!" << std::endl;
		mirkill(x); mirkill(y);
		exit(1);
	}
	mirkill(x); mirkill(y);
	mip->IOBASE=16;
}

void CECContext::free() {
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

void CECContext::setPoint(const big x, const big y)     {
    if (!initialized)   {
                std::cerr << "Error: 'setPoint': context must be initialized first !" << std::endl;
        exit(1);
    }
    if ( !epoint_set(x,y,0,H) ) {
                std::cerr << "Error: 'setPoint': Point does not lie on the curve!" << std::endl;
                exit(1);
        }
}

void CECContext::setPoint(const epoint * P)     {
    if (!initialized)   {
                std::cerr << "Error: 'setPoint': context must be initialized first !" << std::endl;
        exit(1);
    }
        if (P != NULL)
                epoint_copy( (epoint*)P, H);
        else    {
        bigrand(ord,bigBuffer[0]);
        ecurve_mult(bigBuffer[0], G, H);
        }
}

void CECContext::setPoint(const big n)  {
    if (!initialized)   {
                std::cerr << "Error: 'setPoint': context must be initialized first !" << std::endl;
        exit(1);
    }
        else    {
        ecurve_mult(n, G, H);
        }
}

