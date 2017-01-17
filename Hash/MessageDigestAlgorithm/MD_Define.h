#ifndef MD_DEFINE_H
#define MD_DEFINE_H
#include "../../TypeDefine.h"

typedef struct _HashResult {
    UINT32 A;
    UINT32 B;
    UINT32 C;
    UINT32 D;
} HashResult, LPHashResult;
#define UINT32_SHL(X, Y) X << Y | X >> (32 - Y)

#define MD5_F(X, Y, Z) ((X & Y) | (~X & Z))
#define MD5_G(X, Y, Z) ((X & Z) | (Y & ~Z))
#define MD5_H(X, Y, Z) (X ^ Y ^ Z)
#define MD5_I(X, Y, Z) (Y ^ (X | ~Z))

#define MD5_FF(A, B, C, D, K, s, T) \
    A += MD5_F(B, C, D) + K + T;\
    A = UINT32_SHL(A, s);\
    A += B;

#define MD5_GG(A, B, C, D, K, s, T) \
    A += MD5_G(B, C, D) + K + T;\
    A = UINT32_SHL(A, s);\
    A += B;

#define MD5_HH(A, B, C, D, K, s, T) \
    A += MD5_H(B, C, D) + K + T;\
    A = UINT32_SHL(A, s);\
    A += B;

#define MD5_II(A, B, C, D, K, s, T) \
    A += MD5_I(B, C, D) + K + T;\
    A = UINT32_SHL(A, s);\
    A += B;

#define MD4_F(X, Y, Z) ((X & Y) | (~X & Z))
#define MD4_G(X, Y, Z) ((X & Y) | (X & Z) | (Y & Z))
#define MD4_H(X, Y, Z) (X ^ Y ^ Z)

#define MD4_FF(A, B, C, D, K, s)\
    A += MD4_F(B, C, D) + K;\
    A = UINT32_SHL(A, s);

#define MD4_GG(A, B, C, D, K, s)\
    A += MD4_G(B, C, D) + K + 0x5A827999;\
    A = UINT32_SHL(A, s);

#define MD4_HH(A, B, C, D, K, s)\
    A += MD4_H(B, C, D) + K + 0x6ED9EBA1;\
    A = UINT32_SHL(A, s);

#endif //MD_DEFINE_H
