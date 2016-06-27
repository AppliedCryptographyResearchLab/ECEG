/**
 * @file CLambdaAlg.hpp declaration of class CLambdaAlg.
 *
 * @author Martin Kosorinský.
 * @date 1.4.2009.
 */

#ifndef CLAMBDAALG_HEADER
#define CLAMBDAALG_HEADER

#include <iostream>
#include "CECContext.hpp"

using namespace std;

/**
 * @brief Class search encapsulating discrete logarithm method Pollard-Lambda.
 * Class encapsulates the methods and data elements necessary for the calculation of the discrete   
 * logarithm in the group of points elliptic curves. It uses algorimtus known as Pollard-Lambda.
 * @todo Draw up the final version of the algorithm, and implemented two slightly different 
 *  procedures that were presented.
 * @todo Finalize the main algorithm so as to be processed as "distinguished" points.
 * @todo Resolve strategy in case of failure of the search in the selected interval.
 * Select the appropriate parameters for the length of the jumps and the number of hops srotenej kangaroos.
 */
class CLambdaAlg : public CECContext    {
public:
    /**
     * @brief main method of triggering a calculation of the discrete logarithm.
     * The method works on the basis of pre-initialized settings superclass CECContext, including the parameters of the curve and the point, the discrete logarithm looking for.
         * @param[out] result big variable type, in which the result will be stored in case of success calculation.
         * Memory must be allocated in advance!
         * @warning VIn case of failure calculation includes variablea result undefined value.
         * @return Returns TRUE if successful calculation, otherwise it returns FALSE.
     */
    static bool computeLogarithm(big result);
        /**
     * @brief The initializer static calculation parameters.
         * If the parameters are not specified (default value will be NULL) and the parameter A is 
         * set to 0, the upper limit crawl interval B will have a value of <i> ord - 1 </ i>, 
         * where ord is the order of the group.
         * @param[in] a value, which will be initialized parameter A (lower limit of the interval), where a! = NULL.
         * @param[in] b value, which will be initialized parameter B (upper limit of the interval), where b! = NULL.
         */
        static void setLambdaParams(const big a = NULL, const big b = NULL);
        /**
     * @brief Deallocator static parameter calculation algorithm Pollard-Lambda.
         * Ensures correct deallocation of static data items, where their value is not NULL.
         * The parameters that are never allocated set to NULL.
         */
        static void freeLambdaParams();
protected:
    /**
     * @brief The lower limit of the interval crawled.
         * Allocation and setting values provide internal procedure computeLogarithm ().
     */
        static big paramA;
    /**
     * @brief The upper limit of the interval crawled.
         * Allocation and setting values provide internal procedure computeLogarithm ()
     */
        static big paramB;
    /**
     * @brief Limit on which the computed hash of points.
     * Acquired its hash value will range from <1,stepLimit>.
     */
    static big stepLimit;
    /**
     * @brief A feature that provides the calculation of the length of the nearest jump.
     * Function deterministically based on the current state (point @a P) calculates the length of the jump for the needs makeStep functions.
     * @see makeStep().
     * @param[in] P is a point on an elliptic curve on which we calculate the jump.
     * @param[out] stepSize will be at the end of a function of the size match is always obtained for point @a P.
     */
    static inline void getStepSize(const epoint * P, big stepSize);
    /**
     * @brief Procedure performing jump "kangaroos".
     * Command executes a jump from the specified point and adjust it based on the distance traveled.
      * Return is calculated on the basis of the auxiliary function getStepSize and entry point @a P.
     * @see getStepSize().
     * @param[in,out] P is the input-output parameter representing a point on an elliptic curve from which jumping.
     * @param[in,out] dm is the total distance traveled by type big.
     */
    static inline void makeStep(epoint * P, big dm);
};

#endif /* CLAMBDAALG_HEADER */

