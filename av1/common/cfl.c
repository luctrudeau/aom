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

#include "av1/common/cfl.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


void cfl_load_predictor(CFL_CONTEXT *const cfl, int blk_row, int blk_col,
		tran_low_t *const ref_coeff, int tx_blk_size) {
  const int scale = 4; // Needs to be adjusted to support 4:4:4
  const tran_low_t *luma_coeff;
  tran_high_t *const ref_coeff_high = cfl->ref_coeff_high;
  int luma_tx_offset;
  int luma_tx_blk_size;
  int coeff_offset;
  int i, j, k = 1;

  // Needs to be adjusted to support 4:4:4
  blk_row *= 2;
  blk_col *= 2;

  luma_tx_offset = blk_row * CFL_TX_STRIDE + blk_col;
  coeff_offset = scale * blk_row * MAX_SB_SIZE + (scale * blk_col);

  // Check that the tx offset stay inside the tx block memory
  assert(luma_tx_offset < CFL_MAX_TX_BLOCKS);
  // Check that the last coeff offset is smaller than the max superblock size
  assert(coeff_offset
    + ((tx_blk_size-1) * MAX_SB_SIZE + (tx_blk_size-1)) < MAX_SB_SQUARE);

  luma_coeff = &cfl->luma_coeff[coeff_offset];
  luma_tx_blk_size = cfl->luma_tx_blk_sizes[luma_tx_offset];

  if (tx_blk_size == luma_tx_blk_size) {
    // 4 luma block associated with 1 chroma block

    // zero out ref_coeff (this might not be needed)
    for (i = 0; i < MAX_TX_SQUARE; i++) {
      ref_coeff_high[i] = 0;
    }

    // We perform TF and keep the top left quadrant
    od_tf_up_hv_lp(ref_coeff_high, tx_blk_size, luma_coeff, MAX_SB_SIZE,
		    tx_blk_size, tx_blk_size, tx_blk_size);

    // CFL does not apply to DC (only AC)
    for (i = 1; i < tx_blk_size * tx_blk_size; i++) {
      // Applying TF to bigger transforms can cause overflow
      if (ref_coeff_high[i] >= INT16_MAX) {
        ref_coeff[i] = INT16_MAX;
      } else {
        ref_coeff[i] = ref_coeff_high[i];
      }
    }
  } else {
    // 1 luma block associated to 1 chroma block (half the size)
    assert(tx_blk_size * 2 == luma_tx_blk_size);

    // CFL does not apply to DC (only AC)
    // First row
    for (i = 1; i < tx_blk_size; i++) {
      if ( tx_blk_size < 16 ) {
        // Scale the values as the inverse transform will be half the size of
        // the forward transform.
        ref_coeff[k++] = luma_coeff[i] >> 1;
      } else {
	// Don't scale the 32x32 it's already scaled
        ref_coeff[k++] = luma_coeff[i];
      }
    }
    // remainder of the block
    for (j = 1; j < tx_blk_size; j++) {
      for (i = 0; i < tx_blk_size; i++) {
        if ( tx_blk_size < 16 ) {
          ref_coeff[k++] = luma_coeff[j * MAX_SB_SIZE + i] >> 1;
	} else {
          ref_coeff[k++] = luma_coeff[j * MAX_SB_SIZE + i];
	}
      }
    }
  }
}

/* Store the values of the luma plane to predict the values of the chroma plane
 *  * If the AC values are coded the dequantized transformed coefficients are
 *    used
 *  * if the AC values are skipped, the intra luma prediction is used.
 */
void cfl_store_predictor(CFL_CONTEXT *const cfl, int blk_row, int blk_col,
		int tx_blk_size, const tran_low_t *const ref_coeff,
		const tran_low_t *const dqcoeff, int ac_dc_coded) {

  const int scale = 4; // Needs to be adjusted to support 4:4:4
  const int tx_offset = blk_row * CFL_TX_STRIDE + blk_col;
  const int coeff_offset = scale * blk_row * MAX_SB_SIZE + (scale * blk_col);
  const tran_low_t *src;
  tran_low_t *const luma_coeff = &cfl->luma_coeff[coeff_offset];
  int i,j,k = 0;

  // Check that the tx offset stay inside the tx block memory
  assert(tx_offset < CFL_MAX_TX_BLOCKS);
  // Check that the last coeff offset is smaller than the max superblock size
  assert(coeff_offset
    + ((tx_blk_size-1) * MAX_SB_SIZE + (tx_blk_size-1)) < MAX_SB_SQUARE);

  // We need to store the size of the luma transform to compare with the size
  // of the chroma transform. This indicates if we need to perform TF
  cfl->luma_tx_blk_sizes[tx_offset] = tx_blk_size;

  switch(ac_dc_coded) {
    case 0: // DC and AC are skipped
    case 1: // DC is coded and AC is skipped
      // AC is not coded: copy the AC prediction
      src = ref_coeff;
      break;
    case 2: // AC is coded and DC is skipped.
    case 3: // AC is coded and DC is coded
      // AC is coded: copy Luma transform dequantized coefficients
      src = dqcoeff;
      break;
    default:
      fprintf(stderr, "AC_DC_CODED value %d is > 3\n", ac_dc_coded);
      assert(0);
  }

  for (j = 0; j < tx_blk_size; j++) {
    for (i = 0; i < tx_blk_size; i++) {
      luma_coeff[j * MAX_SB_SIZE + i] = src[k++];
    }
  }
}

/*Increase horizontal and vertical frequency resolution of an entire block and
   return the LF quarter.*/
void od_tf_up_hv_lp(tran_high_t *const dst, int dstride,
 const tran_low_t *const src, int sstride, int dx, int dy, int n) {
  int x;
  int y;
  for (y = 0; y < n >> 1; y++) {
    int vswap;
    vswap = y & 1;
    for (x = 0; x < n >> 1; x++) {
      int32_t ll;
      int32_t lh;
      int32_t hl;
      int32_t hh;
      int hswap;
      ll = src[y*sstride + x];
      lh = src[y*sstride + x + dx];
      hl = src[(y + dy)*sstride + x];
      hh = src[(y + dy)*sstride + x + dx];
      /*We swap lh and hl for compatibility with od_tf_up_hv.*/
      OD_HAAR_KERNEL(ll, hl, lh, hh);
      hswap = x & 1;
      dst[(2*y + vswap)*dstride + 2*x + hswap] = ll;
      dst[(2*y + vswap)*dstride + 2*x + 1 - hswap] = lh;
      dst[(2*y + 1 - vswap)*dstride + 2*x + hswap] = hl;
      dst[(2*y + 1 - vswap)*dstride + 2*x + 1 - hswap] = hh;
    }
  }
}
