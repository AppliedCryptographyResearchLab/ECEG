/**
 * @file CECContext.cpp Definition of class CECContext.
 * @author Mohamed Grissa.
 * @ref: adapted from Martin Kosorinský's implementation of Pollard Lambda algorithm.
 * @date 03/15/2015.
 **/

#ifndef CECCONTEXT_HEADER
#define CECCONTEXT_HEADER

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <ctime>
#include "miraclExtensions.hpp"

/**
 * @brief The size of the auxiliary buffer containing pre-allocated structure big.
 * @see CECContext::bigBuffer[].
 */
#ifndef BIG_BUFFER_SIZE
#define BIG_BUFFER_SIZE         3
#endif

/**
 * @brief Class representing the context of calculating discrete logarithms over an elliptic curve.
  * Class CECContext is static. Encapsulates the global calculation parameters, such as the
  * Parameters of elliptic curves, the generator and the point the discrete logarithm is looking for.
  * An important part of the class is CECContext CECContext :: init procedure that provides the 
  * Curve parameters initialization, as well as the MIRACL library.
 */
class CECContext        {
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
     * @brief Elliptic curve point that we are looking for its DL
     */
    static epoint * H;
    /**
     * @brief Auxiliary buffer statically allocated structures big.
      * The main objective is to eliminate unnecessary allocation and deallocation of variables of type big
      * In critical procedures where repeated allocation and deallocation could cause
      * Significant reduction in the efficiency calculation. Field size can be determined only at compile time
      * Setup BIG_BUFFER_SIZE.
     * @warning Should be used with caution, may cause difficult to detect errors.
     * @see BIG_BUFFER_SIZE.
     */
    static big bigBuffer[BIG_BUFFER_SIZE];
    /**
     * @brief NNon-public constructor (as it is not possible to instantiate this class)
     */
    CECContext() {};
public:
    /**
     * @brief Initializer context of calculating the discrete logarithm.
     * Procedure provides for the allocation and initialization of static initialization parameter curve and libraries
     * MIRACL. Information on the curve, and the calculations are loaded from a text file whose full
     * Path is an input parameter of the procedure. The library Miracle transfer settings variables of type big as
     * The parameters of elliptic curves, the calculations to be carried out.
     * During initialization, there is everovaniu correctness of entries, which can cause errors libraries
     * MIRACL during the calculation.
         * Format input ecSource: hotels
         * - The bit size of a prime module in decimal.
         * - Prime modulus curve in hexadecimal.
         * - A Curve parameter in hexadecimal format.
         * - Parameter curve B in hexadecimal.
         * - I groups elliptic curve points in hexadecimal.
         * - X-coordinate generator groups in hexadecimal.
         * - Y-coordinate generator groups in hexadecimal.
         *
         * Point, the discrete logarithm counting is set at random on the basis of the curve.
         * @warning Initialization must take place before the start of the calculation, but before application of MIRACL library.
     * @see free().
     * @param[in] ecSource  is a data input stream containing the parameters of elliptic curves.
         * @param[in] mrPointRep code is desired representation of elliptic curve points. Permissible values are
         * MR_PROJECTIVE and MR_AFFINE representing Parameter evurve_init (). The default value is MR_PROJECTIVE.
         * @param[in] mrPrecision defines accuracy MIRACL library (first Parameter mirsys). Default value is 10.
     */
        static void init(std::istream &ecSource, int mrPointRep = MR_PROJECTIVE, int mrPrecision = 10, const epoint * P = NULL);
        /**
         * @brief Initializer (extended) context of calculating the discrete logarithm.
         * @copydetails CECContext::init()
         * @param[in] dlSource the input data stream containing the coordinates of the point, the discrete logarithm looking for.
         * Stream must contain the X-axis and Y-coordinate of the hexadecimal separated by white patches.
         */
        static void init(std::istream &ecSource, std::istream &dlSource, int mrPointRep = MR_PROJECTIVE, int mrPrecision = 10);
    /**
     * @brief Allocation procedure providing context.
     * Ensure the allocation of resources allocated and initialize the library miracle.
     * @see init().
     */
        static void free();
        /**
         * @brief Initializer point, the discrete logarithm looking for.
         * The procedure ensures the setting point by coordinates supplied as parameters. The procedure does not provide for the
         * allocation of memory alone set the coordinates of the point.
         * @warning Before calling procedure must be initialized context calculation using the procedure init ().
         * @see setPoint(const epoint *)
         * @param[in] x The first coordinate of a point on an elliptic curve.
         * @param[in] y The second coordinate of a point on an elliptic curve, which we seek.
         */
        static void setPoint(const big x, const big y);
        /**
         * @brief Initializer point, the discrete logarithm looking for.
         * Setting point procedure ensures that a copy of the parametera P. procedure
         * Does not provide for the allocation of memory alone set the coordinates of the point.
         * @warning Before calling procedure must be initialized context calculation using the procedure init ().
         * @see setPoint(const big x,const big y)
         * @param[in] Point P on an elliptic curve, which we are looking for DL. If P == NULL, then the point will be created from 
         * randomly initialized groups.
         */
        static void setPoint(const epoint * P = NULL);
        /**
         * @brief Initializer point, the discrete logarithm looking for.
         * rocedúra ensure the setting item by its index. The procedure does not provide for the allocation of memory alone set the coordinates of the point.
         * @warning Before calling procedure must be initialized context calculation using the procedure init ().
         * @see setPoint(const epoint *)
         * @param[in] n Index query point with respect to the generator initialized.
         */
        static void setPoint(const big n);
        /**
     * @brief Initializer point, the discrete logarithm looking for.
     * Boolean function whose output is TRUE if the context correctly initialized.
     * Otherwise, the return value is FALSE.
     * Flag is set to TRUE only after successfully initialized as all lots. It is thus possible,
     * Context that will only partially initialized, which is indicated by the value FALSE.
     * @see init()
     * @see free()
     * @return The return value is TRUE if the context correctly initialized. Otherwise returns FALSE.
     */
    static bool isInitialized() {
        return initialized;
    };
private:
    /**
     * @brief Flag state initialization.
     */
    static bool initialized;
};

#endif /* CECCONTEXT_HEADER */

