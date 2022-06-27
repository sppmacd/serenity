/*
 * Copyright (c) 2022, Maciej <sppmacd@pm.me>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "HSVNoiseFilter.h"

#include <AK/NumericLimits.h>
#include <AK/Random.h>

namespace Gfx {

void HSVNoiseFilter::apply(Bitmap& target_bitmap, IntRect const& target_rect, Bitmap const& source_bitmap, IntRect const& source_rect)
{
    auto random_double = [](double amount) {
        return (get_random<u32>() * 2 * amount / NumericLimits<u32>::max()) - amount;
    };

    for (auto y = 0; y < source_rect.height(); ++y) {
        ssize_t source_y = y + source_rect.y();
        ssize_t target_y = y + target_rect.y();
        for (auto x = 0; x < source_rect.width(); ++x) {
            ssize_t source_x = x + source_rect.x();
            ssize_t target_x = x + target_rect.x();

            auto source_pixel = source_bitmap.get_pixel(source_x, source_y);
            auto hsv = source_pixel.to_hsv();

            hsv.hue = clamp(hsv.hue + random_double(m_amount_h), 0, 359);
            hsv.saturation = clamp(hsv.saturation + random_double(m_amount_s), 0, 1);
            hsv.value = clamp(hsv.value + random_double(m_amount_v), 0, 1);

            target_bitmap.set_pixel(target_x, target_y, Color::from_hsv(hsv));
        }
    }
}

}
