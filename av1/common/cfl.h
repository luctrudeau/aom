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

#include "av1/common/enums.h"

// Forward declaration of AV1_COMMON, in order to avoid creating a cyclic
// dependency by importing av1/common/onyxc_int.h
typedef struct AV1Common AV1_COMMON;

// Forward declaration of MACROBLOCK, in order to avoid creating a cyclic
// dependency by importing av1/common/blockd.h
typedef struct macroblockd MACROBLOCKD;

typedef struct {
  // Pixel buffer containing the luma pixels used as prediction for chroma
  uint8_t y_pix[MAX_SB_SQUARE];

  // Height and width of the luma prediction block currently in the pixel buffer
  int y_height, y_width;

  // Chroma subsampling
  int subsampling_x, subsampling_y;

  // CfL Performs its own block level DC_PRED for each chromatic plane
  double dc_pred[CFL_PRED_PLANES];

  // Count the number of TX blocks in a predicted block to know when you are at
  // the last one, so you can check for skips.
  // TODO(any) Is there a better way to do this?
  int num_tx_blk[CFL_PRED_PLANES];
} CFL_CTX;

// Q8 unit vector codebook used to code the combination of alpha_u and alpha_v
static const int cfl_alpha_uvecs[CFL_ALPHABET_SIZE][CFL_PRED_PLANES] = {
  { 0, 0 },      { 181, -181 }, { -160, 200 }, { 200, -160 },
  { -132, 220 }, { 220, -132 }, { -95, 238 },  { 238, -95 },
  { -50, 251 },  { 251, -50 },  { 0, 256 },    { 256, 0 },
  { 85, 241 },   { 241, 85 },   { 209, 148 },  { 148, 209 }
};

// Q8 magnitudes applied to alpha_uvec
static const int cfl_alpha_mags[CFL_ALPHABET_SIZE] = {
  16, -16, 23, -23, 32, -32, 45, -45, 64, -64, 91, -91, 128, -128, 181, -181
};

void cfl_init(CFL_CTX *cfl, AV1_COMMON *cm, int subsampling_x,
              int subsampling_y);

void cfl_dc_pred(MACROBLOCKD *xd, BLOCK_SIZE plane_bsize, TX_SIZE tx_size);

static inline double cfl_alpha(int component, int mag) {
  return component * mag * (1. / (1 << 16));
}

static inline double cfl_ind_to_alpha(int uvec_ind, int mag_ind,
                                      CFL_PRED_TYPE pred_type) {
  const int comp = cfl_alpha_uvecs[uvec_ind][pred_type];
  return (comp) ? cfl_alpha(comp, cfl_alpha_mags[mag_ind]) : 0.;
}

void cfl_predict_block(const CFL_CTX *cfl, uint8_t *dst, int dst_stride,
                       int row, int col, TX_SIZE tx_size, double dc_pred,
                       double alpha);

void cfl_store(CFL_CTX *cfl, const uint8_t *input, int input_stride, int row,
               int col, TX_SIZE tx_size);

double cfl_load(const CFL_CTX *cfl, uint8_t *output, int output_stride, int row,
                int col, int width, int height);
#endif  // AV1_COMMON_CFL_H_
