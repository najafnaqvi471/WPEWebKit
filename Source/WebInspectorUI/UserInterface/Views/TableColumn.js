/*
 * Copyright (C) 2017 Apple Inc. All Rights Reserved.
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

WI.TableColumn = class TableColumn extends WI.Object
{
    constructor(identifier, name, {initialWidth, minWidth, maxWidth, hidden, sortable, align, resizeType} = {})
    {
        super();

        console.assert(identifier);
        console.assert(name);
        console.assert(!initialWidth || initialWidth > 0);
        console.assert(!minWidth || minWidth >= 0);
        console.assert(!maxWidth || maxWidth >= 0);

        this._identifier = identifier;
        this._name = name;

        this._width = initialWidth || NaN;
        this._minWidth = minWidth || 50;
        this._maxWidth = maxWidth || 0;
        this._hidden = hidden || false;
        this._defaultHidden = hidden || false;
        this._sortable = typeof sortable === "boolean" ? sortable : true;
        this._align = align || null;
        this._resizeType = resizeType || TableColumn.ResizeType.Auto;

        console.assert(!this._minWidth || !this._maxWidth || this._minWidth <= this._maxWidth, "Invalid min/max", this._minWidth, this._maxWidth);
        console.assert(isNaN(this._width) || !this._minWidth || (this._width >= this._minWidth), "Initial width is less than min", this._width, this._minWidth);
        console.assert(isNaN(this._width) || !this._maxWidth || (this._width <= this._maxWidth), "Initial width is greater than max", this._width, this._maxWidth);
        console.assert(!this.locked || this.width, "A locked column should aways have an initial width");
        console.assert(!this.locked || !this.hidden, "A locked column should never be hidden");
    }

    get identifier() { return this._identifier; }
    get name() { return this._name; }
    get minWidth() { return this._minWidth; }
    get maxWidth() { return this._maxWidth; }
    get defaultHidden() { return this._defaultHidden; }
    get sortable() { return this._sortable; }
    get align() { return this._align; }

    get locked() { return this._resizeType === TableColumn.ResizeType.Locked; }
    get flexible() { return this._resizeType === TableColumn.ResizeType.Auto; }

    get width()
    {
        return this._width;
    }

    set width(width)
    {
        // NOTE: We can't assert this because we resize past the minimum and maximum sizes.
        // If we support horizontal scrolling in the Table then we could assert these.
        // console.assert(isNaN(width) || !this._minWidth || width >= this._minWidth, "New width was less than midWidth.", width, this._minWidth);
        // console.assert(isNaN(width) || !this._maxWidth || width <= this._maxWidth, "New width was greater than maxWidth.", width, this._maxWidth);

        if (this._width === width)
            return;

        this._width = width;

        this.dispatchEventToListeners(WI.TableColumn.Event.WidthDidChange);
    }

    get hidden()
    {
        return this._hidden;
    }

    setHidden(x)
    {
        this._hidden = x;
    }
};

WI.TableColumn.ResizeType = {
    Auto: "auto",
    Locked: "locked",
};

WI.TableColumn.Event = {
    WidthDidChange: "table-column-width-did-change",
};
