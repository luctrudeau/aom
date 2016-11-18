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

#include "av1/common/clf.h"

void cfl_load_predictor(tran_low_t *const cfl_luma_coeff,
		const tran_low_t *const ref_coeff, int tx_blk_size) {
  int k = 0;
  for (j = 0; j < tx_blk_size; j++) {
    for (i = 0; i < tx_blk_size; i++) {
      ref_coeff[k++] = cfl_luma_coeff[j * MAX_SB_SIZE + i];
    }
  }
}

void cfl_store_predictor(tran_low_t *const cfl_luma_coeff, 
		const tran_low_t *const ref_coeff,
		const tran_low_t *const dqcoeff, 
		int ac_dc_coded, 
		int tx_blk_size) {
  int i,j,k;
  switch(ac_dc_coded) {
    case 0: // DC is skipped and AC is skipped.
      k = 0;
      for (j = 0; j < tx_blk_size; j++) {
        for (i = 0; i < tx_blk_size; i++) {
          cfl_luma_coeff[j * MAX_SB_SIZE + i] = ref_coeff[k++];
        }
      }
      break;
    case 1: // DC is coded but AC is skipped.
      cfl_luma_coeff[0] = dqcoeff[0];
      // Use the predicted AC.
      k = 1;
      for (i = 1; i < tx_blk_size; i++) {
	cfl_luma_coeff[i] = ref_coeff[k++];
      }
      for (j = 1; j < tx_blk_size; j++) {
        for (i = 0; i < tx_blk_size; i++) {
	  cfl_luma_coeff[i] = ref_coeff[k++];
	}
      }
      break;
    case 2: // AC is coded but DC is skipped.
      // Use predicted DC.
      cfl_luma_coeff[0] = ref_coeff[0];
      k = 1;
      for (i = 1; i < tx_blk_size; i++) {
	cfl_luma_coeff[i] = dqcoeff[k++];
      }
      for (j = 1; j < tx_blk_size; j++) {
        for (i = 0; i < tx_blk_size; i++) {
	  cfl_luma_coeff[i] = dqcoeff[k++];
	}
      }
      break;
    case 3:
      // Store Luma transform dequantized coefficients as predictors for Chroma
      k = 0;
      for (j = 0; j < tx_blk_size; j++) {
        for (i = 0; i < tx_blk_size; i++) {
          cfl_luma_coeff[j * MAX_SB_SIZE + i] = dqcoeff[k++];
        }
      }
      break;
    default:
      assert(0);
  }
}

