#ifndef COMPLEX_H_INCLUDED
#define COMPLEX_H_INCLUDED
#include "../TypeDefine.h"

namespace Math {
    template<typename _Tp>
    class Complex {
    public:
        _Tp Real;
        _Tp Image;

        Complex() {
            Real = _Tp(0);
            Image = _Tp(0);
        }

        Complex(const _Tp& srcReal, const _Tp& srcImage) {
            Real = srcReal;
            Image = srcImage;
        }

        Complex(const _Tp& srcReal) {
            Real = srcReal;
            Image = (_Tp)0;
        }

        Complex(const Complex<_Tp>& srcComplex) {
            Real = srcComplex.Real;
            Image = srcComplex.Image;
        }

        Complex<_Tp> operator+(const Complex<_Tp>& srcComplex) {
            return Complex(Real + srcComplex.Real, Image + srcComplex.Image);
        }

        Complex<_Tp> operator+(const _Tp& srcReal) {
            return Complex(Real + srcReal, Image);
        }

        Complex<_Tp> operator-(const Complex<_Tp>& srcComplex) {
            return Complex(Real - srcComplex.Real, Image - srcComplex.Image);
        }

        Complex<_Tp> operator-(const _Tp& srcReal) {
            return Complex(Real - srcReal, Image);
        }

        Complex<_Tp> operator*(const Complex<_Tp>& srcComplex) {
            return Complex(Real * srcComplex.Real - Image * srcComplex.Image,
                           Real * srcComplex.Image + Image * srcComplex.Real);
        }

        Complex<_Tp> operator*(const _Tp& srcReal) {
            return Complex(Real * srcReal, Image * srcReal);
        }

        template<typename _T>
        friend Complex<_T> operator*(const _T& srcReal, const Complex<_T>& srcComplex);

        Complex<_Tp> operator/(const Complex<_Tp>& srcComplex) {
            _Tp tmp = srcComplex.Real * srcComplex.Real + srcComplex.Image * srcComplex.Image;
            return Complex((Real * srcComplex.Real + Image * srcComplex.Image) / tmp, (Image * srcComplex.Real - Real * srcComplex.Image) / tmp);
        }

        Complex<_Tp> operator/(const _Tp& srcReal) {
            return Complex(Real / srcReal, Image / srcReal);
        }

        template<typename _T>
        friend Complex<_T> operator/(const _T& srcReal, const Complex<_T>& srcComplex);

        Complex<_Tp>& operator+=(const Complex<_Tp>& srcComplex) {
            Real += srcComplex.Real;
            Image += srcComplex.Image;
            return *this;
        }

        Complex<_Tp>& operator+=(const _Tp& srcReal) {
            Real += srcReal;
            return *this;
        }

        Complex<_Tp>& operator-=(const Complex<_Tp>& srcComplex) {
            Real -= srcComplex.Real;
            Image -= srcComplex.Image;
            return *this;
        }

        Complex<_Tp>& operator-=(const _Tp& srcReal) {
            Real -= srcReal;
            return *this;
        }

        Complex<_Tp>& operator*=(const Complex<_Tp>& srcComplex) {
            _Tp tmp = Real * srcComplex.Real - Image * srcComplex.Image;
            Image = Real * srcComplex.Image + Image * srcComplex.Real;
            Real = tmp;
            return *this;
        }

        Complex<_Tp>& operator*=(const _Tp& srcReal) {
            Real *= srcReal;
            Image *= srcReal;
            return *this;
        }

        Complex<_Tp>& operator/=(const Complex<_Tp>& srcComplex) {
            _Tp tmp = srcComplex.Real * srcComplex.Real + srcComplex.Image * srcComplex.Image;
            _Tp tmp2 = (Real * srcComplex.Real + Image * srcComplex.Image) / tmp;
            Image = (Image * srcComplex.Real - Real * srcComplex.Image) / tmp;
            Real = tmp2;
            return *this;
        }

        Complex<_Tp>& operator/=(const _Tp& srcReal) {
            Real /= srcReal;
            Image /= srcReal;
            return *this;
        }

        bool operator==(const Complex<_Tp>& srcComplex) {
            if(srcComplex.Real == Real && srcComplex.Image == Image) return true;
            return false;
        }

        bool operator==(const _Tp& srcReal) {
            if(Image != (_Tp)0) return false;
            if(srcReal == Real) return true;
            return false;
        }

        bool operator!=(const Complex<_Tp>& srcComplex) {
            if(srcComplex.Real != Real || srcComplex.Image != Image) return true;
            return false;
        }

        bool operator!=(const _Tp& srcReal) {
            if(Image != (_Tp)0) return true;
            if(srcReal == Real) return false;
            return true;
        }

        Complex<_Tp> operator*() {
            return Complex(Real, (_Tp)0 - Image);
        }

        /**
         * TODO:
         * GetLength();
         * GetArg();
         */
    };

    template<typename _T>
    Complex<_T> operator*(const _T& srcReal, const Complex<_T>& srcComplex) {
        return Complex<_T>(srcReal * srcComplex.Real, srcReal * srcComplex.Image);
    }

    template<typename _T>
    Complex<_T> operator/(const _T& srcReal, const Complex<_T>& srcComplex) {
        _T tmp = srcComplex.Real * srcComplex.Real + srcComplex.Image * srcComplex.Image;
        return Complex<_T>((srcReal * srcComplex.Real) / tmp, (_T)0 - (srcReal * srcComplex.Image) /tmp);
    }
}


#endif //COMPLEX_H_INCLUDED
