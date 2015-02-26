#ifndef ECEG_HEADER
#define ECEG_HEADER

#include <stdio.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include "miraclExtensions.hpp"
#include "CRhoBrentAlg.hpp"
#include "CTimer.hpp"
#include "CLambdaAlg.hpp"
#include "CECContext.hpp"



/**
 * @brief The size of the auxiliary buffer containing pre-allocated structure big.
 * @see ECEG::bigBuffer[].
 */
#ifndef BIG_BUFFER_SIZE
#define BIG_BUFFER_SIZE         3
#endif

/**
 * @brief Class representing the different steps of the Elliptic Curve El Gamal Cryptosystem.
 */
class ECEG        {
protected:
    /**
     * @brief Parameter A of elliptic curve.
     */
    static big ecA;
    /**
     * @brief Parameter B of elliptic curve.
     */
    static big ecB;
    /**
     * @brief elliptic curve prime.
     */
    static big ecP;
    /**
     * @brief group order.
     */
    static big ord;
    /**
     * @brief group generator.
     */
    static epoint * G;
    /**
     * @brief Elliptic curve point that we are looking for its DL.
     */
    static epoint * H;
    /**
     * @brief Point that reperesents the public key for ECEG.
     */
    static epoint * Q;
    /**
     * @brief Point that represents the mapped message on the curve.
     */
    static epoint * M;
    /**
     * @brief Point that reperesents the first component of the ciphertext.
     */
    static epoint * C1;
    /**
     * @brief Point that reperesents the second component of the ciphertext.
     */
    static epoint * C2;
    /**
     * @brief Auxiliary buffer statically allocated structures big.
      * The main objective is to eliminate unnecessary allocation and deallocation of variables of type big
      * In critical procedures where repeated allocation and deallocation could cause
      * Significant reduction in the efficiency calculation. Field size can be determined only at compile time
      * Setup BIG_BUFFER_SIZE.
     * @warning Should be used with caution, may cause difficulties to detect errors.
     * @see BIG_BUFFER_SIZE.
     */
    static big bigBuffer[BIG_BUFFER_SIZE];
    /**
     * @brief Non-public constructor (as it is not possible to instantiate this class)
     */
    ECEG() {};
public:
        /**
         * @brief Initializer of the ECEG class parameters.
         * @param[in] takes as input the data stream containing the parameters of of the elliptic curve.

         */

        static void init(std::istream &ecSource);

    /**
     * @brief Allocation procedure.
     * Ensures the allocation of resources and initialize the library miracle.
     * @see init().
     */
        static void free();
        /**
         * @brief Generates the keys used in the ECEG Cryptosystem.
         * The procedure ensures the setting point by coordinates supplied as parameters. The procedure does not provide for the
         * allocation of memory alone set the coordinates of the point.
         * @warning Before calling this procedure, the procedure init (std::istream &ecSource) must be called.
         */
        static void keyGen();
   
    static bool isInitialized() {
        return initialized;
    };
private:
    /**
     * @brief Flag state initialization.
     */
    static bool initialized;
};

#endif /* ECEG_HEADER */

