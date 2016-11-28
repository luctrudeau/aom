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

#if CONFIG_CFL_TEST
FILE *_cfl_log = NULL;

void open_cfl_log(const char *filename) {
  _cfl_log = fopen(filename, "w");
  fprintf(_cfl_log, "Plane, Size, Blk_Row, Blk_Col, Block_Skip, AC_DC_Coded\n");
}

void close_cfl_log() {
  if (_cfl_log != NULL) {
    fclose(_cfl_log);
    _cfl_log = NULL;
  }
}
#endif

void cfl_set_luma(CFL_CONTEXT *const cfl, int blk_row, int blk_col,
		int tx_blk_size) {
  const int tx_offset = blk_row * 16 + blk_col; // TODO(ltrudeau) REPLACE WITH CONSTANT
  const int scale = 4;
  const int coeff_offset = scale * blk_row * MAX_SB_SIZE + scale * blk_col;

  assert(tx_offset < 256); // TODO(ltrudeau) REPLACE WITH CONSTANT
  assert(coeff_offset
    + ((tx_blk_size-1) * MAX_SB_SIZE + (tx_blk_size-1)) < MAX_SB_SQUARE);

  cfl->luma_tx_blk_sizes[tx_offset] = tx_blk_size;
  cfl->luma_tx_blk_size = tx_blk_size;
  cfl->luma_coeff_ptr = &cfl->luma_coeff[coeff_offset];

#if CONFIG_CFL_TEST
  fprintf(_cfl_log, "y,%d,%d,%d,\n", tx_blk_size, blk_row, blk_col);
#endif
}

void cfl_set_chroma(CFL_CONTEXT *const cfl, int blk_row, int blk_col,
		int tx_blk_size) {
  // Adjust offset based on block size
  blk_row *= 2;
  blk_col *= 2;
  const int luma_tx_offset = blk_row * 16 + blk_col;
  const int scale = 4; // TODO(ltrudeau) adjust for subsampling
  const int coeff_offset = scale * blk_row * MAX_SB_SIZE + scale * blk_col;

  assert(luma_tx_offset < 256);
  assert(coeff_offset
    + ((tx_blk_size-1) * MAX_SB_SIZE + (tx_blk_size-1)) < MAX_SB_SQUARE);

  cfl->luma_tx_blk_size = cfl->luma_tx_blk_sizes[luma_tx_offset];
  cfl->luma_coeff_ptr = &cfl->luma_coeff[coeff_offset];

#if CONFIG_CFL_TEST
  fprintf(_cfl_log, "c,%d,%d,%d,\n", tx_blk_size, blk_row, blk_col);
#endif
}

void cfl_load_predictor(const CFL_CONTEXT *const cfl,
		tran_low_t *const ref_coeff, int tx_blk_size) {
    const tran_low_t *const luma_coeff = cfl->luma_coeff_ptr;
    const int luma_tx_blk_size = cfl->luma_tx_blk_size;
    int i, j, k = 0;
    //if (tx_blk_size == luma_tx_blk_size) {
      // There's a regression with TF, something must be broken
   //   od_tf_up_hv_lp(ref_coeff, tx_blk_size, luma_coeff, MAX_SB_SIZE,
//		      tx_blk_size, tx_blk_size, tx_blk_size);
   // } else {
   //   assert(tx_blk_size * 2 == luma_tx_blk_size);
#if CONFIG_CFL_TEST
    //fprintf(_cfl_log, "\nBefore Load,");
    //for (i = 0; i < tx_blk_size * tx_blk_size; i++){
    //  fprintf(_cfl_log, "%d,", ref_coeff[i]);
    //}
    //fprintf(_cfl_log, "\n");
#endif
      for (j = 0; j < tx_blk_size; j++) {
        for (i = 0; i < tx_blk_size; i++) {
          ref_coeff[k++] = luma_coeff[j * MAX_SB_SIZE + i];
        }
      }
      if (tx_blk_size < 16 && tx_blk_size < luma_tx_blk_size) {
	ref_coeff[0] >>= 1;
      }
  //  }

#if CONFIG_CFL_TEST
    fprintf(_cfl_log, "\nLoad,");
    for (i = 0; i < tx_blk_size * tx_blk_size; i++){
      fprintf(_cfl_log, "%d,", ref_coeff[i]);
    }
    fprintf(_cfl_log, "\n");
#endif
}

void cfl_store_predictor(CFL_CONTEXT *const cfl,
		const tran_low_t *const ref_coeff,
		const tran_low_t *const dqcoeff,
		int ac_dc_coded) {
  tran_low_t *const luma_coeff = cfl->luma_coeff_ptr;
  const int tx_blk_size = cfl->luma_tx_blk_size;
  int i,j,k = 0;
  switch(ac_dc_coded) {
    case 0: // DC is skipped and AC is skipped.
      k = 0;
      for (j = 0; j < tx_blk_size; j++) {
        for (i = 0; i < tx_blk_size; i++) {
          luma_coeff[j * MAX_SB_SIZE + i] = ref_coeff[k++];
        }
      }
      break;
    case 1: // DC is coded but AC is skipped.
      luma_coeff[0] = dqcoeff[0];
      // Use the predicted AC.
      k = 1;
      for (i = 1; i < tx_blk_size; i++) {
	luma_coeff[i] = ref_coeff[k++];
      }
      for (j = 1; j < tx_blk_size; j++) {
        for (i = 0; i < tx_blk_size; i++) {
	  luma_coeff[j * MAX_SB_SIZE + i] = ref_coeff[k++];
	}
      }
      break;
    case 2: // AC is coded but DC is skipped.
      // Use predicted DC.
      luma_coeff[0] = ref_coeff[0];
      k = 1;
      for (i = 1; i < tx_blk_size; i++) {
	luma_coeff[i] = dqcoeff[k++];
      }
      for (j = 1; j < tx_blk_size; j++) {
        for (i = 0; i < tx_blk_size; i++) {
	  luma_coeff[j * MAX_SB_SIZE + i] = dqcoeff[k++];
	}
      }
      break;
    case 3:
      // Store Luma transform dequantized coefficients as predictors for Chroma
      k = 0;
      for (j = 0; j < tx_blk_size; j++) {
        for (i = 0; i < tx_blk_size; i++) {
          luma_coeff[j * MAX_SB_SIZE + i] = dqcoeff[k++];
        }
      }
      break;
    default:
      assert(0);
  }
#if CONFIG_CFL_TEST
/*  fprintf(_cfl_log, "%d\nDequant,", ac_dc_coded);
  if(ac_dc_coded) {
    for (i = 0; i < tx_blk_size * tx_blk_size; i++) {
      fprintf(_cfl_log, "%d,", dqcoeff[i]);
    }
  }*/
  fprintf(_cfl_log, "\nStore(%d),", ac_dc_coded);
  for (j = 0; j < tx_blk_size; j++) {
    for (i = 0; i < tx_blk_size; i++) {
      fprintf(_cfl_log, "%d,", luma_coeff[j * MAX_SB_SIZE + i]);
    }
  }
  fprintf(_cfl_log, "\n");
#endif
}


/*Increase horizontal and vertical frequency resolution of an entire block and
   return the LF quarter.*/
void od_tf_up_hv_lp(tran_low_t *dst, int dstride,
 const tran_low_t *src, int sstride, int dx, int dy, int n) {
  int x;
  int y;
  for (y = 0; y < n >> 1; y++) {
    int vswap;
    vswap = y & 1;
    for (x = 0; x < n >> 1; x++) {
      tran_low_t ll;
      tran_low_t lh;
      tran_low_t hl;
      tran_low_t hh;
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
