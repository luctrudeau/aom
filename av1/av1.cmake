##
## Copyright (c) 2017, Alliance for Open Media. All rights reserved
##
## This source code is subject to the terms of the BSD 2 Clause License and
## the Alliance for Open Media Patent License 1.0. If the BSD 2 Clause License
## was not distributed with this source code in the LICENSE file, you can
## obtain it at www.aomedia.org/license/software. If the Alliance for Open
## Media Patent License 1.0 was not distributed with this source code in the
## PATENTS file, you can obtain it at www.aomedia.org/license/patent.
##
set(AOM_AV1_COMMON_SOURCES
    "${AOM_ROOT}/av1/av1_iface_common.h"
    "${AOM_ROOT}/av1/common/alloccommon.c"
    "${AOM_ROOT}/av1/common/alloccommon.h"
    # TODO(tomfinegan): Foward transform belongs in encoder.
    "${AOM_ROOT}/av1/common/av1_fwd_txfm1d.c"
    "${AOM_ROOT}/av1/common/av1_fwd_txfm1d.h"
    "${AOM_ROOT}/av1/common/av1_fwd_txfm2d.c"
    "${AOM_ROOT}/av1/common/av1_fwd_txfm2d_cfg.h"
    "${AOM_ROOT}/av1/common/av1_inv_txfm1d.c"
    "${AOM_ROOT}/av1/common/av1_inv_txfm1d.h"
    "${AOM_ROOT}/av1/common/av1_inv_txfm2d.c"
    "${AOM_ROOT}/av1/common/av1_inv_txfm2d_cfg.h"
    "${AOM_ROOT}/av1/common/av1_loopfilter.c"
    "${AOM_ROOT}/av1/common/av1_loopfilter.h"
    "${AOM_ROOT}/av1/common/av1_txfm.h"
    "${AOM_ROOT}/av1/common/blockd.c"
    "${AOM_ROOT}/av1/common/blockd.h"
    "${AOM_ROOT}/av1/common/common.h"
    "${AOM_ROOT}/av1/common/common_data.h"
    "${AOM_ROOT}/av1/common/convolve.c"
    "${AOM_ROOT}/av1/common/convolve.h"
    "${AOM_ROOT}/av1/common/debugmodes.c"
    "${AOM_ROOT}/av1/common/entropy.c"
    "${AOM_ROOT}/av1/common/entropy.h"
    "${AOM_ROOT}/av1/common/entropymode.c"
    "${AOM_ROOT}/av1/common/entropymode.h"
    "${AOM_ROOT}/av1/common/entropymv.c"
    "${AOM_ROOT}/av1/common/entropymv.h"
    "${AOM_ROOT}/av1/common/enums.h"
    "${AOM_ROOT}/av1/common/filter.c"
    "${AOM_ROOT}/av1/common/filter.h"
    "${AOM_ROOT}/av1/common/frame_buffers.c"
    "${AOM_ROOT}/av1/common/frame_buffers.h"
    "${AOM_ROOT}/av1/common/idct.c"
    "${AOM_ROOT}/av1/common/idct.h"
    "${AOM_ROOT}/av1/common/mv.h"
    "${AOM_ROOT}/av1/common/mvref_common.c"
    "${AOM_ROOT}/av1/common/mvref_common.h"
    "${AOM_ROOT}/av1/common/odintrin.c"
    "${AOM_ROOT}/av1/common/odintrin.h"
    "${AOM_ROOT}/av1/common/onyxc_int.h"
    "${AOM_ROOT}/av1/common/pred_common.c"
    "${AOM_ROOT}/av1/common/pred_common.h"
    "${AOM_ROOT}/av1/common/quant_common.c"
    "${AOM_ROOT}/av1/common/quant_common.h"
    "${AOM_ROOT}/av1/common/reconinter.c"
    "${AOM_ROOT}/av1/common/reconinter.h"
    "${AOM_ROOT}/av1/common/reconintra.c"
    "${AOM_ROOT}/av1/common/reconintra.h"
    "${AOM_ROOT}/av1/common/restoration.h"
    "${AOM_ROOT}/av1/common/scale.c"
    "${AOM_ROOT}/av1/common/scale.h"
    "${AOM_ROOT}/av1/common/scan.c"
    "${AOM_ROOT}/av1/common/scan.h"
    "${AOM_ROOT}/av1/common/seg_common.c"
    "${AOM_ROOT}/av1/common/seg_common.h"
    "${AOM_ROOT}/av1/common/thread_common.c"
    "${AOM_ROOT}/av1/common/thread_common.h"
    "${AOM_ROOT}/av1/common/tile_common.c"
    "${AOM_ROOT}/av1/common/tile_common.h")

set(AOM_AV1_DECODER_SOURCES
    "${AOM_ROOT}/av1/av1_dx_iface.c"
    "${AOM_ROOT}/av1/decoder/decodeframe.c"
    "${AOM_ROOT}/av1/decoder/decodeframe.h"
    "${AOM_ROOT}/av1/decoder/decodemv.c"
    "${AOM_ROOT}/av1/decoder/decodemv.h"
    "${AOM_ROOT}/av1/decoder/decoder.c"
    "${AOM_ROOT}/av1/decoder/decoder.h"
    "${AOM_ROOT}/av1/decoder/detokenize.c"
    "${AOM_ROOT}/av1/decoder/detokenize.h"
    "${AOM_ROOT}/av1/decoder/dsubexp.c"
    "${AOM_ROOT}/av1/decoder/dsubexp.h"
    "${AOM_ROOT}/av1/decoder/dthread.c"
    "${AOM_ROOT}/av1/decoder/dthread.h")

set(AOM_AV1_ENCODER_SOURCES
    "${AOM_ROOT}/av1/av1_cx_iface.c"
    "${AOM_ROOT}/av1/encoder/aq_complexity.c"
    "${AOM_ROOT}/av1/encoder/aq_complexity.h"
    "${AOM_ROOT}/av1/encoder/aq_cyclicrefresh.c"
    "${AOM_ROOT}/av1/encoder/aq_cyclicrefresh.h"
    "${AOM_ROOT}/av1/encoder/aq_variance.c"
    "${AOM_ROOT}/av1/encoder/aq_variance.h"
    "${AOM_ROOT}/av1/encoder/av1_quantize.c"
    "${AOM_ROOT}/av1/encoder/av1_quantize.h"
    "${AOM_ROOT}/av1/encoder/bitstream.c"
    "${AOM_ROOT}/av1/encoder/bitstream.h"
    "${AOM_ROOT}/av1/encoder/block.h"
    "${AOM_ROOT}/av1/encoder/context_tree.c"
    "${AOM_ROOT}/av1/encoder/context_tree.h"
    "${AOM_ROOT}/av1/encoder/cost.c"
    "${AOM_ROOT}/av1/encoder/cost.h"
    "${AOM_ROOT}/av1/encoder/dct.c"
    "${AOM_ROOT}/av1/encoder/encodeframe.c"
    "${AOM_ROOT}/av1/encoder/encodeframe.h"
    "${AOM_ROOT}/av1/encoder/encodemb.c"
    "${AOM_ROOT}/av1/encoder/encodemb.h"
    "${AOM_ROOT}/av1/encoder/encodemv.c"
    "${AOM_ROOT}/av1/encoder/encodemv.h"
    "${AOM_ROOT}/av1/encoder/encoder.c"
    "${AOM_ROOT}/av1/encoder/encoder.h"
    "${AOM_ROOT}/av1/encoder/ethread.c"
    "${AOM_ROOT}/av1/encoder/ethread.h"
    "${AOM_ROOT}/av1/encoder/extend.c"
    "${AOM_ROOT}/av1/encoder/extend.h"
    "${AOM_ROOT}/av1/encoder/firstpass.c"
    "${AOM_ROOT}/av1/encoder/firstpass.h"
    "${AOM_ROOT}/av1/encoder/hybrid_fwd_txfm.c"
    "${AOM_ROOT}/av1/encoder/hybrid_fwd_txfm.h"
    "${AOM_ROOT}/av1/encoder/lookahead.c"
    "${AOM_ROOT}/av1/encoder/lookahead.h"
    "${AOM_ROOT}/av1/encoder/mbgraph.c"
    "${AOM_ROOT}/av1/encoder/mbgraph.h"
    "${AOM_ROOT}/av1/encoder/mcomp.c"
    "${AOM_ROOT}/av1/encoder/mcomp.h"
    "${AOM_ROOT}/av1/encoder/picklpf.c"
    "${AOM_ROOT}/av1/encoder/picklpf.h"
    "${AOM_ROOT}/av1/encoder/ratectrl.c"
    "${AOM_ROOT}/av1/encoder/ratectrl.h"
    "${AOM_ROOT}/av1/encoder/rd.c"
    "${AOM_ROOT}/av1/encoder/rd.h"
    "${AOM_ROOT}/av1/encoder/rdopt.c"
    "${AOM_ROOT}/av1/encoder/rdopt.h"
    "${AOM_ROOT}/av1/encoder/resize.c"
    "${AOM_ROOT}/av1/encoder/resize.h"
    "${AOM_ROOT}/av1/encoder/segmentation.c"
    "${AOM_ROOT}/av1/encoder/segmentation.h"
    "${AOM_ROOT}/av1/encoder/speed_features.c"
    "${AOM_ROOT}/av1/encoder/speed_features.h"
    "${AOM_ROOT}/av1/encoder/subexp.c"
    "${AOM_ROOT}/av1/encoder/subexp.h"
    "${AOM_ROOT}/av1/encoder/temporal_filter.c"
    "${AOM_ROOT}/av1/encoder/temporal_filter.h"
    "${AOM_ROOT}/av1/encoder/tokenize.c"
    "${AOM_ROOT}/av1/encoder/tokenize.h"
    "${AOM_ROOT}/av1/encoder/treewriter.c"
    "${AOM_ROOT}/av1/encoder/treewriter.h"
    "${AOM_ROOT}/av1/encoder/variance_tree.c"
    "${AOM_ROOT}/av1/encoder/variance_tree.h")

set(AOM_AV1_COMMON_SSE2_INTRIN
    # Requires CONFIG_GLOBAL_MOTION or CONFIG_WARPED_MOTION
    #"${AOM_ROOT}/av1/common/x86/warp_plane_sse2.c"
    "${AOM_ROOT}/av1/common/x86/idct_intrin_sse2.c")

set(AOM_AV1_COMMON_SSSE3_INTRIN
    "${AOM_ROOT}/av1/common/x86/av1_convolve_ssse3.c")

set(AOM_AV1_COMMON_SSE4_1_INTRIN
    "${AOM_ROOT}/av1/common/x86/av1_fwd_txfm1d_sse4.c"
    "${AOM_ROOT}/av1/common/x86/av1_fwd_txfm2d_sse4.c")

set(AOM_AV1_COMMON_AVX2_INTRIN
    "${AOM_ROOT}/av1/common/x86/hybrid_inv_txfm_avx2.c")

set(AOM_AV1_ENCODER_SSE2_ASM
    "${AOM_ROOT}/av1/encoder/x86/dct_sse2.asm"
    "${AOM_ROOT}/av1/encoder/x86/error_sse2.asm"
    "${AOM_ROOT}/av1/encoder/x86/temporal_filter_apply_sse2.asm")

set(AOM_AV1_ENCODER_SSE2_INTRIN
    "${AOM_ROOT}/av1/encoder/x86/dct_intrin_sse2.c"
    "${AOM_ROOT}/av1/encoder/x86/highbd_block_error_intrin_sse2.c"
    "${AOM_ROOT}/av1/encoder/x86/av1_quantize_sse2.c")

set(AOM_AV1_ENCODER_SSSE3_ASM_X86_64
    "${AOM_ROOT}/av1/encoder/x86/av1_quantize_ssse3_x86_64.asm")

set(AOM_AV1_ENCODER_SSSE3_INTRIN
    "${AOM_ROOT}/av1/encoder/x86/dct_ssse3.c")

set(AOM_AV1_ENCODER_AVX2_INTRIN
    "${AOM_ROOT}/av1/encoder/x86/error_intrin_avx2.c"
    "${AOM_ROOT}/av1/encoder/x86/hybrid_fwd_txfm_avx2.c")

if (CONFIG_ACCOUNTING)
  set(AOM_AV1_COMMON_SOURCES
      ${AOM_AV1_COMMON_SOURCES}
      "${AOM_ROOT}/av1/common/accounting.c"
      "${AOM_ROOT}/av1/common/accounting.h")
endif ()

if (CONFIG_AOM_HIGHBITDEPTH)
  set(AOM_AV1_COMMON_SSE4_1_INTRIN
      ${AOM_AV1_COMMON_SSE4_1_INTRIN}
      "${AOM_ROOT}/av1/common/x86/av1_highbd_convolve_sse4.c"
      "${AOM_ROOT}/av1/common/x86/highbd_inv_txfm_sse4.c")

  set(AOM_AV1_COMMON_AVX2_INTRIN
      ${AOM_AV1_COMMON_AVX2_INTRIN}
      "${AOM_ROOT}/av1/common/x86/highbd_inv_txfm_avx2.c")

  set(AOM_AV1_ENCODER_SSE4_1_INTRIN
      ${AOM_AV1_ENCODER_SSE4_1_INTRIN}
      "${AOM_ROOT}/av1/encoder/x86/av1_highbd_quantize_sse4.c"
      "${AOM_ROOT}/av1/encoder/x86/highbd_fwd_txfm_sse4.c")
endif ()

if (CONFIG_CDEF)
  set(AOM_AV1_COMMON_SOURCES
      ${AOM_AV1_COMMON_SOURCES}
      "${AOM_ROOT}/av1/common/clpf.c"
      "${AOM_ROOT}/av1/common/clpf.h"
      "${AOM_ROOT}/av1/common/clpf_simd.h"
      "${AOM_ROOT}/av1/common/clpf_simd_kernel.h"
      "${AOM_ROOT}/av1/common/dering.c"
      "${AOM_ROOT}/av1/common/dering.h"
      "${AOM_ROOT}/av1/common/od_dering.c"
      "${AOM_ROOT}/av1/common/od_dering.h")

  set(AOM_AV1_ENCODER_SOURCES
      ${AOM_AV1_ENCODER_SOURCES}
      "${AOM_ROOT}/av1/encoder/clpf_rdo.c"
      "${AOM_ROOT}/av1/encoder/clpf_rdo.h"
      "${AOM_ROOT}/av1/encoder/pickdering.c")

  set(AOM_AV1_COMMON_SSE2_INTRIN
      ${AOM_AV1_COMMON_SSE2_INTRIN}
      "${AOM_ROOT}/av1/common/clpf_sse2.c")

  set(AOM_AV1_COMMON_SSSE3_INTRIN
      ${AOM_AV1_COMMON_SSSE3_INTRIN}
      "${AOM_ROOT}/av1/common/clpf_ssse3.c")

  set(AOM_AV1_COMMON_SSE4_1_INTRIN
      ${AOM_AV1_COMMON_SSE4_1_INTRIN}
      "${AOM_ROOT}/av1/common/clpf_sse4.c")

  set(AOM_AV1_ENCODER_SSE2_INTRIN
      ${AOM_AV1_ENCODER_SSE2_INTRIN}
      "${AOM_ROOT}/av1/encoder/clpf_rdo_sse2.c")

  set(AOM_AV1_ENCODER_SSSE3_INTRIN
      ${AOM_AV1_ENCODER_SSSE3_INTRIN}
      "${AOM_ROOT}/av1/encoder/clpf_rdo_ssse3.c")

  set(AOM_AV1_ENCODER_SSE4_1_INTRIN
      ${AOM_AV1_ENCODER_SSE4_1_INTRIN}
      "${AOM_ROOT}/av1/encoder/clpf_rdo_sse4.c"
      "${AOM_ROOT}/av1/common/x86/od_dering_sse4.c"
      "${AOM_ROOT}/av1/common/x86/od_dering_sse4.h")
endif ()

if (CONFIG_EXT_INTER)
  set(AOM_AV1_ENCODER_SOURCES
      ${AOM_AV1_ENCODER_SOURCES}
      "${AOM_ROOT}/av1/encoder/wedge_utils.c")

  set(AOM_AV1_ENCODER_SSE2_INTRIN
      ${AOM_AV1_ENCODER_SSE2_INTRIN}
      "${AOM_ROOT}/av1/encoder/x86/wedge_utils_sse2.c")
endif ()

if (CONFIG_FILTER_INTRA)
  set(AOM_AV1_COMMON_SSE4_1_INTRIN
      ${AOM_AV1_COMMON_SSE4_1_INTRIN}
      "${AOM_ROOT}/av1/common/x86/filterintra_sse4.c")
endif ()

if (CONFIG_INSPECTION)
  set(AOM_AV1_DECODER_SOURCES
      ${AOM_AV1_DECODER_SOURCES}
      "${AOM_ROOT}/av1/decoder/inspection.c"
      "${AOM_ROOT}/av1/decoder/inspection.h")
endif ()

if (CONFIG_INTERNAL_STATS)
  set(AOM_AV1_ENCODER_SOURCES
      ${AOM_AV1_ENCODER_SOURCES}
      "${AOM_ROOT}/av1/encoder/blockiness.c")
endif ()

if (CONFIG_PALETTE)
  set(AOM_AV1_ENCODER_SOURCES
      ${AOM_AV1_ENCODER_SOURCES}
      "${AOM_ROOT}/av1/encoder/palette.c"
      "${AOM_ROOT}/av1/encoder/palette.h")
endif ()

if (CONFIG_PVQ)
  set(AOM_AV1_COMMON_SOURCES
      ${AOM_AV1_COMMON_SOURCES}
      "${AOM_ROOT}/av1/common/laplace_tables.c"
      "${AOM_ROOT}/av1/common/pvq.c"
      "${AOM_ROOT}/av1/common/pvq.h"
      "${AOM_ROOT}/av1/common/pvq_state.c"
      "${AOM_ROOT}/av1/common/pvq_state.h"
      "${AOM_ROOT}/av1/common/partition.c"
      "${AOM_ROOT}/av1/common/partition.h"
      "${AOM_ROOT}/av1/common/generic_code.c"
      "${AOM_ROOT}/av1/common/generic_code.h"
      "${AOM_ROOT}/av1/common/zigzag4.c"
      "${AOM_ROOT}/av1/common/zigzag8.c"
      "${AOM_ROOT}/av1/common/zigzag16.c"
      "${AOM_ROOT}/av1/common/zigzag32.c")

    set(AOM_AV1_DECODER_SOURCES
        ${AOM_AV1_DECODER_SOURCES}
        "${AOM_ROOT}/av1/decoder/decint.h"
        "${AOM_ROOT}/av1/decoder/pvq_decoder.c"
        "${AOM_ROOT}/av1/decoder/pvq_decoder.h"
        "${AOM_ROOT}/av1/decoder/generic_decoder.c"
        "${AOM_ROOT}/av1/decoder/laplace_decoder.c")

    set(AOM_AV1_ENCODER_SOURCES
        ${AOM_AV1_ENCODER_SOURCES}
        "${AOM_ROOT}/av1/encoder/daala_compat_enc.c"
        "${AOM_ROOT}/av1/encoder/encint.h"
        "${AOM_ROOT}/av1/encoder/pvq_encoder.c"
        "${AOM_ROOT}/av1/encoder/pvq_encoder.h"
        "${AOM_ROOT}/av1/encoder/generic_encoder.c"
        "${AOM_ROOT}/av1/encoder/laplace_encoder.c")

    set(AOM_AV1_COMMON_SSE4_1_INTRIN
        ${AOM_AV1_COMMON_SSE4_1_INTRIN}
        "${AOM_ROOT}/av1/common/x86/pvq_sse4.c"
        "${AOM_ROOT}/av1/common/x86/pvq_sse4.h")

    if (NOT CONFIG_AV1_ENCODER)
      # TODO(tomfinegan): These should probably be in av1/common, and in a
      # common source list. For now this mirrors the original build system.
      set(AOM_AV1_DECODER_SOURCES
          ${AOM_AV1_DECODER_SOURCES}
          "${AOM_ROOT}/av1/encoder/dct.c"
          "${AOM_ROOT}/av1/encoder/hybrid_fwd_txfm.c"
          "${AOM_ROOT}/av1/encoder/hybrid_fwd_txfm.h")

      set(AOM_AV1_DECODER_SSE2_ASM
          ${AOM_AV1_DECODER_SSE2_ASM}
          "${AOM_ROOT}/av1/encoder/x86/dct_sse2.asm")

      set(AOM_AV1_DECODER_SSE2_INTRIN
          ${AOM_AV1_DECODER_SSE2_INTRIN}
          "${AOM_ROOT}/av1/encoder/x86/dct_intrin_sse2.c")

      set(AOM_AV1_DECODER_SSSE3_INTRIN
          ${AOM_AV1_DECODER_SSSE3_INTRIN}
          "${AOM_ROOT}/av1/encoder/x86/dct_ssse3.c")
    endif ()
endif ()

# Setup AV1 common/decoder/encoder targets. The libaom target must exist before
# this function is called.
function (setup_av1_targets)
  add_library(aom_av1_common OBJECT ${AOM_AV1_COMMON_SOURCES})
  set(AOM_LIB_TARGETS ${AOM_LIB_TARGETS} aom_av1_common)
  target_sources(aom PUBLIC $<TARGET_OBJECTS:aom_av1_common>)

  if (CONFIG_AV1_DECODER)
    add_library(aom_av1_decoder OBJECT ${AOM_AV1_DECODER_SOURCES})
    set(AOM_LIB_TARGETS ${AOM_LIB_TARGETS} aom_av1_decoder)
    target_sources(aom PUBLIC $<TARGET_OBJECTS:aom_av1_decoder>)
  endif ()

  if (CONFIG_AV1_ENCODER)
    add_library(aom_av1_encoder OBJECT ${AOM_AV1_ENCODER_SOURCES})
    set(AOM_LIB_TARGETS ${AOM_LIB_TARGETS} aom_av1_encoder)
    target_sources(aom PUBLIC $<TARGET_OBJECTS:aom_av1_encoder>)
  endif ()

  if (HAVE_SSE2)
    require_flag_nomsvc("-msse2" NO)
    add_intrinsics_object_library("-msse2" "sse2" "aom_av1_common"
                                  "AOM_AV1_COMMON_SSE2_INTRIN")
    if (CONFIG_AV1_DECODER)
      if (AOM_AV1_DECODER_SSE2_ASM)
        add_asm_library("aom_av1_decoder_sse2" "AOM_AV1_DECODER_SSE2_ASM" "aom")
      endif ()

      if (AOM_AV1_DECODER_SSE2_INTRIN)
        add_intrinsics_object_library("-msse2" "sse2" "aom_av1_decoder"
                                      "AOM_AV1_DECODER_SSE2_INTRIN")
      endif ()
    endif ()

    if (CONFIG_AV1_ENCODER)
      add_asm_library("aom_av1_encoder_sse2" "AOM_AV1_ENCODER_SSE2_ASM" "aom")
      add_intrinsics_object_library("-msse2" "sse2" "aom_av1_encoder"
                                    "AOM_AV1_ENCODER_SSE2_INTRIN")
    endif ()
  endif ()

  if (HAVE_SSSE3)
    require_flag_nomsvc("-mssse3" NO)
    add_intrinsics_object_library("-mssse3" "ssse3" "aom_av1_common"
                                  "AOM_AV1_COMMON_SSSE3_INTRIN")

    if (CONFIG_AV1_DECODER)
      if (AOM_AV1_DECODER_SSSE3_INTRIN)
        add_intrinsics_object_library("-mssse3" "ssse3" "aom_av1_decoder"
                                      "AOM_AV1_DECODER_SSSE3_INTRIN")
      endif ()
    endif ()

    if (CONFIG_AV1_ENCODER)
      add_intrinsics_object_library("-mssse3" "ssse3" "aom_av1_encoder"
                                    "AOM_AV1_ENCODER_SSSE3_INTRIN")
    endif ()
  endif ()

  if (HAVE_SSE4_1)
    require_flag_nomsvc("-msse4.1" NO)
    add_intrinsics_object_library("-msse4.1" "sse4" "aom_av1_common"
                                  "AOM_AV1_COMMON_SSE4_1_INTRIN")

    if (CONFIG_AV1_ENCODER)
      if ("${AOM_TARGET_CPU}" STREQUAL "x86_64")
        add_asm_library("aom_av1_encoder_ssse3"
                        "AOM_AV1_ENCODER_SSSE3_ASM_X86_64" "aom")
      endif ()

      if (AOM_AV1_ENCODER_SSE4_1_INTRIN)
        add_intrinsics_object_library("-msse4.1" "sse4" "aom_av1_encoder"
                                      "AOM_AV1_ENCODER_SSE4_1_INTRIN")
      endif ()
    endif ()
  endif ()

  if (HAVE_AVX2)
    require_flag_nomsvc("-mavx2" NO)
    add_intrinsics_object_library("-mavx2" "avx2" "aom_av1_common"
                                  "AOM_AV1_COMMON_AVX2_INTRIN")

    if (CONFIG_AV1_ENCODER)
      add_intrinsics_object_library("-mavx2" "avx2" "aom_av1_encoder"
                                    "AOM_AV1_ENCODER_AVX2_INTRIN")
    endif ()
  endif ()

  # Pass the new lib targets up to the parent scope instance of
  # $AOM_LIB_TARGETS.
  set(AOM_LIB_TARGETS ${AOM_LIB_TARGETS} PARENT_SCOPE)
endfunction ()

function (setup_av1_test_targets)
endfunction ()
