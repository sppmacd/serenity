/*
 * Copyright (c) 2022, Maciej <sppmacd@pm.me>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/StringView.h>
#include <LibGfx/Filters/ColorFilter.h>

namespace Gfx {

class HSVNoiseFilter : public Filter {
public:
    HSVNoiseFilter(float amount_h, float amount_s, float amount_v)
        : m_amount_h(amount_h)
        , m_amount_s(amount_s)
        , m_amount_v(amount_v)
    {
    }

    virtual ~HSVNoiseFilter() = default;

    virtual StringView class_name() const override { return "HSVNoiseFilter"sv; }
    virtual void apply(Bitmap&, IntRect const&, Bitmap const&, IntRect const&) override;

private:
    float m_amount_h {};
    float m_amount_s {};
    float m_amount_v {};
};

}
