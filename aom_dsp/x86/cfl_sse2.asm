;
; Copyright (c) 2016, Alliance for Open Media. All rights reserved
;
; This source code is subject to the terms of the BSD 2 Clause License and
; the Alliance for Open Media Patent License 1.0. If the BSD 2 Clause License
; was not distributed with this source code in the LICENSE file, you can
; obtain it at www.aomedia.org/license/software. If the Alliance for Open
; Media Patent License 1.0 was not distributed with this source code in the
; PATENTS file, you can obtain it at www.aomedia.org/license/patent.
;

;

%include "third_party/x86inc/x86inc.asm"

SECTION_RODATA 64

all_2s: times 16 dw 2

zero_upper: times 16 dw 0xff

SECTION .text

%macro CFL_FN 0
cglobal cfl_luma_subsampling_420, 4, 9, 8, y_pix, output, width, height

%define counter_hq r7q
%define counter_wq r8q

xor counter_hq, counter_hq

mova m4, [all_2s]
mova m5, [zero_upper]


.loop_h:

  xor counter_wq, counter_wq

  .loop_w:

    movu m0, [y_pixq + counter_wq*2]       ;  y_pix[top_left]...y_pix[top_left + 15]
    movu m1, [y_pixq + counter_wq*2 + 64]  ;  y_pix[bot_left]...y_pix[bot_left + 15]

    psrlw m2, m0, 8
    psrlw m3, m1, 8
    pand  m0, m5
    pand  m1, m5
    paddw m0, m2
    paddw m1, m3

    paddw m0, m1

    paddw m0, m4
    psrlw m0, 2
    pand  m0, m5
    packuswb m0, m1

%if cpuflag(avx2)
    vextracti128 xm1, m0, 1
    punpcklqdq xm0, xm0, xm1
    movu [outputq + counter_wq], xm0
%else
    movq [outputq + counter_wq], m0
%endif


%if cpuflag(avx2)
    add counter_wq, 16
%else
    add counter_wq, 8
%endif
    cmp counter_wq, widthq
    jl .loop_w

  add y_pixq, 128
  add outputq, 64

  add counter_hq, 1
  cmp counter_hq, heightq
  jl .loop_h


    RET
%endmacro


INIT_XMM sse2
CFL_FN

INIT_YMM avx2
CFL_FN
