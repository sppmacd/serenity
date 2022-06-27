/*
 * Copyright (c) 2022, Maciej <sppmacd@pm.me>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include "Filter.h"

namespace PixelPaint::Filters {

class HSVNoise final : public Filter {
public:
    virtual void apply(Gfx::Bitmap& target_bitmap, Gfx::Bitmap const& source_bitmap) const override;
    virtual RefPtr<GUI::Widget> get_settings_widget() override;

    virtual StringView filter_name() override { return "HSV Noise"sv; }

    HSVNoise(ImageEditor* editor)
        : Filter(editor) {};

private:
    int m_amount_h {};
    int m_amount_s {};
    int m_amount_v {};
};

}
