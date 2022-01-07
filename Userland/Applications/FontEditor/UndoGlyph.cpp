/*
 * Copyright (c) 2022, the SerenityOS developers.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "UndoGlyph.h"

void MultipleGlyphsUndoCommand::restore_state(NonnullRefPtrVector<UndoGlyph> const& glyphs, NonnullRefPtrVector<UndoGlyph> const& new_state)
{
    VERIFY(glyphs.size() == new_state.size());
    for (size_t s = 0; s < glyphs.size(); s++)
        glyphs[s].restore_state(new_state[s]);
}

NonnullRefPtrVector<UndoGlyph> MultipleGlyphsUndoCommand::save_state(NonnullRefPtrVector<UndoGlyph> const& glyphs)
{
    NonnullRefPtrVector<UndoGlyph> state;
    for (size_t s = 0; s < glyphs.size(); s++)
        state.append(glyphs[s].save_state());
    return state;
}
