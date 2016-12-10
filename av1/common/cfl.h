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

#include <stdio.h>
#include <assert.h>

#include "aom_dsp/aom_dsp_common.h"
#include "av1/common/enums.h"

#ifdef __cplusplus
extern "C" {
#endif

#if CONFIG_CFL_TEST
extern FILE *_cfl_log;
extern FILE *_cfl_chroma_pred;

void open_cfl_log(const char* filename);

void close_cfl_log();

void cfl_store_ac_dc_coded(int ac_dc_coded);
#endif

// CFL: This will replace cfl_ctx
typedef struct cfl_context {
  // Let's waste memory for now and fix it when it works
  int luma_tx_blk_sizes[256]; // TODO(ltrudeau) find constant later
  int luma_tx_blk_size;

  /* Dequantized transformed coefficients of Luma used to predict Chroma.*/
  DECLARE_ALIGNED(16, tran_low_t, luma_coeff[MAX_SB_SQUARE]);
  tran_low_t *luma_coeff_ptr;
} CFL_CONTEXT;


void cfl_set_luma(CFL_CONTEXT *const cfl, int blk_row, int blk_col,
		int tx_blk_size);

void cfl_set_chroma(CFL_CONTEXT *const cfl, int blk_row, int blk_col,
		int tx_blk_size);

void cfl_load_predictor(const CFL_CONTEXT *const cfl,
		tran_low_t *const ref_coeff, int tx_blk_size);

void cfl_store_predictor(CFL_CONTEXT *const cfl,
		const tran_low_t *const ref_coeff,
		const tran_low_t *const dqcoeff,
		int ac_dc_coded);


/*This is an in-place, reversible, orthonormal Haar transform in 7 adds,
   1 shift (2 operations per sample).
  It is its own inverse (but requires swapping lh and hl on one side for
   bit-exact reversibility).
  It is defined in a macro here so it can be reused in various places.*/
#define OD_HAAR_KERNEL(ll, lh, hl, hh) \
  do { \
    tran_high_t llmhh_2__; \
    (ll) += (hl); \
    (hh) -= (lh); \
    llmhh_2__ = ((ll) - (hh)) >> 1; \
    (lh) = llmhh_2__ - (lh); \
    (hl) = llmhh_2__ - (hl); \
    (ll) -= (lh); \
    (hh) += (hl); \
  } \
  while(0)


void od_tf_up_hv_lp(tran_high_t *dst, int dstride, const tran_low_t *src,
		int sstride, int dx, int dy, int n);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif // AV1_COMMON_CFL_G
