/*
 * Copyright (c) 2021-2022, the SerenityOS developers.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/NonnullRefPtrVector.h>
#include <LibGUI/Command.h>
#include <LibGUI/UndoStack.h>
#include <LibGfx/BitmapFont.h>

class UndoGlyph : public RefCounted<UndoGlyph> {
public:
    explicit UndoGlyph(u32 const code_point, Gfx::BitmapFont const& font)
        : m_code_point(code_point)
        , m_font(font)
    {
    }
    NonnullRefPtr<UndoGlyph> save_state() const
    {
        auto state = adopt_ref(*new UndoGlyph(m_code_point, *m_font));
        auto glyph = font().glyph(m_code_point).glyph_bitmap();
        for (int x = 0; x < font().max_glyph_width(); x++)
            for (int y = 0; y < font().glyph_height(); y++)
                state->m_bits[x][y] = glyph.bit_at(x, y);
        state->m_width = glyph.width();
        return state;
    }
    void restore_state(UndoGlyph const& state) const
    {
        auto bitmap = font().glyph(state.m_code_point).glyph_bitmap();
        for (int x = 0; x < font().max_glyph_width(); x++)
            for (int y = 0; y < font().glyph_height(); y++)
                bitmap.set_bit_at(x, y, state.m_bits[x][y]);
        m_restored_width = state.m_width;
        m_restored_code_point = state.m_code_point;
    }
    void set_code_point(u32 code_point) { m_code_point = code_point; }
    void set_font(Gfx::BitmapFont& font) { m_font = font; }
    Gfx::BitmapFont const& font() const { return *m_font; }
    u8 restored_width() const { return m_restored_width; }
    u32 restored_code_point() const { return m_restored_code_point; }

private:
    u32 m_code_point { 0 };
    RefPtr<Gfx::BitmapFont> m_font;
    u8 m_bits[Gfx::GlyphBitmap::max_width()][Gfx::GlyphBitmap::max_height()] {};
    u8 m_width { 0 };
    mutable u8 m_restored_width { 0 };
    mutable u32 m_restored_code_point { 0 };
};

class GlyphUndoCommand : public GUI::Command {
public:
    GlyphUndoCommand(UndoGlyph& glyph)
        : m_undo_state(glyph.save_state())
        , m_undo_glyph(glyph)
    {
    }

    virtual void undo() override
    {
        if (!m_redo_state)
            m_redo_state = m_undo_state->save_state();
        m_undo_glyph.restore_state(*m_undo_state);
    }

    virtual void redo() override
    {
        m_undo_glyph.restore_state(*m_redo_state);
    }

private:
    NonnullRefPtr<UndoGlyph> m_undo_state;
    RefPtr<UndoGlyph> m_redo_state;
    UndoGlyph& m_undo_glyph;
};

class MultipleGlyphsUndoCommand : public GUI::Command {
public:
    MultipleGlyphsUndoCommand(NonnullRefPtrVector<UndoGlyph>& undo_glyphs)
        : m_undo_states(save_state(undo_glyphs))
        , m_undo_glyphs(undo_glyphs)
    {
    }

    virtual void undo() override
    {
        if (m_redo_states.is_empty())
            m_redo_states = save_state(m_undo_glyphs);
        restore_state(m_undo_glyphs, m_undo_states);
    }

    virtual void redo() override
    {
        restore_state(m_undo_glyphs, m_redo_states);
    }

private:
    void restore_state(NonnullRefPtrVector<UndoGlyph> const& glyphs, NonnullRefPtrVector<UndoGlyph> const& new_state);
    NonnullRefPtrVector<UndoGlyph> save_state(NonnullRefPtrVector<UndoGlyph> const& glyphs);

    RefPtr<Gfx::BitmapFont> m_font;
    NonnullRefPtrVector<UndoGlyph> m_undo_states;
    NonnullRefPtrVector<UndoGlyph> m_redo_states;
    NonnullRefPtrVector<UndoGlyph>& m_undo_glyphs;
};
