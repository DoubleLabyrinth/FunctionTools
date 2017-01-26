#ifndef TYPEDEFINE_H_INCLUDED
#define TYPEDEFINE_H_INCLUDED

typedef char INT8;
typedef unsigned char BYTE;
typedef short INT16;
typedef unsigned short UINT16;
typedef int INT32;
typedef unsigned int UINT32;
typedef long long INT64;
typedef unsigned long long UINT64;

class UINT128 {
public:
    UINT64 L64 = 0;
    UINT64 H64 = 0;

    UINT128();

    UINT128(const char& x) {
        L64 = (UINT64)x;
        H64 = x >= 0 ? 0 : 0xFFFFFFFFFFFFFFFF;
    }

    UINT128(const BYTE& x) {
        L64 = x;
    }

    UINT128(const INT16& x) {
        L64 = x;
        H64 = x >= 0 ? 0 : 0xFFFFFFFFFFFFFFFF;
    }

    UINT128(const UINT16& x) {
        L64 = x;
    }

    UINT128(const INT32& x) {
        L64 = x;
        H64 = x >= 0 ? 0 : 0xFFFFFFFFFFFFFFFF;
    }

    UINT128(const UINT32& x) {
        L64 = x;
    }

    UINT128(const INT64& x) {
        L64 = x;
        H64 = x >= 0 ? 0 : 0xFFFFFFFFFFFFFFFF;
    }

    UINT128(const UINT64& x) {
        L64 = x;
    }

    UINT128(const UINT64& Lx, const INT64& Hx) {
        L64 = Lx;
        H64 = Hx;
    }

    UINT128(UINT64& Lx, UINT64& Hx) {
        L64 = Lx;
        H64 = Hx;
    }

    UINT128(const UINT128& x) {
        L64 = x.L64;
        H64 = x.H64;
    }

    UINT128 operator~() {
        return UINT128(~L64, ~H64);
    }


    UINT128 operator+(const UINT128& x) {
        return UINT128(x.L64 + L64, L64 + x.L64 < L64 ? H64 + x.H64 + 1 : H64 + x.H64);
    }

    UINT128& operator+=(const UINT128& x) {
        H64 += (L64 + x.L64 < L64) ? x.H64 + 1 : x.H64;
        L64 += L64;
        return *this;
    }

    UINT128 operator-(const UINT128& x) {
        UINT128 temp = x;
        temp.H64 = ~(temp.H64);
        temp.L64 = ~(temp.L64);
        temp += 1;
        temp += *this;
        return temp;
    }

    UINT128& operator-=(const UINT128& x) {
        UINT128 temp = x;
        temp.H64 = ~(temp.H64);
        temp.L64 = ~(temp.L64);
        temp += 1;
        *this += temp;
        return *this;
    }

    UINT128 operator>>(BYTE s) {
        return UINT128(L64 >> s | H64 << (64 - s), H64 >> s);
    }

    UINT128 operator>>=(BYTE s) {
        L64 = L64 >> s | H64 << (64 - s);
        H64 >>= s;
        return *this;
    }

    UINT128 operator<<(BYTE s) {
        return UINT128(L64 << s, H64 << s | L64 >> (64 - s));
    }

    UINT128& operator<<=(BYTE s) {
        H64 = H64 << s | L64 >> (64 - s);
        L64 <<= s;
        return *this;
    }

    UINT128 operator&(const UINT128& x) {
        return UINT128(L64 & x.L64, H64 & x.H64);
    }

    UINT128 operator^(const UINT128& x) {
        return UINT128(L64 ^ x.L64, H64 ^ x.H64);
    }

    bool operator<(const UINT128& x) {
        if(H64 < x.H64) return true;
        else if(H64 > x.H64) return false;
        if(L64 < x.L64) return true;
        else return false;
    }

    bool operator<=(const UINT128& x) {
        if(H64 < x.H64) return true;
        else if(H64 > x.H64) return false;
        if(L64 <= x.L64) return true;
        else return false;
    }

    bool operator>(const UINT128& x) {
        if(H64 > x.H64) return true;
        else if(H64 < x.H64) return false;
        if(L64 > x.L64) return true;
        else return false;
    }

    bool operator>=(const UINT128& x) {
        if(H64 > x.H64) return true;
        else if(H64 < x.H64) return false;
        if(L64 >= x.L64) return true;
        else return false;
    }

    UINT128& operator++() {
        if(L64 + 1 < L64) H64++;
        L64++;
        return *this;
    }

    UINT128& operator--() {
        if(L64 - 1 > L64) H64--;
        L64--;
        return *this;
    }
};

#endif //TYPEDEFINE_H_INCLUDED
