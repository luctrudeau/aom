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
  const int luma_tx_blk_size = cfl->luma_tx_blk_size;
  tran_high_t *const ref_coeff_high = cfl->ref_coeff_high;
  int coeff_offset;
  int i, j, k = 1;

  // Adjusting row and cols for 4:2:0
  blk_row *= 2;
  blk_col *= 2;

  coeff_offset = scale * blk_row * MAX_SB_SIZE + (scale * blk_col);

  // Check that the last coeff offset is smaller than the max superblock size
  assert(coeff_offset
      + ((tx_blk_size-1) * MAX_SB_SIZE + (tx_blk_size-1)) < MAX_SB_SQUARE);

  luma_coeff = &cfl->luma_coeff[coeff_offset];

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
  } if (tx_blk_size == 2 * luma_tx_blk_size) {
    tran_high_t *out;
    // zero out ref_coeff (this might not be needed)
    for (i = 0; i < MAX_TX_SQUARE; i++) {
      ref_coeff_high[i] = 0;
    }

    for (j = 0; j < 2; j++) {
      for (i = 0; i < 2; i++) {
        out = ref_coeff_high + (j * MAX_TX_SQUARE + i);



      // We perform TF and keep the top left quadrant
      od_tf_up_hv_lp(ref_coeff_high, tx_blk_size, luma_coeff, MAX_SB_SIZE,
          tx_blk_size, tx_blk_size, tx_blk_size);

      // CFL does not apply to DC (only AC)
      for (k = 1; i < tx_blk_size * tx_blk_size; k++) {
        // Applying TF to bigger transforms can cause overflow
        if (ref_coeff_high[i] >= INT16_MAX) {
          ref_coeff[i] = INT16_MAX;
        } else {
          ref_coeff[i] = ref_coeff_high[i];
        }
      }
    }

  } else {


    int shift = 0;
    if (tx_blk_size * 2 == luma_tx_blk_size) {
      shift = 1;
    } else if (tx_blk_size * 4 == luma_tx_blk_size) {
      shift = 2;
    }

    // Clean this up
    if (tx_blk_size * 2 != luma_tx_blk_size && tx_blk_size * 4 != luma_tx_blk_size)
      printf("tx_blk_size %d luma_tx_blk_size %d\n", tx_blk_size, luma_tx_blk_size);
    assert(tx_blk_size * 2 == luma_tx_blk_size || tx_blk_size * 4 == luma_tx_blk_size );

    assert(shift);

    // cfl does not apply to dc (only ac)
    // first row
    for (i = 1; i < tx_blk_size; i++) {
      if ( tx_blk_size < 16 ) {
        // scale the values as the inverse transform will be half the size of
        // the forward transform.
        ref_coeff[k++] = luma_coeff[i] >> shift;
      } else {
	// don't scale the 32x32 it's already scaled
        ref_coeff[k++] = luma_coeff[i];
      }
    }
    // remainder of the block
    for (j = 1; j < tx_blk_size; j++) {
      for (i = 0; i < tx_blk_size; i++) {
        if ( tx_blk_size < 16 ) {
          ref_coeff[k++] = luma_coeff[j * MAX_SB_SIZE + i] >> shift;
	} else {
          ref_coeff[k++] = luma_coeff[j * MAX_SB_SIZE + i];
	}
      }
    }
  }
}

/* store the values of the luma plane to predict the values of the chroma plane
 *  * if the ac values are coded the dequantized transformed coefficients are
 *    used
 *  * if the ac values are skipped, the intra luma prediction is used.
 */
void cfl_store_predictor(CFL_CONTEXT *const cfl, int blk_row, int blk_col,
		int tx_blk_size, const tran_low_t *const ref_coeff,
		const tran_low_t *const dqcoeff, int ac_dc_coded) {

  const int scale = 4; // needs to be adjusted to support 4:4:4
  const int coeff_offset = scale * blk_row * MAX_SB_SIZE + (scale * blk_col);
  const tran_low_t *src;
  tran_low_t *const luma_coeff = &cfl->luma_coeff[coeff_offset];
  int i,j,k = 0;

  // We store the Luma transform size as it can differ from the chroma
  // transform size. We assume that Luma transform size is constant inside a
  // partition.
  cfl->luma_tx_blk_size = tx_blk_size;

  // check that the last coeff offset is smaller than the max superblock size
  assert(coeff_offset
    + ((tx_blk_size-1) * MAX_SB_SIZE + (tx_blk_size-1)) < MAX_SB_SQUARE);


  switch(ac_dc_coded) {
    case 0: // dc and ac are skipped
    case 1: // dc is coded and ac is skipped
      // ac is not coded: copy the ac prediction
      src = ref_coeff;
      break;
    case 2: // ac is coded and dc is skipped.
    case 3: // ac is coded and dc is coded
      // ac is coded: copy luma transform dequantized coefficients
      src = dqcoeff;
      break;
    default:
      fprintf(stderr, "ac_dc_coded value %d is > 3\n", ac_dc_coded);
      assert(0);
  }

  for (j = 0; j < tx_blk_size; j++) {
    for (i = 0; i < tx_blk_size; i++) {
      luma_coeff[j * MAX_SB_SIZE + i] = src[k++];
    }
  }
}

/*increase horizontal and vertical frequency resolution of an entire block and
   return the lf quarter.*/
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
      /*we swap lh and hl for compatibility with od_tf_up_hv.*/
      OD_HAAR_KERNEL(ll, hl, lh, hh);
      hswap = x & 1;
      dst[(2*y + vswap)*dstride + 2*x + hswap] = ll;
      dst[(2*y + vswap)*dstride + 2*x + 1 - hswap] = lh;
      dst[(2*y + 1 - vswap)*dstride + 2*x + hswap] = hl;
      dst[(2*y + 1 - vswap)*dstride + 2*x + 1 - hswap] = hh;
    }
  }
}
