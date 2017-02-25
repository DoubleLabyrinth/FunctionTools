#ifndef FOURIER_TRANS_H_INCLUDED
#define FOURIER_TRANS_H_INCLUDED
#include <complex>
#include <iostream>
#include "../../TypeDefine.h"
#include "../../Math/Matrix/Matrix.h"

namespace Audio {
    namespace Algorithm {
        /**
         * NOTICE: Function DFT is strictly based DFT definition. No optimization is used.
         */
        template<typename _Tp>
        bool DFT(_Tp* srcData, UINT32 srcDataLength, std::complex<long double>* dstData) {
            /*switch(typeid(_Tp1)) {
                case typeid(char):
                case typeid(unsigned char):
                case typeid(short):
                case typeid(unsigned short):
                case typeid(int):
                case typeid(unsigned int):
                case typeid(long):
                case typeid(unsigned long):
                case typeid(long long):
                case typeid(unsigned long long):
                case typeid(float):
                case typeid(double):
                case typeid(long double):
                    break;
                default:
                    return false;
            }*/
            const long double PI = 3.14159265358979323846L;
            std::complex<long double> Wn(0, -2 * PI / srcDataLength);
            Matrix<std::complex<long double>> DFTMatrix(srcDataLength, srcDataLength);

            for(UINT32 i = 0; i < srcDataLength; i++) {
                for(UINT32 j = i; j < srcDataLength; j++) {
                    std::complex<long double> tmp = std::exp(Wn * (long double)i * (long double)j);
                    if(!DFTMatrix.SetElement(tmp, i, j)) std::cout << "i, j ---------Failed.\n";
                    if(!DFTMatrix.SetElement(tmp, j, i)) std::cout << "j, i ---------Failed.\n";
                }
            }

            Matrix<std::complex<long double>> Result(srcDataLength, 1);
            std::complex<long double> tmp2(0, 0);
            for(UINT32 i = 0; i < srcDataLength; i++) {
                tmp2.real(srcData[i]);
                if(!Result.SetElement(tmp2, i, 0)) std::cout << "Result: i, 0 -------Failed.\n";
            }
\
            Result = DFTMatrix * Result;

            for(UINT32 i = 0; i < srcDataLength; i++) {
                dstData[i] = Result.GetElement(i, 0);
            }
\
            return true;
        }
    }
}

#endif //FOURIER_TRANS_H_INCLUDED
