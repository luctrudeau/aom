/*
 * Copyright (c) 2016, Alliance for Open Media. All rights reserved
 *
 * This source code is subject to the terms of the BSD 2 Clause License and
 * the Alliance for Open Media Patent License 1.0. If the BSD 2 Clause License
 * was not distributed with this source code in the LICENSE file, you can
 * obtain it at www.aomedia.org/license/software. If the Alliance for Open
 * Media Patent License 1.0 was not distributed with this source code in the
 * PATENTS file, you can obtain it at www.aomedia.org/license/patent.
 */
#ifndef AV1_COMMON_CFL_H_
#define AV1_COMMON_CFL_H_

#include "aom_dsp/aom_dsp_common.h"

#ifdef __cplusplus
extern "C" {
#endif

// CFL: This will replace cfl_ctx
typedef struct cfl_context {
  /* Luma tx type for all blocks */
  int luma_tx_type[MAX_SB_SQUARE >> 4];
  /* Dequantized transformed coefficients of Luma used to predict Chroma.*/
  DECLARE_ALIGNED(16, tran_low_t, luma_coeff[MAX_SB_SQUARE]);
} CFL_CONTEXT;


void cfl_load_predictor(tran_low_t *const cfl_luma_coeff,
		const tran_low_t *const ref_coeff, int tx_blk_size);

void cfl_store_predictor(tran_low_t *const cfl_luma_coeff, 
		const tran_low_t *const ref_coeff,
		const tran_low_t *const dqcoeff, 
		int ac_dc_coded, 
		int tx_blk_size);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif // AV1_COMMON_CFL_G 
