#ifndef MATH_MATRIX_H_INCLUDED
#define MATH_MATRIX_H_INCLUDED
#include "../../TypeDefine.h"
#include <memory>

template<typename _Tp>
class Matrix {
private:
    UINT32 RowCount;
    UINT32 ColumnCount;
    _Tp** Data = nullptr;

public:
    Matrix() {
        Data = nullptr;
        RowCount = ColumnCount = 0;
    }

    Matrix(UINT32 srcRowCount, UINT32 srcColumnCount) {
        if(srcRowCount < 1 || srcColumnCount < 1) {
            Data = nullptr;
            RowCount = ColumnCount = 0;
            return;
        }

        RowCount = srcRowCount;
        ColumnCount = srcColumnCount;
        Data = new _Tp*[RowCount];
        for(UINT32 i = 0; i < RowCount; i++) {
            Data[i] = new _Tp[ColumnCount]();
        }
    }

    Matrix(const Matrix<_Tp>& srcMatrix) {
        if(srcMatrix.RowCount < 1 || srcMatrix.ColumnCount < 1) {
            Data = nullptr;
            RowCount = ColumnCount = 0;
            return;
        }

        RowCount = srcMatrix.RowCount;
        ColumnCount = srcMatrix.ColumnCount;

        Data = new _Tp[RowCount][ColumnCount];
        memcpy(Data, srcMatrix.Data, RowCount * ColumnCount);
    }

    ~Matrix() {
        if(Data == nullptr) return;
        for(UINT32 i = 0; i < RowCount; i++) {
            delete[] Data[i];
        }
        delete[] Data;
    }

    bool SetElement(_Tp& srcData, UINT32 srcRowIndex, UINT32 srcColumnIndex) {
        if(srcRowIndex >= 0 && srcRowIndex < RowCount && srcColumnIndex >= 0 && srcColumnIndex < ColumnCount) {
            Data[srcRowIndex][srcRowIndex] = srcData;
            return true;
        }
        return false;
    }

    _Tp GetElement(UINT32 srcRowIndex, UINT32 srcColumnIndex) const {
        if(srcRowIndex >= 0 && srcRowIndex < RowCount && srcColumnIndex >= 0 && srcColumnIndex < ColumnCount) {
            return Data[srcRowIndex][srcColumnIndex];
        }
        throw "Index out of Bounds.";
    }

    UINT32 GetRowCount() const { return RowCount; }
    UINT32 GetColumnCount() const { return ColumnCount; }

    Matrix<_Tp> operator+(const Matrix<_Tp>& srcMatrix) {
        if(RowCount == srcMatrix.RowCount && ColumnCount == srcMatrix.ColumnCount) {
            Matrix<_Tp> ret(RowCount, ColumnCount);
            for(UINT32 i = 0; i < RowCount; i++) {
                for(UINT32 j = 0; j < ColumnCount; j++) {
                    ret.Data[i][j] = Data[i][j] + srcMatrix.Data[i][j];
                }
            }
            return ret;
        }
        throw "Matrix doesn't match.";
    }

    Matrix<_Tp> operator-(const Matrix<_Tp>& srcMatrix) {
        if(RowCount == srcMatrix.RowCount && ColumnCount == srcMatrix.ColumnCount) {
            Matrix<_Tp> ret(RowCount, ColumnCount);
            for(UINT32 i = 0; i < RowCount; i++) {
                for(UINT32 j = 0; j < ColumnCount; j++) {
                    ret.Data[i][j] = Data[i][j] - srcMatrix.Data[i][j];
                }
            }
            return ret;
        }
        throw "Matrix doesn't match.";
    }

    Matrix<_Tp> operator*(const Matrix<_Tp>& srcMatrix) {
        if(ColumnCount == srcMatrix.RowCount) {
            Matrix<_Tp> ret(RowCount, srcMatrix.ColumnCount);
            for(UINT32 i = 0; i < RowCount; i++) {
                for(UINT32 j = 0; j < srcMatrix.ColumnCount; j++) {
                    for(UINT32 k = 0; k < ColumnCount; k++) {
                        ret.Data[i][j] += Data[i][k] * srcMatrix.Data[k][j];
                    }
                }
            }
            return ret;
        }
        throw "Matrix doesn't match.";
    }

    template<typename _T>
    friend Matrix<_T> operator*(const _T& k, const Matrix<_T>& srcMatrix);
    template<typename _T>
    friend Matrix<_T> operator*(const Matrix<_T>& srcMatrix, const _T& k);

    Matrix<_Tp> operator/(const _Tp& k) {
        Matrix<_Tp> ret(RowCount, ColumnCount);
        for(UINT32 i = 0; i < RowCount; i++) {
            for(UINT32 j = 0; j < ColumnCount; j++) {
                ret.Data[i][j] = Data[i][j] / k;
            }
        }
        return ret;
    }

    Matrix<_Tp>& operator=(const Matrix<_Tp>& srcMatrix) {
        if(RowCount == srcMatrix.RowCount && ColumnCount == srcMatrix.ColumnCount) {
            for(UINT32 i = 0; i < RowCount; i++) {
                for(UINT32 j = 0; j < ColumnCount; j++) {
                    Data[i][j] = srcMatrix.Data[i][j];
                }
            }
            return *this;
        }
        throw "Matrix doesn't match.";
    }

    Matrix<_Tp>& operator+=(const Matrix<_Tp>& srcMatrix) {
        if(RowCount == srcMatrix.RowCount && ColumnCount == srcMatrix.ColumnCount) {
            for(UINT32 i = 0; i < RowCount; i++) {
                for(UINT32 j = 0; j < ColumnCount; j++) {
                    Data[i][j] += srcMatrix.Data[i][j];
                }
            }
            return *this;
        }
        throw "Matrix doesn't match.";
    }

    Matrix<_Tp>& operator-=(const Matrix<_Tp>& srcMatrix) {
        if(RowCount == srcMatrix.RowCount && ColumnCount == srcMatrix.ColumnCount) {
            for(UINT32 i = 0; i < RowCount; i++) {
                for(UINT32 j = 0; j < ColumnCount; j++) {
                    Data[i][j] -= srcMatrix.Data[i][j];
                }
            }
            return *this;
        }
        throw "Matrix doesn't match.";
    }

    Matrix<_Tp>& operator*=(const Matrix<_Tp>& srcMatrix) {
        if(ColumnCount == srcMatrix.RowCount) {
            Matrix temp(RowCount, srcMatrix.ColumnCount);
            for(UINT32 i = 0; i < RowCount; i++) {
                for(UINT32 j = 0; j < srcMatrix.ColumnCount; j++) {
                    for(UINT32 k = 0; k < ColumnCount; k++) {
                        temp.Data[i][j] += Data[i][k] * srcMatrix.Data[k][j];
                    }
                }
            }
            *this = temp;
            return *this;
        }
        throw "Matrix doesn't match.";
    }

    Matrix<_Tp>& operator*=(const _Tp& k) {
        for(UINT32 i = 0; i < RowCount; i++) {
            for(UINT32 j = 0; j < ColumnCount; j++) {
                Data[i][j] *= k;
            }
        }
        return *this;
    }

    Matrix<_Tp>& operator/=(const _Tp& k) {
        for(UINT32 i = 0; i < RowCount; i++) {
            for(UINT32 j = 0; j < ColumnCount; j++) {
                Data[i][j] /= k;
            }
        }
        return *this;
    }

    Matrix<_Tp> GetCofactorMatrix(UINT32 srcRowIndex, UINT32 srcColumnIndex) const {
        if(RowCount == 1 || ColumnCount == 1) return Matrix();

        Matrix<_Tp> ret(RowCount - 1, ColumnCount - 1);
        for(UINT32 i = 0; i < RowCount; i++) {
            for(UINT32 j = 0; j < ColumnCount; j++) {
                if(i < srcRowIndex && j < srcColumnIndex) {ret.Data[i][j] = Data[i][j]; continue;}
                if(i < srcRowIndex && j > srcColumnIndex) {ret.Data[i][j - 1] = Data[i][j]; continue;}
                if(i > srcRowIndex && j < srcColumnIndex) {ret.Data[i - 1][j] = Data[i][j]; continue;}
                if(i > srcRowIndex && j > srcColumnIndex) {ret.Data[i - 1][j - 1] = Data[i][j]; continue;}
            }
        }
        return ret;
    }

    _Tp GetDeterminantValue() {
        if(ColumnCount != RowCount) throw "Matrix is not square matrix.";

        /*long double ret = 0.0;
        if(srcMatrix.ColumnCount == 2) {
            return srcMatrix.Data[0][0] * srcMatrix.Data[1][1] - srcMatrix.Data[0][1] * srcMatrix.Data[1][0];
        } else if (srcMatrix.ColumnCount == 1) {
            return srcMatrix.Data[0][0];
        } else {
            for(int j = 1; j <= srcMatrix.ColumnCount; j++) {
                Matrix tmp(srcMatrix.GetCofactorMatrix(1, j));
                ret += ((1 + j) % 2 == 0 ? 1 : -1) * srcMatrix.Data[0][j - 1] * GetDeterminantValue(tmp);
            }
            return ret;
        }*/

        Matrix<_Tp> tmp = GetGaussEliminationMatrix_Safe(*this);
        _Tp ret = tmp.Data[0][0];
        for(UINT32 i = 1; i < tmp.RowCount; i++) {
            ret *= tmp.Data[i][i];
        }
        return ret;
    }

    Matrix<_Tp> Transpose() {
        if(ColumnCount == 1 && RowCount == 1) {
            return *this;
        } else {
            Matrix<_Tp> ret(ColumnCount, RowCount);
            for(UINT32 i = 0; i < RowCount; i++) {
                for(UINT32 j = 0; j < ColumnCount; j++) {
                    ret.Data[j][i] = Data[i][j];
                }
            }
            return ret;
        }
    }

    Matrix<_Tp> GetAdjugateMatrix() {   // Is it effective?
        if(RowCount != ColumnCount) throw "Matrix is not square matrix.";

        Matrix<_Tp> ret(RowCount, ColumnCount);
        for(UINT32 i = 0; i < RowCount; i++) {
            for(UINT32 j = 0; j < ColumnCount; j++) {
                ret.Data[j][i] = GetDeterminantValue(GetCofactorMatrix(i, j)) * ((i + j) & 0x01 == 0 ? 1 : -1);
            }
        }
        return ret;
    }

    Matrix<_Tp> GetInverseMatrix() {
        if(RowCount != ColumnCount) throw "Matrix is not square matrix.";
        return ret(GetAdjugateMatrix() / GetDeterminantValue());
    }

    Matrix GetGaussEliminationMatrix() {
        Matrix ret(*this);
        if(ret.RowCount == 1) return ret;

        for(UINT32 j = 0; j < ColumnCount; j++) {
            UINT32 NotZeroPointer = j;
            for(; NotZeroPointer < RowCount; NotZeroPointer++) {
                if(ret.Data[NotZeroPointer][j] != 0) {
                    break;
                } else if(ret.Data[NotZeroPointer][j] == 0 && NotZeroPointer == RowCount - 1) {
                    return ret;
                }
            }

            if(NotZeroPointer != j) ret.SwapRow(NotZeroPointer, j);

            for(int i = 0; i < RowCount; i++) {
                if(i == j) continue;
                ret.RowSubtraction_Multiply(j, i, ret.Data[i][j] / ret.Data[j][j]);
            }
        }
        return ret;
    }

    //The first kind of matrix transformation
    void SwapRow(UINT32 srcRowIndex1, UINT32 srcRowIndex2) {
        if(0 <= srcRowIndex1 && srcRowIndex1 < RowCount && 0 <= srcRowIndex2 && srcRowIndex2 < RowCount){
            for(UINT32 j = 0; j < ColumnCount; j++) {
                _Tp tmp = Data[srcRowIndex1][j];
                Data[srcRowIndex1][j] = Data[srcRowIndex2][j];
                Data[srcRowIndex1][j] = tmp;
            }
        }
    }

    void SwapColumn(UINT32 srcColumnIndex1, UINT32 srcColumnIndex2) {
        if(0 <= srcColumnIndex1 && srcColumnIndex1 < ColumnCount && 0 <= srcColumnIndex2 && srcColumnIndex2 < ColumnCount) {
            for(UINT32 i = 0; i < RowCount; i++) {
                _Tp tmp = Data[i][srcColumnIndex1];
                Data[i][srcColumnIndex1] = Data[i][srcColumnIndex2];
                Data[i][srcColumnIndex2] = tmp;
            }
        }
    }

    //The second kind of matrix transformation
    void RowMultiply(UINT32 srcRowIndex, const _Tp& k) {
        if(0 <= srcRowIndex && srcRowIndex < RowCount) {
            for(UINT32 j = 0; j < ColumnCount; j++) {
                Data[srcRowIndex][j] *= k;
            }
        }
    }

    void ColumnMultiply(UINT32 srcColumnIndex, const _Tp& k) {
        if(0 <= srcColumnIndex && srcColumnIndex < ColumnCount) {
            for(int i = 0; i < RowCount; i++) {
                Data[i][srcColumnIndex] *= k;
            }
        }
    }

    //The third kind of matrix transformation
    void RowAddition_Multiply(UINT32 srcRowIndex, UINT32 dstRowIndex, const _Tp& k) {
        if(0 <= srcRowIndex && srcRowIndex < RowCount && 0 <= dstRowIndex && dstRowIndex < RowCount) {
            for(UINT32 j = 0; j < ColumnCount; j++) {
                Data[dstRowIndex][j] += Data[srcRowIndex][j] * k;
            }
        }
    }

    void ColumnAddition_Multiply(UINT32 srcColumnIndex, UINT32 dstColumnIndex, const _Tp& k) {
        if(0 <= srcColumnIndex && srcColumnIndex < RowCount && 0 <= dstColumnIndex && dstColumnIndex < RowCount) {
            for(UINT32 i = 0; i < RowCount; i++) {
                Data[i][dstColumnIndex] += Data[i][srcColumnIndex] * k;
            }
        }
    }

    void RowSubtraction_Multiply(UINT32 srcRowIndex, UINT32 dstRowIndex, const _Tp& k) {
        if(0 <= srcRowIndex && srcRowIndex < RowCount && 0 <= dstRowIndex && dstRowIndex < RowCount) {
            for(UINT32 j = 0; j < ColumnCount; j++) {
                Data[dstRowIndex][j] -= Data[srcRowIndex][j] * k;
            }
        }
    }

    void ColumnSubtraction_Multiply(UINT32 srcColumnIndex, UINT32 dstColumnIndex, const _Tp& k) {
        if(0 <= srcColumnIndex && srcColumnIndex < RowCount && 0 <= dstColumnIndex && dstColumnIndex < RowCount) {
            for(UINT32 i = 0; i < RowCount; i++) {
                Data[i][dstColumnIndex] -= Data[i][srcColumnIndex] * k;
            }
        }

    }
};

template<typename _T>
Matrix<_T> operator*(const _T& k, const Matrix<_T>& srcMatrix) {
    Matrix<_T> ret(srcMatrix.RowCount, srcMatrix.ColumnCount);
    for(UINT32 i = 0; i < srcMatrix.RowCount; i++) {
        for(UINT32 j = 0; j < srcMatrix.ColumnCount; j++) {
            ret.Data[i][j] = k * srcMatrix.Data[i][j];
        }
    }
    return ret;
}

template<typename _T>
Matrix<_T> operator*(const Matrix<_T>& srcMatrix, const _T& k) {
    return operator*(srcMatrix, k);
}

#endif // MATH_MATRIX_H_INCLUDED
