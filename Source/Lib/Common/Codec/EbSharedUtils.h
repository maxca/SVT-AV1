/*
* Copyright(c) 2019 Netflix, Inc.
* SPDX - License - Identifier: BSD - 2 - Clause - Patent
*/

#ifndef EbSharedUtils_h
#define EbSharedUtils_h

#include "EbDefinitions.h"

static INLINE PredictionMode get_uv_mode(UvPredictionMode mode) {
    assert(mode < UV_INTRA_MODES);
    static const PredictionMode uv2y[] = {
            DC_PRED, // UV_DC_PRED
            V_PRED, // UV_V_PRED
            H_PRED, // UV_H_PRED
            D45_PRED, // UV_D45_PRED
            D135_PRED, // UV_D135_PRED
            D113_PRED, // UV_D113_PRED
            D157_PRED, // UV_D157_PRED
            D203_PRED, // UV_D203_PRED
            D67_PRED, // UV_D67_PRED
            SMOOTH_PRED, // UV_SMOOTH_PRED
            SMOOTH_V_PRED, // UV_SMOOTH_V_PRED
            SMOOTH_H_PRED, // UV_SMOOTH_H_PRED
            PAETH_PRED, // UV_PAETH_PRED
            DC_PRED, // UV_CFL_PRED
            INTRA_INVALID, // UV_INTRA_MODES
            INTRA_INVALID, // UV_MODE_INVALID
    };
    return uv2y[mode];
}

static INLINE int32_t is_chroma_reference(int32_t mi_row, int32_t mi_col, BlockSize bsize,
                                          int32_t subsampling_x, int32_t subsampling_y) {
    const int32_t bw      = mi_size_wide[bsize];
    const int32_t bh      = mi_size_high[bsize];
    int32_t       ref_pos = ((mi_row & 0x01) || !(bh & 0x01) || !subsampling_y) &&
                            ((mi_col & 0x01) || !(bw & 0x01) || !subsampling_x);
    return ref_pos;
}

static INLINE int get_segdata(SegmentationParams *seg, int segment_id,
                              SEG_LVL_FEATURES feature_id) {
    return seg->feature_data[segment_id][feature_id];
}

#endif //EbSharedUtils_h