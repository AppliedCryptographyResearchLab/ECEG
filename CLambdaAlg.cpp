/**
 * @file CLambdaAlg.cpp Definition of class CLambdaAlg.
 * @author Mohamed Grissa.
 * @ref: adapted from Martin Kosorinský's implementation of Pollard Lambda algorithm.
 * @date 03/15/2015.
 **/

#include "CLambdaAlg.hpp"

big CLambdaAlg::stepLimit = NULL;
big CLambdaAlg::paramA = NULL;
big CLambdaAlg::paramB = NULL;


void CLambdaAlg::setLambdaParams(const big a, const big b)      {
        if (paramA == NULL)    
                paramA = mirvar(0);                                                 // allocating the lower boundary scan interval
        if (paramB == NULL)    
                paramB = mirvar(0);                                                 // allocating the lower boundary scan interval               
        if (stepLimit == NULL)
                stepLimit = mirvar(0);

        if (a != NULL)
                copy(a,paramA);
        else    
                zero(paramA);                                                       // default setting the lower limit = 0
       
        if (b != NULL)
                copy(b,paramB);
        else
                decr(ord, 1, paramB);                                               // Upper limit set to ord-1 -> thus searches the entire group
}

void CLambdaAlg::freeLambdaParams()     {
        if (paramA != NULL)     {
                mirkill(paramA);
                paramA = NULL;
        }
        if (paramB != NULL)     {
                mirkill(paramB);
                paramB = NULL;
        }
        if (stepLimit != NULL)  {
                mirkill(stepLimit);
                stepLimit = NULL;
        }
}

bool CLambdaAlg::computeLogarithm(big result) {
    
    tracer << "Pollard-Lambda:" << endl;
    if ( !isInitialized() )     {
        cerr << "It is not initialized context!" << endl;
        return false;
    }

        //setLambdaParams();                                                              // allocation and set a static calculation parameters                     
       
        big paramN = mirvar(0);                                             // Number of traps = number of jumps Taming kangaroos
        subtract(paramB, paramA, bigBuffer[0]);                 // auxiliary scanning width calculation interval
        nroot(bigBuffer[0], 2, paramN);                                 // number of traps set to root lengths crawl interval
        sftbit(paramN, 1, stepLimit);                                   // The average value jumps the root of the interval length.
        // TODO: StepLimit set to the nearest power of 2 for speed
	miracl *mip = get_mip();
	std::cout << "Base" << mip->IOBASE << std::endl;
	mip->IOBASE = 10;
	std::cout << "Base" << mip->IOBASE << std::endl;
    tracer << "Interval: (" << paramA << "," << paramB << ")" << endl;
    tracer << "Width of interval: " << bigBuffer[0] << endl;
    tracer << "Parameter N: " << paramN << endl;
    tracer << "Step limit: " << stepLimit << endl;

    epoint * T = epoint_init();                                     // point representing tame kangaroo
    big dT = mirvar(0);                                                         // distance traveled Taming kangaroos
    ecurve_mult(paramB, G, T);                                      // skrotena  kangaroo starts running at the end of the crawl interval

    for (zero(bigBuffer[2]); mr_compare(bigBuffer[2],paramN) < 1 ; incr(bigBuffer[2],1,bigBuffer[2]) )     {
        makeStep(T, dT);                                                        // deterministic pseudorandom jump Taming kangaroos
    }
    tracer << "Skrotena kangaroo is entrenched in " << T << " (distance = " << dT << ")" << endl;

    epoint * W = epoint_init();                                         // wild kangaroo
    big dW = mirvar(0);                                                     // distance traveled wild kangaroos
    epoint_copy(H, W);                                                          // wild kangaroo starts at that point which DL hladame
    big distT = mirvar(0);                                                      // The proxy for the calculations
    add(paramB, dT, distT);                                                     // We have stored in dist dn + paramB
        bool catchedWild = false;                                               // flag signaling the capture of wild kangaroos

    for (int i = 1; i <= 10; i++)       {                               // sequentially deleted 10 wild kangaroo
        while (!catchedWild)    {
            if ( epoint_comp(W,T) )     {                               // coordinates wild kangaroos compared with a buried
                tracer << "THE WILD kangaroo trapped!" << " (distance = " << dW << ")" << endl;
                               
                subtract(distT, dW, result);          // index calculation
                divide(result, ord, ord);                     // Index Module range of groups (for sure: P)
                                catchedWild = true;                                     // Indicates captured kangaroos
                                break;
            }
            makeStep(W, dW);                                            // power jump wild kangaroos

            subtract(distT, paramA , result);         // verify that wild kangaroo is not already too far for a buried
            if ( mr_compare(result, dW) == -1 )  {       // if TRUE, wild kangaroo escaped .. no meaning to continue calculating
                tracer << endl << "WILD kangaroo drift away !!" << endl;
                catchedWild = false;                            // kangaroo has been caught
                                break;                                                          // interrupt loops -> deallocation and end the computation of failure
            }
        }
                if (catchedWild)                                                        // if the kangaroo caught, interrupt hladanie
                        break;

        // when we were there and escaped kangaroo .. we make a new search with new wild kangaroos
        // TODO: There would be properly treat with the new shift kangaroos
        tracer << "Exclude other wild kangaroo." << endl;
        epoint_copy(H,W);
        for (int n = 0; n < i; n++)
            ecurve_add(G,W);
        zero(dW);
        incr(dW, i, dW);
    }
        if (!catchedWild)
                tracer << endl << "Failure: all the kangaroos ran off !!! Koncime calculation." << endl;

        freeLambdaParams();
    mirkill(paramN);
    mirkill(dT);
    epoint_free(T);
    mirkill(distT);
    mirkill(dW);
    epoint_free(W);
    return catchedWild;
}

inline void CLambdaAlg::getStepSize(const epoint * P, big stepSize)     {
    epoint_get( (epoint*)P, bigBuffer[0], bigBuffer[1]);
    add(bigBuffer[0], bigBuffer[1], bigBuffer[0]);
    divide(bigBuffer[0], stepLimit, stepLimit);
    incr(bigBuffer[0], 1, stepSize);
}

inline void CLambdaAlg::makeStep(epoint * P, big dm)    {
    epoint * X = epoint_init();
    getStepSize(P, bigBuffer[0]);
    ecurve_mult( bigBuffer[0], G, X);
    ecurve_add(X, P);
    add(dm, bigBuffer[0], dm);
    epoint_free(X);
}

