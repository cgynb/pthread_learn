#include <iostream>
#include <cstdlib>
#include <cstring>
#include <stdarg.h>
#include <pthread.h>

#define thread_count 3

template<typename T>
class Matrix{
    private:
        int row, col;
        T** mtx;
    public:
        Matrix(int row, int col){
            this->row = row;
            this->col = col;
            mtx = (T**)malloc(row * sizeof(T*));
            for(int i=0; i<row; i++){
                mtx[i] = (T*)malloc(col * sizeof(T));
                memset(mtx[i], 0, sizeof(mtx[i]));
            }
        }
        ~Matrix(){
            for(int i=0; i<row; i++){
                free(mtx[i]);
            }
            free(mtx);
        }
        int getRow(){
            return row;
        }
        int getCol(){
            return col;
        }
        void setMatrix(T first,  ...){
            T value;
            va_list v;
            va_start(v, first);
            for(int i=0; i<row; i++){
                for(int j=0; j<col; j++){
                    if(i == 0 && j == 0){
                        mtx[i][j] = first;
                    }else{
                        value = va_arg(v, T);
                        mtx[i][j] = value;
                    }
                }
            }
            va_end(v);
        }
        void setMatrix(){
            for(int i=0; i<row; i++){
                for(int j=0; j<col; j++){
                    std::cin >> mtx[i][j];
                }
            }
        }
        void print(){
            for(int i=0; i<row; i++){
                for(int j=0; j<col; j++){
                    std::cout << mtx[i][j] << ' ';
                }
                std::cout << std::endl;
            }
        }
        Matrix<T> operator+(const Matrix& m2){
            Matrix m(this->row, this->col);
            if((this->row == m2.row) && (this->col == m2.col)){
                for(int r=0; r<this->row; r++){
                    for(int c=0; c<this->col; c++){
                        m.mtx[r][c] = this->mtx[r][c] + m2.mtx[r][c];
                    }
                }
            }
            return m;
        }
        Matrix<T> operator-(const Matrix& m2){
            Matrix m(this->row, this->col);
            if((this->row == m2.row) && (this->col == m2.col)){
                for(int r=0; r<this->row; r++){
                    for(int c=0; c<this->col; c++){
                        m.mtx[r][c] = this->mtx[r][c] - m2.mtx[r][c];
                    }
                }
            }
            return m;
        }
        Matrix<T> operator*(const Matrix& m2){
            Matrix<T> m(this->row, m2.col);
            if(this->col == m2.row){
                for(int r=0; r<this->row; r++){
                    for(int c=0; c<m2.col; c++){
                        for(int i=0; i<this->col; i++){
                            m.mtx[r][c] += this->mtx[r][i] * m2.mtx[i][c];
                        }
                    }
                }
            }
            return m;
        }
};



void test(){
    int row, col;
    row = 3;
    col = 2;
    Matrix<int> m1(row, col);
    Matrix<int> m2(row, col);
    m1.setMatrix(2, 3,
                 4, 5,
                 6, 7);
    m2.setMatrix(4, 4,
                 4, 5,
                 6, 7);
    Matrix<int> mAdd = m1+m2;

    Matrix<int> mMinus = mAdd - m1;

    Matrix<int> m3(2, 3), m4(3, 2);
    m3.setMatrix(1, 2, 3,
                 4, 5, 6);
    m4.setMatrix(1, 2,
                 3, 4,
                 5, 6);
    Matrix<int> mMultiplication = m3 * m4;
    mMultiplication.print();
}

int main(){
    // test();
    Matrix<double> m1(5, 3);
    Matrix<double> m2(3, 4);
    m1.setMatrix(
        1.1, 2.2, 3.3,
        4.4, 5.5, 6.6,
        7.7, 8.8, 9.9,
        10.1, 11.11, 12.12,
        13.13, 14.14, 15.15
    );
    m2.setMatrix(
        1.1, 2.2, 3.3, 4.4,
        5.5, 6.6, 7.7, 8.8,
        9.9, 10.1, 11.11, 12.12
    );
    Matrix<double> m = m1*m2;
    m.print();
    return 0;
}