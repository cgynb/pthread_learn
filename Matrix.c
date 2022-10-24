#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdarg.h>

int thread_count = 3;

typedef struct{
    int row;
    int col;
    double** m;
}Matrix;

struct params{
    int tid;
    Matrix* res;
    Matrix* m1;
    Matrix* m2;
};

void matrix_init(Matrix* m, int row, int col){
    m->row = row;
    m->col = col;
    m->m = (double**)malloc(m->row * sizeof(double*));
    for(int r=0; r<m->row; r++){
        m->m[r] = (double*)calloc(m->col, sizeof(double));
    }
}

void matrix_set(Matrix* m, ...){
    va_list v;
    va_start(v, m);
    double value;
    for(int r=0; r<m->row; r++){
        for(int c=0; c<m->col; c++){
            value = va_arg(v, double);
            m->m[r][c] = value;
        }
    }
    va_end(v);
}

void matrix_print(Matrix* m){
    for(int r=0; r<m->row; r++){
        for(int c=0; c<m->col; c++){
            printf("%.3lf ", m->m[r][c]);
        }
        printf("\n");
    }
}

void matrix_destroy(Matrix* m){
    for(int r=0; r<m->row; r++){
        free(m->m[r]);
    }
    free(m->m);
}

void matrix_plus(Matrix* res, Matrix* m1, Matrix* m2){
    if(res->row == m1->row && res->col == m1->col && res->row == m2->row && res->col == m2->col){
        for(int r=0; r < res->row; r++){
            for(int c=0; c < res->col; c++){
                res->m[r][c] = m1->m[r][c] + m2->m[r][c];
            }
        }
    }
}

void matrix_minus(Matrix* res, Matrix* m1, Matrix* m2){
    if(res->row == m1->row && res->col == m1->col && res->row == m2->row && res->col == m2->col){
        for(int r=0; r < res->row; r++){
            for(int c=0; c < res->col; c++){
                res->m[r][c] = m1->m[r][c] - m2->m[r][c];
            }
        }
    }
}

void matrix_multiply(Matrix* res, Matrix* m1, Matrix* m2){
    if(m1->col == m2->row && res->row == m1->row && res->col == m2->col){
        for(int r=0; r < res->row; r++){
            for(int c=0; c < res->col; c++){
                res->m[r][c] = 0;
                for(int i=0; i< m1->col; i++){
                    res->m[r][c] += m1->m[r][i] * m2->m[i][c];
                }
            }
        }
    }
}

int main(){
    Matrix m1, m2, m3, m_plus, m_minus, m_multiply;
    matrix_init(&m1, 5, 3);
    matrix_init(&m2, 5, 3);
    matrix_init(&m3, 3, 4);
    matrix_init(&m_plus, 5, 3);
    matrix_init(&m_minus, 5, 3);
    matrix_init(&m_multiply, 5, 4);

    matrix_set(&m1, 1.1, 2.2, 3.3,
                    4.4, 5.5, 6.6,
                    7.7, 8.8, 9.9,
                    10.1, 11.11, 12.12,
                    13.13, 14.14, 15.15);
    matrix_set(&m2, 1.1, 2.2, 3.3,
                    4.4, 5.5, 6.6,
                    7.7, 8.8, 9.9,
                    10.1, 11.11, 12.12,
                    13.13, 14.14, 15.15);
    matrix_set(&m3, 1.1, 2.2, 3.3, 4.4,
                    5.5, 6.6, 7.7, 8.8,
                    9.9, 10.1, 11.11, 12.12);
    

    // plus
    matrix_plus(&m_plus, &m1, &m2);
    printf("m_plus:\n");
    matrix_print(&m_plus);

    // minus
    matrix_minus(&m_minus, &m_minus, &m2);
    printf("m_minus:\n");
    matrix_print(&m_minus);

    // multiply
    matrix_multiply(&m_multiply, &m1, &m3);
    printf("m_multiply:\n");
    matrix_print(&m_multiply);

    matrix_destroy(&m1);
    matrix_destroy(&m2);
    matrix_destroy(&m3);
    matrix_destroy(&m_plus);
    matrix_destroy(&m_minus);
    matrix_destroy(&m_multiply);
}