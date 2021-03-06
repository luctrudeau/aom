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

#include "./av1_rtcd.h"
#include "av1/common/enums.h"
#include "av1/common/av1_txfm.h"
#include "av1/common/av1_inv_txfm1d.h"
#include "av1/common/av1_inv_txfm2d_cfg.h"

static INLINE TxfmFunc inv_txfm_type_to_func(TXFM_TYPE txfm_type) {
  switch (txfm_type) {
    case TXFM_TYPE_DCT4: return av1_idct4_new;
    case TXFM_TYPE_DCT8: return av1_idct8_new;
    case TXFM_TYPE_DCT16: return av1_idct16_new;
    case TXFM_TYPE_DCT32: return av1_idct32_new;
    case TXFM_TYPE_ADST4: return av1_iadst4_new;
    case TXFM_TYPE_ADST8: return av1_iadst8_new;
    case TXFM_TYPE_ADST16: return av1_iadst16_new;
    case TXFM_TYPE_ADST32: return av1_iadst32_new;
    default: assert(0); return NULL;
  }
}

#if CONFIG_EXT_TX
static const TXFM_2D_CFG *inv_txfm_cfg_ls[FLIPADST_ADST + 1][TX_SIZES] = {
  {
#if CONFIG_CB4X4
      NULL,
#endif
      &inv_txfm_2d_cfg_dct_dct_4, &inv_txfm_2d_cfg_dct_dct_8,
      &inv_txfm_2d_cfg_dct_dct_16, &inv_txfm_2d_cfg_dct_dct_32 },
  {
#if CONFIG_CB4X4
      NULL,
#endif
      &inv_txfm_2d_cfg_adst_dct_4, &inv_txfm_2d_cfg_adst_dct_8,
      &inv_txfm_2d_cfg_adst_dct_16, &inv_txfm_2d_cfg_adst_dct_32 },
  {
#if CONFIG_CB4X4
      NULL,
#endif
      &inv_txfm_2d_cfg_dct_adst_4, &inv_txfm_2d_cfg_dct_adst_8,
      &inv_txfm_2d_cfg_dct_adst_16, &inv_txfm_2d_cfg_dct_adst_32 },
  {
#if CONFIG_CB4X4
      NULL,
#endif
      &inv_txfm_2d_cfg_adst_adst_4, &inv_txfm_2d_cfg_adst_adst_8,
      &inv_txfm_2d_cfg_adst_adst_16, &inv_txfm_2d_cfg_adst_adst_32 },
  {
#if CONFIG_CB4X4
      NULL,
#endif
      &inv_txfm_2d_cfg_adst_dct_4, &inv_txfm_2d_cfg_adst_dct_8,
      &inv_txfm_2d_cfg_adst_dct_16, &inv_txfm_2d_cfg_adst_dct_32 },
  {
#if CONFIG_CB4X4
      NULL,
#endif
      &inv_txfm_2d_cfg_dct_adst_4, &inv_txfm_2d_cfg_dct_adst_8,
      &inv_txfm_2d_cfg_dct_adst_16, &inv_txfm_2d_cfg_dct_adst_32 },
  {
#if CONFIG_CB4X4
      NULL,
#endif
      &inv_txfm_2d_cfg_adst_adst_4, &inv_txfm_2d_cfg_adst_adst_8,
      &inv_txfm_2d_cfg_adst_adst_16, &inv_txfm_2d_cfg_adst_adst_32 },
  {
#if CONFIG_CB4X4
      NULL,
#endif
      &inv_txfm_2d_cfg_adst_adst_4, &inv_txfm_2d_cfg_adst_adst_8,
      &inv_txfm_2d_cfg_adst_adst_16, &inv_txfm_2d_cfg_adst_adst_32 },
  {
#if CONFIG_CB4X4
      NULL,
#endif
      &inv_txfm_2d_cfg_adst_adst_4, &inv_txfm_2d_cfg_adst_adst_8,
      &inv_txfm_2d_cfg_adst_adst_16, &inv_txfm_2d_cfg_adst_adst_32 },
};
#else
static const TXFM_2D_CFG *inv_txfm_cfg_ls[TX_TYPES][TX_SIZES] = {
  {
#if CONFIG_CB4X4
      NULL,
#endif
      &inv_txfm_2d_cfg_dct_dct_4, &inv_txfm_2d_cfg_dct_dct_8,
      &inv_txfm_2d_cfg_dct_dct_16, &inv_txfm_2d_cfg_dct_dct_32 },
  {
#if CONFIG_CB4X4
      NULL,
#endif
      &inv_txfm_2d_cfg_adst_dct_4, &inv_txfm_2d_cfg_adst_dct_8,
      &inv_txfm_2d_cfg_adst_dct_16, &inv_txfm_2d_cfg_adst_dct_32 },
  {
#if CONFIG_CB4X4
      NULL,
#endif
      &inv_txfm_2d_cfg_dct_adst_4, &inv_txfm_2d_cfg_dct_adst_8,
      &inv_txfm_2d_cfg_dct_adst_16, &inv_txfm_2d_cfg_dct_adst_32 },
  {
#if CONFIG_CB4X4
      NULL,
#endif
      &inv_txfm_2d_cfg_adst_adst_4, &inv_txfm_2d_cfg_adst_adst_8,
      &inv_txfm_2d_cfg_adst_adst_16, &inv_txfm_2d_cfg_adst_adst_32 },
};
#endif

TXFM_2D_FLIP_CFG av1_get_inv_txfm_cfg(int tx_type, int tx_size) {
  TXFM_2D_FLIP_CFG cfg;
  set_flip_cfg(tx_type, &cfg);
  cfg.cfg = inv_txfm_cfg_ls[tx_type][tx_size];
  return cfg;
}

TXFM_2D_FLIP_CFG av1_get_inv_txfm_64x64_cfg(int tx_type) {
  TXFM_2D_FLIP_CFG cfg = { 0, 0, NULL };
  switch (tx_type) {
    case DCT_DCT:
      cfg.cfg = &inv_txfm_2d_cfg_dct_dct_64;
      set_flip_cfg(tx_type, &cfg);
      break;
    default: assert(0);
  }
  return cfg;
}

static INLINE void inv_txfm2d_add_c(const int32_t *input, int16_t *output,
                                    int stride, TXFM_2D_FLIP_CFG *cfg,
                                    int32_t *txfm_buf) {
  const int txfm_size = cfg->cfg->txfm_size;
  const int8_t *shift = cfg->cfg->shift;
  const int8_t *stage_range_col = cfg->cfg->stage_range_col;
  const int8_t *stage_range_row = cfg->cfg->stage_range_row;
  const int8_t *cos_bit_col = cfg->cfg->cos_bit_col;
  const int8_t *cos_bit_row = cfg->cfg->cos_bit_row;
  const TxfmFunc txfm_func_col = inv_txfm_type_to_func(cfg->cfg->txfm_type_col);
  const TxfmFunc txfm_func_row = inv_txfm_type_to_func(cfg->cfg->txfm_type_row);

  // txfm_buf's length is  txfm_size * txfm_size + 2 * txfm_size
  // it is used for intermediate data buffering
  int32_t *temp_in = txfm_buf;
  int32_t *temp_out = temp_in + txfm_size;
  int32_t *buf = temp_out + txfm_size;
  int32_t *buf_ptr = buf;
  int c, r;

  // Rows
  for (r = 0; r < txfm_size; ++r) {
    txfm_func_row(input, buf_ptr, cos_bit_row, stage_range_row);
    round_shift_array(buf_ptr, txfm_size, -shift[0]);
    input += txfm_size;
    buf_ptr += txfm_size;
  }

  // Columns
  for (c = 0; c < txfm_size; ++c) {
    if (cfg->lr_flip == 0) {
      for (r = 0; r < txfm_size; ++r) temp_in[r] = buf[r * txfm_size + c];
    } else {
      // flip left right
      for (r = 0; r < txfm_size; ++r)
        temp_in[r] = buf[r * txfm_size + (txfm_size - c - 1)];
    }
    txfm_func_col(temp_in, temp_out, cos_bit_col, stage_range_col);
    round_shift_array(temp_out, txfm_size, -shift[1]);
    if (cfg->ud_flip == 0) {
      for (r = 0; r < txfm_size; ++r) output[r * stride + c] += temp_out[r];
    } else {
      // flip upside down
      for (r = 0; r < txfm_size; ++r)
        output[r * stride + c] += temp_out[txfm_size - r - 1];
    }
  }
}

void av1_inv_txfm2d_add_4x4_c(const int32_t *input, uint16_t *output,
                              int stride, int tx_type, int bd) {
  int txfm_buf[4 * 4 + 4 + 4];
  // output contains the prediction signal which is always positive and smaller
  // than (1 << bd) - 1
  // since bd < 16-1, therefore we can treat the uint16_t* output buffer as an
  // int16_t*
  TXFM_2D_FLIP_CFG cfg = av1_get_inv_txfm_cfg(tx_type, TX_4X4);
  inv_txfm2d_add_c(input, (int16_t *)output, stride, &cfg, txfm_buf);
  clamp_block((int16_t *)output, 4, stride, 0, (1 << bd) - 1);
}

void av1_inv_txfm2d_add_8x8_c(const int32_t *input, uint16_t *output,
                              int stride, int tx_type, int bd) {
  int txfm_buf[8 * 8 + 8 + 8];
  // output contains the prediction signal which is always positive and smaller
  // than (1 << bd) - 1
  // since bd < 16-1, therefore we can treat the uint16_t* output buffer as an
  // int16_t*
  TXFM_2D_FLIP_CFG cfg = av1_get_inv_txfm_cfg(tx_type, TX_8X8);
  inv_txfm2d_add_c(input, (int16_t *)output, stride, &cfg, txfm_buf);
  clamp_block((int16_t *)output, 8, stride, 0, (1 << bd) - 1);
}

void av1_inv_txfm2d_add_16x16_c(const int32_t *input, uint16_t *output,
                                int stride, int tx_type, int bd) {
  int txfm_buf[16 * 16 + 16 + 16];
  // output contains the prediction signal which is always positive and smaller
  // than (1 << bd) - 1
  // since bd < 16-1, therefore we can treat the uint16_t* output buffer as an
  // int16_t*
  TXFM_2D_FLIP_CFG cfg = av1_get_inv_txfm_cfg(tx_type, TX_16X16);
  inv_txfm2d_add_c(input, (int16_t *)output, stride, &cfg, txfm_buf);
  clamp_block((int16_t *)output, 16, stride, 0, (1 << bd) - 1);
}

void av1_inv_txfm2d_add_32x32_c(const int32_t *input, uint16_t *output,
                                int stride, int tx_type, int bd) {
  int txfm_buf[32 * 32 + 32 + 32];
  // output contains the prediction signal which is always positive and smaller
  // than (1 << bd) - 1
  // since bd < 16-1, therefore we can treat the uint16_t* output buffer as an
  // int16_t*
  TXFM_2D_FLIP_CFG cfg = av1_get_inv_txfm_cfg(tx_type, TX_32X32);
  inv_txfm2d_add_c(input, (int16_t *)output, stride, &cfg, txfm_buf);
  clamp_block((int16_t *)output, 32, stride, 0, (1 << bd) - 1);
}

void av1_inv_txfm2d_add_64x64_c(const int32_t *input, uint16_t *output,
                                int stride, int tx_type, int bd) {
  int txfm_buf[64 * 64 + 64 + 64];
  // output contains the prediction signal which is always positive and smaller
  // than (1 << bd) - 1
  // since bd < 16-1, therefore we can treat the uint16_t* output buffer as an
  // int16_t*
  TXFM_2D_FLIP_CFG cfg = av1_get_inv_txfm_64x64_cfg(tx_type);
  inv_txfm2d_add_c(input, (int16_t *)output, stride, &cfg, txfm_buf);
  clamp_block((int16_t *)output, 64, stride, 0, (1 << bd) - 1);
}
