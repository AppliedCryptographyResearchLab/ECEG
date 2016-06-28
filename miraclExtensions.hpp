/**
 * @file miraclExtensions.hpp Extension for MIRACL libraries, declaration of CEpointCmp and CEpointEq.
 *
 * @author Martin Kosorinský.
 * @date 20.3.2009.
 */

#ifndef MIRACLEEXTENSIONS_HEADER
#define MIRACLEEXTENSIONS_HEADER

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <string>
#include <ctime>

#ifdef __cplusplus
extern "C"      {
#endif
#include "miracl.h"
#ifdef __cplusplus
}
#endif

#if (__GNUC__ >= 4 || _MSC_VER >= 1500)
    #ifdef __GNUC__
    #include <tr1/unordered_map>
    #else
    #include <unordered_map>
    #endif
        struct CEpointCmp;
        struct CEpointEq;
        #define USING_HASHTABLE
        typedef std::tr1::unordered_map<epoint*, big, CEpointCmp, CEpointEq> TLambdaEpointList;
        typedef std::tr1::unordered_map<epoint*, std::pair<big,big>, CEpointCmp, CEpointEq> TRhoEpointList;
#else
#include <map>
struct CEpointCmp;
typedef std::map<epoint*, big, CEpointCmp> TLambdaEpointList;
typedef std::map<epoint*, std::pair<big,big>, CEpointCmp> TRhoEpointList;
#endif

/**
 * @brief Advanced trace macro - lists the name of the function from which it was called.
 * Writing with buffering messages in the error. It leverages the capabilities of C ++ streams.
 * Optimalizácia kompilátora by mala zabezpečiť jeho neúčinnosť a neprítomnosť v kóde v prípade
 * release kompilácie.
 */
#ifdef _DEBUG
#define tracerex std::clog << "TRACE [" << __FUNCTION__ << "]: "

/**
 * @brief Trace macro.
 * Writing with buffering messages in the error. Leverages the capabilities of C ++ streams.
 * Optimalizácia kompilátora by mala zabezpečiť jeho neúčinnost a neprítomnosť v kóde v prípade
 * release kompilácie.
 */
#define tracer std::clog
#else
/**
 * @brief Rožšírené trace makro - vypisuje meno funkcie, z ktorej bolo volané.
 * Zapisuje pomocné správy do bufferovaneho chybového výstupu. Využíva možnosti C++ prúdov.
 * Optimalizácia kompilátora by mala zabezpečiť jeho neúčinnost a neprítomnosť v kóde v prípade
 * release kompilácie.
 */
#define tracerex if(1); else std::clog

/**
 * @brief Trace makro.
 * Zapisuje pomocné správy do bufferovaneho chybového výstupu. Využíva možnosti C++ prúdov.
 * Optimalizácia kompilátora by mala zabezpečiť jeho neúčinnost a neprítomnosť v kóde v prípade
 * release kompilácie.
 */
#define tracer if(1); else std::clog
#endif


/**
 * @brief Komparátor a hasher bodov na eliptickej krivke.
 * Súčastou triedy sú operátory usporiadania dvoch bodov na eliptickej krivke, ako ja hashovací
 * operátor. Trieda je vytvorená na základe požiadaviek štandardných STL usporiadaných kontajnerov
 * ako aj hashovacích tabuliek (unordered_map).
 * @todo Táto trieda je pre hashovacie kontajnery zbytočná, hlavne komparačná časť. Do budúcnosti
 * by bolo vhodné triedu upraviť, alebo hashovaciu zložku presunúť do CEpointEq.
 */
#ifdef USING_HASHTABLE
struct CEpointCmp  : public std::tr1::hash<epoint*>     {
#else
struct CEpointCmp       {
#endif
    /**
     * @brief Operátor porovnania dvoch bodov na eliptickej krivke.
     * Funkcia porovnáva dva body na základe ich afinných súradníc. Prioritou pri
     * porovnávaní je prvá, x-ová súradnica. V prípade rovnosti rozhoduje o výsledku
     * y-ová súradnica.
     * Funkcia vyhodnocuje nasledovný výraz: (E1.x < E2.x) || ((E1.x == E2.x) && (E1.y < E2.y)).
     * @param[in] E1 je prvý bod na eliptickej krivke.
     * @param[in] E2 je druhý bod na eliptickej krivke.
     * @return Vráti TRUE, ak platí E1 < E2, v opačnom prípade vráti FALSE.
     */
    bool operator()( const epoint* E1, const epoint* E2 ) const;

    /**
     * @brief Hashovaci operator, ktorého vstupom je bod na eliptickej krivke a výstupom hash typu size_t.
     * Funkcia počíta hodnotu hashu na základe vstupného parametra, ktorým je bod na
     * eliptickej krivke.
     * Výstupná hodnota funkcie je závislá na x-ovej afinnej súradnici bodu, presnejšie
     * na jej posledných bitoch - čím sa zabezpečí vyhovujúca efektívnosť hashovania a zároveň dostatočne
     * náhodné rozloženie hodnôt hashov.
     * @param[in] P je bod na eliptickej krivke, ktorého hash počítame.
     * @return Výstupom je hodnota hashu vstupného bodu typu size_t.
     * @todo Do budúcnosti by bolo vhodné zvážiť možnosti, ako počítať hodnotu hashu z bodu na
     * eliptickej krivke. Terajšie riešenie je pre projektívne súradnice zbytočne komplikované.
     */
    size_t operator()( const epoint *P ) const;
};


/**
 * @brief Komparátor bodov na eliptickej krivke.
 * Porovnáva body na eliptickej krivke, vracia TRUE, ak su body identické. Na porovnávanie je
 * využívaná funkciu epoint_comp() obsiahnutá v knižnici miracl.
 * Komparátor je vytvorený podľa požiadaviek TR1 kontajneru <i>unsorted_map</i> ako aj GNUC verzie <i>hash_map</i>.
 */
struct CEpointEq    {
    /**
     * @brief Operátor porovnania dvoch bodov na eliptickej krivke.
     * Funkcia je wrapper pre epoint_comp(E1,E2)
     * @param E1 je prvý bod na eliptickej krivke.
     * @param E2 je druhý bod na eliptickej krivke.
     * @return Vráti TRUE, ak platí E1 == E2, v opačnom prípade vráti FALSE.
     */
    inline bool operator()( const epoint* E1, const epoint* E2 ) const {
        return (epoint_comp( (epoint*)E1, (epoint*)E2) == 1);
    };
};

/**
 * @brief Operátor výstupu do streamu pre premenné typu big.
 * Preťažený globálny operátor výstupu premennej typu <i>big</i> do prúdu.
 * @param[in,out] stream je prúd, do ktorého vypisujeme.
 * @param[in] b je vypisovaná premenná typu <i>big</i>.
 * @return Návratová hodnota je referencia na objekt prúdu, do ktorého zapisujeme - teda referencia na parameter <i>stream</i>.
 */
std::ostream &operator<<(std::ostream &stream, const big b);

/**
 * @brief Operátor výstupu do streamu pre premenné typu epoint*.
 * Preťažený globálny operátor pre výstup premennej typu <i>epoint*</i> do prudu.
 * @param[in,out] stream je prúd, do ktorého vypisujeme.
 * @param[in] B je vypisovaná premenná typu <i>epoint*</i>.
 * @return Návratová hodnota je referencia na objekt prúdu, do ktorého zapisujeme - teda referencia na parameter <i>stream</i>.
 */
std::ostream &operator<<(std::ostream &stream, const epoint * B);

/**
 * @brief Operátor vstupu zo streamu pre premenné typu big.
 * Preťažený globálny operátor pre vstupu premennej typu <i>big</i> z prúdu.
 * @param[in,out] stream je prúd, z ktorého čítame.
 * @param[out] b je alokovaná pamäť, do ktorej zapisujeme dáta z prúdu.
 * @return Návratová hodnota je referencia na objekt prúdu, z ktorého čítame - teda referencia na parameter <i>stream</i>.
 */
std::istream &operator>>(std::istream &stream, big b);

#endif /* MIRACLEEXTENSIONS_HEADER */

