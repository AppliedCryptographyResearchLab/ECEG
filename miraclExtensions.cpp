/**
 * @file miraclExtensions.cpp.
 *
 * @author Martin Kosorinský.
 * @date 1.4.2009.
 */

#include "miraclExtensions.hpp"

bool CEpointCmp::operator()( const epoint* E1, const epoint* E2 ) const {
    big x1 = mirvar(0);
    big x2 = mirvar(0);
    big y1 = mirvar(0);
    big y2 = mirvar(0);
    epoint_get( (epoint*) E1, x1, y1);
    epoint_get( (epoint*) E2, x2, y2);
    bool vysledok = ((mr_compare(x1,x2)==-1) || ((mr_compare(x1,x2)==0) && (mr_compare(y1,y2)==-1)));
    mirkill(x1);
    mirkill(x2);
    mirkill(y1);
    mirkill(y2);
    return vysledok;
}

size_t CEpointCmp::operator()( const epoint *P ) const  {
    big x = mirvar(0);
    epoint_get( (epoint*)P, x ,x);
        size_t vysledok = (size_t) remain(x,1048576);
        mirkill(x);
    return vysledok;
}

std::ostream &operator<<(std::ostream &stream, const big b)     {
    miracl *mip=get_mip();
    cotstr(b,mip->IOBUFF);
    stream << mip->IOBUFF;
    return stream;
}

std::istream &operator>>(std::istream &stream, big b)   {
    miracl *mip = get_mip();
	std::cout << "Base" << mip->IOBASE << std::endl;
    if (mip->IOBASE > 60) {
        stream.sync();
        stream.getline(mip->IOBUFF, mip->IOBSIZ);
    } else      {
        stream >> mip->IOBUFF;
    }
    if (stream.eof() || stream.bad()) {
        zero(b);
        return stream;
    }
    cinstr(b,mip->IOBUFF);
    return stream;
}

std::ostream &operator<<(std::ostream &stream, const epoint * B)        {
    if ( point_at_infinity( (epoint*)B) )
        stream << "(infinum)";
    else        {
        big x = mirvar(0);
        big y = mirvar(0);
        epoint_get( (epoint*)B,x,y);
        stream << "(" << x << "," << y << ")";
        mirkill(x);
        mirkill(y);
    }
    return stream;
}

