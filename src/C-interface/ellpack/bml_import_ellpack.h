#ifndef __BML_IMPORT_ELLPACK_H
#define __BML_IMPORT_ELLPACK_H

#include "bml_types_ellpack.h"

bml_matrix_ellpack_t *bml_import_from_dense_ellpack(
    const bml_matrix_precision_t matrix_precision,
    const bml_dense_order_t order,
    const int N,
    const void *A,
    const double threshold,
    const int M,
    const bml_distribution_mode_t distrib_mode);

bml_matrix_ellpack_t *bml_import_from_dense_ellpack_single_real(
    const bml_dense_order_t order,
    const int N,
    const void *A,
    const double threshold,
    const int M,
    const bml_distribution_mode_t distrib_mode);

bml_matrix_ellpack_t *bml_import_from_dense_ellpack_double_real(
    const bml_dense_order_t order,
    const int N,
    const void *A,
    const double threshold,
    const int M,
    const bml_distribution_mode_t distrib_mode);

bml_matrix_ellpack_t *bml_import_from_dense_ellpack_single_complex(
    const bml_dense_order_t order,
    const int N,
    const void *A,
    const double threshold,
    const int M,
    const bml_distribution_mode_t distrib_mode);

bml_matrix_ellpack_t *bml_import_from_dense_ellpack_double_complex(
    const bml_dense_order_t order,
    const int N,
    const void *A,
    const double threshold,
    const int M,
    const bml_distribution_mode_t distrib_mode);

#endif
