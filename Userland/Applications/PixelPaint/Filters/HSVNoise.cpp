/*
 * Copyright (c) 2022, Maciej <sppmacd@pm.me>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "HSVNoise.h"
#include "../FilterParams.h"

#include <AK/NumericLimits.h>
#include <LibGUI/Label.h>
#include <LibGUI/SpinBox.h>
#include <LibGUI/ValueSlider.h>
#include <LibGfx/Filters/HSVNoiseFilter.h>

namespace PixelPaint::Filters {

void HSVNoise::apply(Gfx::Bitmap& target_bitmap, Gfx::Bitmap const& source_bitmap) const
{
    Gfx::HSVNoiseFilter filter { static_cast<float>(m_amount_h), m_amount_s / 100.f, m_amount_v / 100.f };
    filter.apply(target_bitmap, target_bitmap.rect(), source_bitmap, source_bitmap.rect());
}

RefPtr<GUI::Widget> HSVNoise::get_settings_widget()
{
    if (!m_settings_widget) {
        m_settings_widget = GUI::Widget::construct();
        m_settings_widget->set_layout<GUI::VerticalBoxLayout>();

        auto& name_label = m_settings_widget->add<GUI::Label>("HSV Noise");
        name_label.set_font_weight(Gfx::FontWeight::Bold);
        name_label.set_text_alignment(Gfx::TextAlignment::CenterLeft);
        name_label.set_fixed_height(20);

        auto add_component_slider = [&](StringView label_string, auto& target, double max) {
            auto& container = m_settings_widget->add<GUI::Widget>();
            container.set_layout<GUI::HorizontalBoxLayout>();
            container.layout()->set_margins({ 4, 0, 4, 0 });

            auto& label = container.add<GUI::Label>(label_string);
            label.set_text_alignment(Gfx::TextAlignment::CenterLeft);
            label.set_fixed_height(20);

            auto& slider = container.add<GUI::ValueSlider>(Orientation::Horizontal);
            slider.set_range(0, max);
            slider.set_value(target);
            slider.on_change = [&](int value) {
                target = value;
                update_preview();
            };
        };

        add_component_slider("Amount H:", m_amount_h, 359);
        add_component_slider("Amount S:", m_amount_s, 100);
        add_component_slider("Amount V:", m_amount_v, 100);
    }

    return m_settings_widget;
}

}
