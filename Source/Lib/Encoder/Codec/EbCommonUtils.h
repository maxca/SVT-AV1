/*
* Copyright(c) 2019 Netflix, Inc.
* SPDX - License - Identifier: BSD - 2 - Clause - Patent
*/

#ifndef EbCommonUtils_h
#define EbCommonUtils_h

#include "EbCodingUnit.h"
#include "EbDefinitions.h"
#include "EbSegmentationParams.h"
#include "EbSharedUtils.h"

#define MAX_OFFSET_WIDTH 64
#define MAX_OFFSET_HEIGHT 0

static const int16_t eb_k_eob_group_start[12] = {0, 1, 2, 3, 5, 9, 17, 33, 65, 129, 257, 513};
static const int16_t eb_k_eob_offset_bits[12] = {0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

static const PredictionMode fimode_to_intradir[FILTER_INTRA_MODES] = {
    DC_PRED, V_PRED, H_PRED, D157_PRED, DC_PRED};

static INLINE uint8_t *set_levels(uint8_t *const levels_buf, const int32_t width) {
    return levels_buf + TX_PAD_TOP * (width + TX_PAD_HOR);
}

static INLINE int get_padded_idx(const int idx, const int bwl) {
    return idx + ((idx >> bwl) << TX_PAD_HOR_LOG2);
}

static INLINE int get_txb_bwl(TxSize tx_size) {
    tx_size = av1_get_adjusted_tx_size(tx_size);
    return tx_size_wide_log2[tx_size];
}

static INLINE int get_txb_wide(TxSize tx_size) {
    tx_size = av1_get_adjusted_tx_size(tx_size);
    return tx_size_wide[tx_size];
}

static INLINE int get_txb_high(TxSize tx_size) {
    tx_size = av1_get_adjusted_tx_size(tx_size);
    return tx_size_high[tx_size];
}

static INLINE TxType intra_mode_to_tx_type(const BlockModeInfo *mbmi, PlaneType plane_type) {
    static const TxType _intra_mode_to_tx_type[INTRA_MODES] = {
        DCT_DCT, // DC
        ADST_DCT, // V
        DCT_ADST, // H
        DCT_DCT, // D45
        ADST_ADST, // D135
        ADST_DCT, // D117
        DCT_ADST, // D153
        DCT_ADST, // D207
        ADST_DCT, // D63
        ADST_ADST, // SMOOTH
        ADST_DCT, // SMOOTH_V
        DCT_ADST, // SMOOTH_H
        ADST_ADST, // PAETH
    };
    const PredictionMode mode =
        (plane_type == PLANE_TYPE_Y) ? mbmi->mode : get_uv_mode(mbmi->uv_mode);
    assert(mode < INTRA_MODES);
    return _intra_mode_to_tx_type[mode];
}

static AOM_FORCE_INLINE int get_br_ctx_eob(const int c, // raster order
                                           const int bwl, const TxClass tx_class) {
    const int row = c >> bwl;
    const int col = c - (row << bwl);
    if (c == 0) return 0;
    if ((tx_class == TX_CLASS_2D && row < 2 && col < 2) ||
        (tx_class == TX_CLASS_HORIZ && col == 0) || (tx_class == TX_CLASS_VERT && row == 0))
        return 7;
    return 14;
}

static INLINE int32_t get_br_ctx(const uint8_t *const levels,
                                 const int32_t        c, // raster order
                                 const int32_t bwl, const TxType tx_type) {
    const int32_t row      = c >> bwl;
    const int32_t col      = c - (row << bwl);
    const int32_t stride   = (1 << bwl) + TX_PAD_HOR;
    const TxClass tx_class = tx_type_to_class[tx_type];
    const int32_t pos      = row * stride + col;
    int32_t       mag      = levels[pos + 1];
    mag += levels[pos + stride];
    switch (tx_class) {
    case TX_CLASS_2D:
        mag += levels[pos + stride + 1];
        mag = AOMMIN((mag + 1) >> 1, 6);
        if (c == 0) return mag;
        if ((row < 2) && (col < 2)) return mag + 7;
        break;
    case TX_CLASS_HORIZ:
        mag += levels[pos + 2];
        mag = AOMMIN((mag + 1) >> 1, 6);
        if (c == 0) return mag;
        if (col == 0) return mag + 7;
        break;
    case TX_CLASS_VERT:
        mag += levels[pos + (stride << 1)];
        mag = AOMMIN((mag + 1) >> 1, 6);
        if (c == 0) return mag;
        if (row == 0) return mag + 7;
        break;
    default: break;
    }

    return mag + 14;
}

static INLINE int get_lower_levels_ctx_eob(int bwl, int height, int scan_idx) {
    if (scan_idx == 0) return 0;
    if (scan_idx <= (height << bwl) / 8) return 1;
    if (scan_idx <= (height << bwl) / 4) return 2;
    return 3;
}

#endif //EbCommonUtils_h
