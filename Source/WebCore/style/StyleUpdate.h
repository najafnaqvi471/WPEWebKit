/*
 * Copyright (C) 2016 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "Node.h"
#include "StyleChange.h"
#include <wtf/HashMap.h>
#include <wtf/HashSet.h>
#include <wtf/ListHashSet.h>

namespace WebCore {

class ContainerNode;
class Document;
class Element;
class Node;
class RenderStyle;
class Text;

namespace Style {

struct ElementUpdate {
#if !COMPILER_SUPPORTS(NSDMI_FOR_AGGREGATES)
    ElementUpdate() = default;
    ElementUpdate(std::unique_ptr<RenderStyle> style, Change change, bool recompositeLayer)
        : style { WTFMove(style) }
        , change { change }
        , recompositeLayer { recompositeLayer }
    {
    }
#endif
    std::unique_ptr<RenderStyle> style;
    Change change { NoChange };
    bool recompositeLayer { false };
};

struct TextUpdate {
#if !COMPILER_SUPPORTS(NSDMI_FOR_AGGREGATES)
    TextUpdate() = default;
    TextUpdate(unsigned offset, unsigned length)
        : offset { offset }
        , length { length }
    {
    }
#endif

    unsigned offset { 0 };
    unsigned length { std::numeric_limits<unsigned>::max() };
};

class Update {
    WTF_MAKE_FAST_ALLOCATED;
public:
    Update(Document&);

    const ListHashSet<ContainerNode*>& roots() const { return m_roots; }

    const ElementUpdate* elementUpdate(const Element&) const;
    ElementUpdate* elementUpdate(const Element&);

    const TextUpdate* textUpdate(const Text&) const;

    const RenderStyle* elementStyle(const Element&) const;
    RenderStyle* elementStyle(const Element&);

    const Document& document() const { return m_document; }

    unsigned size() const { return m_elements.size() + m_texts.size(); }

    void addElement(Element&, Element* parent, ElementUpdate&&);
    void addText(Text&, Element* parent, TextUpdate&&);
    void addText(Text&, TextUpdate&&);

private:
    void addPossibleRoot(Element*);

    Document& m_document;
    ListHashSet<ContainerNode*> m_roots;
    HashMap<const Element*, ElementUpdate> m_elements;
    HashMap<const Text*, TextUpdate> m_texts;
};

}
}
