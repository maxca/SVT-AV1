/*
* Copyright(c) 2019 Intel Corporation
* SPDX - License - Identifier: BSD - 2 - Clause - Patent
*/

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

#ifndef EbTransforms_h
#define EbTransforms_h

#include "EbDefinitions.h"
#include "EbCoefficients.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "EbEncDecProcess.h"

#define UNIT_QUANT_SHIFT 2
#define INV_COS_BIT 12
#define MAX_TXFM_STAGE_NUM 12
#define MAX_TXWH_IDX 5
#define AOM_QM_BITS 5
#define MAX_TX_SCALE 1
/*****************************
    * DEBUG MACROS
    *****************************/
#define ZERO_COEFF 0
#define ZERO_COEFF_CHROMA 0

#define IS_2D_TRANSFORM(tx_type) (tx_type < IDTX)

#define av1_zero(dest) memset(&(dest), 0, sizeof(dest))

static const int8_t txsize_log2_minus4[TX_SIZES_ALL] = {
    0, // TX_4X4
    2, // TX_8X8
    4, // TX_16X16
    6, // TX_32X32
    6, // TX_64X64
    1, // TX_4X8
    1, // TX_8X4
    3, // TX_8X16
    3, // TX_16X8
    5, // TX_16X32
    5, // TX_32X16
    6, // TX_32X64
    6, // TX_64X32
    2, // TX_4X16
    2, // TX_16X4
    4, // TX_8X32
    4, // TX_32X8
    5, // TX_16X64
    5, // TX_64X16
};
// Note:
// TranHigh is the datatype used for intermediate transform stages.
typedef int64_t TranHigh;

static const TxType1D vtx_tab[TX_TYPES] = {
    DCT_1D,
    ADST_1D,
    DCT_1D,
    ADST_1D,
    FLIPADST_1D,
    DCT_1D,
    FLIPADST_1D,
    ADST_1D,
    FLIPADST_1D,
    IDTX_1D,
    DCT_1D,
    IDTX_1D,
    ADST_1D,
    IDTX_1D,
    FLIPADST_1D,
    IDTX_1D,
};
static const TxType1D htx_tab[TX_TYPES] = {
    DCT_1D,
    DCT_1D,
    ADST_1D,
    ADST_1D,
    DCT_1D,
    FLIPADST_1D,
    FLIPADST_1D,
    FLIPADST_1D,
    ADST_1D,
    IDTX_1D,
    IDTX_1D,
    DCT_1D,
    IDTX_1D,
    ADST_1D,
    IDTX_1D,
    FLIPADST_1D,
};
static const BlockSize txsize_to_bsize[TX_SIZES_ALL] = {
    BLOCK_4X4, // TX_4X4
    BLOCK_8X8, // TX_8X8
    BLOCK_16X16, // TX_16X16
    BLOCK_32X32, // TX_32X32
    BLOCK_64X64, // TX_64X64
    BLOCK_4X8, // TX_4X8
    BLOCK_8X4, // TX_8X4
    BLOCK_8X16, // TX_8X16
    BLOCK_16X8, // TX_16X8
    BLOCK_16X32, // TX_16X32
    BLOCK_32X16, // TX_32X16
    BLOCK_32X64, // TX_32X64
    BLOCK_64X32, // TX_64X32
    BLOCK_4X16, // TX_4X16
    BLOCK_16X4, // TX_16X4
    BLOCK_8X32, // TX_8X32
    BLOCK_32X8, // TX_32X8
    BLOCK_16X64, // TX_16X64
    BLOCK_64X16, // TX_64X16
};
static const int8_t fwd_shift_4x4[3]   = {2, 0, 0};
static const int8_t fwd_shift_8x8[3]   = {2, -1, 0};
static const int8_t fwd_shift_16x16[3] = {2, -2, 0};
static const int8_t fwd_shift_32x32[3] = {2, -4, 0};
static const int8_t fwd_shift_64x64[3] = {0, -2, -2};
static const int8_t fwd_shift_4x8[3]   = {2, -1, 0};
static const int8_t fwd_shift_8x4[3]   = {2, -1, 0};
static const int8_t fwd_shift_8x16[3]  = {2, -2, 0};
static const int8_t fwd_shift_16x8[3]  = {2, -2, 0};
static const int8_t fwd_shift_16x32[3] = {2, -4, 0};
static const int8_t fwd_shift_32x16[3] = {2, -4, 0};
static const int8_t fwd_shift_32x64[3] = {0, -2, -2};
static const int8_t fwd_shift_64x32[3] = {2, -4, -2};
static const int8_t fwd_shift_4x16[3]  = {2, -1, 0};
static const int8_t fwd_shift_16x4[3]  = {2, -1, 0};
static const int8_t fwd_shift_8x32[3]  = {2, -2, 0};
static const int8_t fwd_shift_32x8[3]  = {2, -2, 0};
static const int8_t fwd_shift_16x64[3] = {0, -2, 0};
static const int8_t fwd_shift_64x16[3] = {2, -4, 0};

static const int8_t fwd_cos_bit_col[MAX_TXWH_IDX /*txw_idx*/][MAX_TXWH_IDX /*txh_idx*/] = {
    {13, 13, 13, 0, 0},
    {13, 13, 13, 12, 0},
    {13, 13, 13, 12, 13},
    {0, 13, 13, 12, 13},
    {0, 0, 13, 12, 13}};
static const int8_t fwd_cos_bit_row[MAX_TXWH_IDX /*txw_idx*/][MAX_TXWH_IDX /*txh_idx*/] = {
    {13, 13, 12, 0, 0},
    {13, 13, 13, 12, 0},
    {13, 13, 12, 13, 12},
    {0, 12, 13, 12, 11},
    {0, 0, 12, 11, 10}};

// Utility function that returns the log of the ratio of the col and row
// sizes.
typedef enum TxfmType {
    TXFM_TYPE_DCT4,
    TXFM_TYPE_DCT8,
    TXFM_TYPE_DCT16,
    TXFM_TYPE_DCT32,
    TXFM_TYPE_DCT64,
    TXFM_TYPE_ADST4,
    TXFM_TYPE_ADST8,
    TXFM_TYPE_ADST16,
    TXFM_TYPE_ADST32,
    TXFM_TYPE_IDENTITY4,
    TXFM_TYPE_IDENTITY8,
    TXFM_TYPE_IDENTITY16,
    TXFM_TYPE_IDENTITY32,
    TXFM_TYPE_IDENTITY64,
    TXFM_TYPES,
    TXFM_TYPE_INVALID,
} TxfmType;
typedef struct Txfm2dFlipCfg {
    TxSize        tx_size;
    int32_t       ud_flip; // flip upside down
    int32_t       lr_flip; // flip left to right
    const int8_t *shift;
    int8_t        cos_bit_col;
    int8_t        cos_bit_row;
    int8_t        stage_range_col[MAX_TXFM_STAGE_NUM];
    int8_t        stage_range_row[MAX_TXFM_STAGE_NUM];
    TxfmType      txfm_type_col;
    TxfmType      txfm_type_row;
    int32_t       stage_num_col;
    int32_t       stage_num_row;
} Txfm2dFlipCfg;

static const TxfmType av1_txfm_type_ls[5][TX_TYPES_1D] = {
    {TXFM_TYPE_DCT4, TXFM_TYPE_ADST4, TXFM_TYPE_ADST4, TXFM_TYPE_IDENTITY4},
    {TXFM_TYPE_DCT8, TXFM_TYPE_ADST8, TXFM_TYPE_ADST8, TXFM_TYPE_IDENTITY8},
    {TXFM_TYPE_DCT16, TXFM_TYPE_ADST16, TXFM_TYPE_ADST16, TXFM_TYPE_IDENTITY16},
    {TXFM_TYPE_DCT32, TXFM_TYPE_ADST32, TXFM_TYPE_ADST32, TXFM_TYPE_IDENTITY32},
    {TXFM_TYPE_DCT64, TXFM_TYPE_INVALID, TXFM_TYPE_INVALID, TXFM_TYPE_IDENTITY64}};
static const int8_t av1_txfm_stage_num_list[TXFM_TYPES] = {
    4, // TXFM_TYPE_DCT4
    6, // TXFM_TYPE_DCT8
    8, // TXFM_TYPE_DCT16
    10, // TXFM_TYPE_DCT32
    12, // TXFM_TYPE_DCT64
    7, // TXFM_TYPE_ADST4
    8, // TXFM_TYPE_ADST8
    10, // TXFM_TYPE_ADST16
    12, // TXFM_TYPE_ADST32
    1, // TXFM_TYPE_IDENTITY4
    1, // TXFM_TYPE_IDENTITY8
    1, // TXFM_TYPE_IDENTITY16
    1, // TXFM_TYPE_IDENTITY32
    1, // TXFM_TYPE_IDENTITY64
};

static INLINE int32_t get_txh_idx(TxSize tx_size) {
    return tx_size_high_log2[tx_size] - tx_size_high_log2[0];
}
static const int8_t fdct4_range_mult2[4]       = {0, 2, 3, 3};
static const int8_t fdct8_range_mult2[6]       = {0, 2, 4, 5, 5, 5};
static const int8_t fdct16_range_mult2[8]      = {0, 2, 4, 6, 7, 7, 7, 7};
static const int8_t fdct32_range_mult2[10]     = {0, 2, 4, 6, 8, 9, 9, 9, 9, 9};
static const int8_t fdct64_range_mult2[12]     = {0, 2, 4, 6, 8, 10, 11, 11, 11, 11, 11, 11};
static const int8_t fadst4_range_mult2[7]      = {0, 2, 4, 3, 3, 3, 3};
static const int8_t fadst8_range_mult2[8]      = {0, 0, 1, 3, 3, 5, 5, 5};
static const int8_t fadst16_range_mult2[10]    = {0, 0, 1, 3, 3, 5, 5, 7, 7, 7};
static const int8_t fadst32_range_mult2[12]    = {0, 0, 1, 3, 3, 5, 5, 7, 7, 9, 9, 9};
static const int8_t max_fwd_range_mult2_col[5] = {3, 5, 7, 9, 11};
static const int8_t fidtx4_range_mult2[1]      = {1};
static const int8_t fidtx8_range_mult2[1]      = {2};
static const int8_t fidtx16_range_mult2[1]     = {3};
static const int8_t fidtx32_range_mult2[1]     = {4};
static const int8_t fidtx64_range_mult2[1]     = {5};
static const int8_t inv_cos_bit_col[MAX_TXWH_IDX][MAX_TXWH_IDX] = {
    {INV_COS_BIT, INV_COS_BIT, INV_COS_BIT, 0, 0},
    {INV_COS_BIT, INV_COS_BIT, INV_COS_BIT, INV_COS_BIT, 0},
    {INV_COS_BIT, INV_COS_BIT, INV_COS_BIT, INV_COS_BIT, INV_COS_BIT},
    {0, INV_COS_BIT, INV_COS_BIT, INV_COS_BIT, INV_COS_BIT},
    {0, 0, INV_COS_BIT, INV_COS_BIT, INV_COS_BIT}};
static const int8_t inv_cos_bit_row[MAX_TXWH_IDX][MAX_TXWH_IDX] = {
    {INV_COS_BIT, INV_COS_BIT, INV_COS_BIT, 0, 0},
    {INV_COS_BIT, INV_COS_BIT, INV_COS_BIT, INV_COS_BIT, 0},
    {INV_COS_BIT, INV_COS_BIT, INV_COS_BIT, INV_COS_BIT, INV_COS_BIT},
    {0, INV_COS_BIT, INV_COS_BIT, INV_COS_BIT, INV_COS_BIT},
    {0, 0, INV_COS_BIT, INV_COS_BIT, INV_COS_BIT}};
static const int8_t inv_shift_4x4[2]   = {0, -4};
static const int8_t inv_shift_8x8[2]   = {-1, -4};
static const int8_t inv_shift_16x16[2] = {-2, -4};
static const int8_t inv_shift_32x32[2] = {-2, -4};
static const int8_t inv_shift_64x64[2] = {-2, -4};
static const int8_t inv_shift_4x8[2]   = {0, -4};
static const int8_t inv_shift_8x4[2]   = {0, -4};
static const int8_t inv_shift_8x16[2]  = {-1, -4};
static const int8_t inv_shift_16x8[2]  = {-1, -4};
static const int8_t inv_shift_16x32[2] = {-1, -4};
static const int8_t inv_shift_32x16[2] = {-1, -4};
static const int8_t inv_shift_32x64[2] = {-1, -4};
static const int8_t inv_shift_64x32[2] = {-1, -4};
static const int8_t inv_shift_4x16[2]  = {-1, -4};
static const int8_t inv_shift_16x4[2]  = {-1, -4};
static const int8_t inv_shift_8x32[2]  = {-2, -4};
static const int8_t inv_shift_32x8[2]  = {-2, -4};
static const int8_t inv_shift_16x64[2] = {-2, -4};
static const int8_t inv_shift_64x16[2] = {-2, -4};

static INLINE int32_t get_txw_idx(TxSize tx_size) {
    return tx_size_wide_log2[tx_size] - tx_size_wide_log2[0];
}

#define BLOCK_SIZES_ALL 22
static INLINE int is_rect_tx(TxSize tx_size) { return tx_size >= TX_SIZES; }
static INLINE int is_rect_tx_allowed_bsize(BlockSize bsize) {
    static const char lut[BLOCK_SIZES_ALL] = {
        0, // BLOCK_4X4
        1, // BLOCK_4X8
        1, // BLOCK_8X4
        0, // BLOCK_8X8
        1, // BLOCK_8X16
        1, // BLOCK_16X8
        0, // BLOCK_16X16
        1, // BLOCK_16X32
        1, // BLOCK_32X16
        0, // BLOCK_32X32
        1, // BLOCK_32X64
        1, // BLOCK_64X32
        0, // BLOCK_64X64
        0, // BLOCK_64X128
        0, // BLOCK_128X64
        0, // BLOCK_128X128
        1, // BLOCK_4X16
        1, // BLOCK_16X4
        1, // BLOCK_8X32
        1, // BLOCK_32X8
        1, // BLOCK_16X64
        1, // BLOCK_64X16
    };

    return lut[bsize];
}
static INLINE int is_rect_tx_allowed(/*const MacroBlockD *xd,*/
                                     const MbModeInfo *mbmi) {
    return is_rect_tx_allowed_bsize(mbmi->block_mi.sb_type) /*&&
            !xd->lossless[mbmi->segment_id]*/
        ;
}

static const int8_t iadst4_range[7] = {0, 1, 0, 0, 0, 0, 0};

// sum of fwd_shift_##
static const int8_t inv_start_range[TX_SIZES_ALL] = {
    5, // 4x4 transform
    6, // 8x8 transform
    7, // 16x16 transform
    7, // 32x32 transform
    7, // 64x64 transform
    5, // 4x8 transform
    5, // 8x4 transform
    6, // 8x16 transform
    6, // 16x8 transform
    6, // 16x32 transform
    6, // 32x16 transform
    6, // 32x64 transform
    6, // 64x32 transform
    6, // 4x16 transform
    6, // 16x4 transform
    7, // 8x32 transform
    7, // 32x8 transform
    7, // 16x64 transform
    7, // 64x16 transform
};
////////////////////// QUANTIZATION//////////////
typedef struct QuantParam {
    int32_t      log_scale;
    TxSize       tx_size;
    const QmVal *qmatrix;
    const QmVal *iqmatrix;
} QuantParam;

static INLINE int32_t av1_get_max_eob(TxSize tx_size) {
    if (tx_size == TX_64X64 || tx_size == TX_64X32 || tx_size == TX_32X64) return 1024;
    if (tx_size == TX_16X64 || tx_size == TX_64X16) return 512;
    return tx_size_2d[tx_size];
}
static const uint32_t q_func[] = {26214, 23302, 20560, 18396, 16384, 14564};

extern const int32_t eb_av1_cospi_arr_data[7][64];
extern const int32_t eb_av1_sinpi_arr_data[7][5];
extern const int8_t *eb_inv_txfm_shift_ls[TX_SIZES_ALL];

static const int32_t cos_bit_min = 10;

static const int32_t new_sqrt2_bits = 12;
// 2^12 * sqrt(2)
static const int32_t new_sqrt2 = 5793;
// 2^12 / sqrt(2)
static const int32_t new_inv_sqrt2 = 2896;

static INLINE const int32_t *cospi_arr(int32_t n) { return eb_av1_cospi_arr_data[n - cos_bit_min]; }

static INLINE const int32_t *sinpi_arr(int32_t n) { return eb_av1_sinpi_arr_data[n - cos_bit_min]; }

static INLINE void get_flip_cfg(TxType tx_type, int32_t *ud_flip, int32_t *lr_flip) {
    switch (tx_type) {
    case DCT_DCT:
    case ADST_DCT:
    case DCT_ADST:
    case ADST_ADST:
        *ud_flip = 0;
        *lr_flip = 0;
        break;
    case IDTX:
    case V_DCT:
    case H_DCT:
    case V_ADST:
    case H_ADST:
        *ud_flip = 0;
        *lr_flip = 0;
        break;
    case FLIPADST_DCT:
    case FLIPADST_ADST:
    case V_FLIPADST:
        *ud_flip = 1;
        *lr_flip = 0;
        break;
    case DCT_FLIPADST:
    case ADST_FLIPADST:
    case H_FLIPADST:
        *ud_flip = 0;
        *lr_flip = 1;
        break;
    case FLIPADST_FLIPADST:
        *ud_flip = 1;
        *lr_flip = 1;
        break;
    default:
        *ud_flip = 0;
        *lr_flip = 0;
        assert(0);
    }
}

static INLINE int32_t get_rect_tx_log_ratio(int32_t col, int32_t row) {
    if (col == row) return 0;
    if (col > row) {
        if (col == row * 2) return 1;
        if (col == row * 4) return 2;
        assert(0 && "Unsupported transform size");
    } else {
        if (row == col * 2) return -1;
        if (row == col * 4) return -2;
        assert(0 && "Unsupported transform size");
    }
    return 0; // Invalid
}

extern EbErrorType av1_estimate_transform(int16_t *residual_buffer, uint32_t residual_stride,
                                          int32_t *coeff_buffer, uint32_t coeff_stride,
                                          TxSize transform_size, uint64_t *three_quad_energy,
                                          int16_t *transform_inner_array_ptr,
                                          uint32_t bit_increment, TxType transform_type,
                                          PlaneType            component_type,
                                          EB_TRANS_COEFF_SHAPE trans_coeff_shape);

extern int32_t av1_quantize_inv_quantize(
    PictureControlSet *pcs_ptr, ModeDecisionContext *md_context, int32_t *coeff,
    const uint32_t coeff_stride, int32_t *quant_coeff, int32_t *recon_coeff, uint32_t qp,
    int32_t segmentation_qp_offset, uint32_t width, uint32_t height, TxSize txsize, uint16_t *eob,
    uint32_t *y_count_non_zero_coeffs, uint32_t component_type, uint32_t bit_increment,
    TxType tx_type, ModeDecisionCandidateBuffer *candidate_buffer, int16_t txb_skip_context,
    int16_t dc_sign_context, PredictionMode pred_mode, EbBool is_intra_bc, EbBool is_encode_pass);

EbErrorType av1_inv_transform_recon(int32_t *coeff_buffer, //1D buffer
                                    uint8_t *recon_buffer_r, uint32_t recon_stride_r,
                                    uint8_t *recon_buffer_w, uint32_t recon_stride_w, TxSize txsize,
                                    uint32_t bit_increment, TxType transform_type,
                                    PlaneType component_type, uint32_t eob, uint8_t lossless);

EbErrorType av1_inv_transform_recon8bit(int32_t *coeff_buffer, //1D buffer
                                        uint8_t *recon_buffer_r, uint32_t recon_stride_r,
                                        uint8_t *recon_buffer_w, uint32_t recon_stride_w,
                                        TxSize txsize, TxType transform_type,
                                        PlaneType component_type, uint32_t eob, uint8_t lossless);

#ifdef __cplusplus
}
#endif

#endif // EbTransforms_h
