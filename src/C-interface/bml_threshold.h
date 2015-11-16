/** \file */

#ifndef __BML_THRESHOLD_H
#define __BML_THRESHOLD_H

#include "bml_types.h"

bml_matrix_t *bml_threshold_new(
    const bml_matrix_t * A,
    const double threshold);

void bml_threshold(
    bml_matrix_t * A,
    const double threshold);

#endif
