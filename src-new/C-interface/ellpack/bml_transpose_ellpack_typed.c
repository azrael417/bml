#include "../typed.h"
#include "bml_allocate.h"
#include "bml_transpose.h"
#include "bml_types.h"
#include "bml_allocate_ellpack.h"
#include "bml_transpose_ellpack.h"
#include "bml_types_ellpack.h"

#include <complex.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#ifdef _OPENMP
#include <omp.h>
#endif

/** Transpose a matrix.
 *
 *  \ingroup transpose_group
 *
 *  \param A The matrix to be transposeed
 *  \return the transposeed A
 */
bml_matrix_ellpack_t *TYPED_FUNC(
    bml_transpose_new_ellpack) (
    const bml_matrix_ellpack_t * A)
{
    int N = A->N;
    int M = A->M;

    bml_matrix_ellpack_t *B = TYPED_FUNC(bml_zero_matrix_ellpack) (N, M);

    REAL_T *A_value = (REAL_T *) A->value;
    int *A_index = A->index;
    int *A_nnz = A->nnz;

    REAL_T *B_value = (REAL_T *) B->value;
    int *B_index = B->index;
    int *B_nnz = B->nnz;

    // First move diagonal elements over
    #pragma omp parallel for shared(N,M,B_index,B_value,B_nnz)
    for (int i = 0; i < N; i++)
    {
        B_index[i * M] = i;
        B_value[i * M] = A_value[i * M]; 
        B_nnz[i] = 1;
    }

    // Transpose off-diagonal elements
    #pragma omp parallel for shared(N,M,B_index,B_value,B_nnz)
    for (int i = 0; i < N; i++)
    {
        for (int j = 1; j < A_nnz[i]; j++)
        {
            int trow = A_index[i * M + j];
            #pragma omp critical
            {
                int colcnt = B_nnz[trow];
                B_index[trow * M + colcnt] = i;
                B_value[trow * M + colcnt] = A_value[i * M + j];
                B_nnz[trow]++;
            }
        }
    }

    return B;
}

/** Transpose a matrix in place.
 *
 *  \ingroup transpose_group
 *
 *  \param A The matrix to be transposeed
 *  \return the transposeed A
 */
void TYPED_FUNC(
    bml_transpose_ellpack) (
    const bml_matrix_ellpack_t * A)
{
    int N = A->N;
    int M = A->M;

    REAL_T *A_value = (REAL_T *) A->value;
    int *A_index = A->index;
    int *A_nnz = A->nnz;

    // Transpose off-diagonal elements
    // No need to move diagonal elements in first column
    #pragma omp parallel for shared(N,M,A_value,A_index,A_nnz)
    for (int i = 0; i < N; i++)
    {
        for (int j = A_nnz[i] - 1; j > 0; j--)
        {
            if (A_index[i * M + j] > i)
            {
                int ind = A_index[i * M + j];
                int exchangeDone = 0;
                for (int k = 0; k < A_nnz[ind]; k++)
                {
                    // Existing corresponding value for transpose - exchange
                    if (A_index[ind * M + k] == i)
                    {
                        REAL_T tmp = A_value[i * M + j];

                        #pragma omp critical
                        {
                            A_value[i * M + j] = A_value[ind * M + k];
                            A_value[ind * M + k] = tmp;
                        }
                        exchangeDone = 1;
                        break;
                    }
                }

                // If no match add to end of row
                if (!exchangeDone)
                {
                    int jind = A_nnz[ind];

                    #pragma omp critical
                    {
                        A_index[ind * M + jind] = i;
                        A_value[ind * M + jind] = A_value[i * M + j];
                        A_nnz[ind]++;
                        A_nnz[i]--;
                    }
                }
            }
        }
    }

}
