/**
 * This file is part of the DOM implementation for KDE.
 *
 * (C) 2001 Peter Kelly (pmk@post.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#include "dom/dom2_views.h"
#include "dom/dom_exception.h"
#include "xml/dom2_eventsimpl.h"

using namespace DOM;

EventListener::EventListener()
{
}

EventListener::~EventListener()
{
}

void EventListener::handleEvent(Event & /*evt*/)
{
}

DOMString EventListener::eventListenerType()
{
    return "";
}

// -----------------------------------------------------------------------------

Event::Event()
{
    impl = 0;
}


Event::Event(const Event &other)
{
    impl = other.impl;
    if(impl)
        impl->ref();
}

Event::Event(EventImpl *i)
{
    impl = i;
    if(impl)
        impl->ref();
}

Event::~Event()
{
    if(impl)
        impl->deref();
}

Event &Event::operator=(const Event &other)
{
    if(impl != other.impl)
    {
        if(impl)
            impl->deref();
        impl = other.impl;
        if(impl)
            impl->ref();
    }
    return *this;
}

DOMString Event::type() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    return impl->type();
}

Node Event::target() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    return impl->target();
}

Node Event::currentTarget() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    return impl->currentTarget();
}

unsigned short Event::eventPhase() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    return impl->eventPhase();
}

bool Event::bubbles() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    return impl->bubbles();
}

bool Event::cancelable() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    return impl->cancelable();
}

DOMTimeStamp Event::timeStamp() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    return impl->timeStamp();
}

void Event::stopPropagation()
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    impl->stopPropagation(true);
}

void Event::preventDefault()
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    impl->preventDefault(true);
}

void Event::initEvent(const DOMString &eventTypeArg, bool canBubbleArg, bool cancelableArg)
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    impl->initEvent(eventTypeArg, canBubbleArg, cancelableArg);
}

EventImpl *Event::handle() const
{
    return impl;
}

bool Event::isNull() const
{
    return (impl == 0);
}

// -----------------------------------------------------------------------------

#ifndef SAVE_SPACE

EventException::EventException(unsigned short _code)
{
    code = _code;
}

EventException::EventException(const EventException &other)
{
    code = other.code;
}

EventException &EventException::operator=(const EventException &other)
{
    code = other.code;
    return *this;
}

#endif

// -----------------------------------------------------------------------------

UIEvent::UIEvent() : Event()
{
}

UIEvent::UIEvent(const UIEvent &other) : Event(other)
{
}

UIEvent::UIEvent(const Event &other) : Event()
{
    (*this) = other;
}

UIEvent::UIEvent(UIEventImpl *impl) : Event(impl)
{
}

UIEvent &UIEvent::operator=(const UIEvent &other)
{
    Event::operator=(other);
    return *this;
}

UIEvent &UIEvent::operator=(const Event &other)
{
    Event e;
    e = other;
    if(!e.isNull() && !e.handle()->isUIEvent())
    {
        if(impl)
            impl->deref();
        impl = 0;
    }
    else
        Event::operator=(other);
    return *this;
}

UIEvent::~UIEvent()
{
}

AbstractView UIEvent::view() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    return static_cast< UIEventImpl * >(impl)->view();
}

long UIEvent::detail() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    return static_cast< UIEventImpl * >(impl)->detail();
}

int UIEvent::keyCode() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    if(impl->isTextInputEvent() || impl->isKeyboardEvent())
        return static_cast< KeyEventBaseImpl * >(impl)->keyCode();

    return 0;
}

int UIEvent::charCode() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    if(impl->isTextInputEvent() || impl->isKeyboardEvent())
        return static_cast< KeyEventBaseImpl * >(impl)->charCode();

    return 0;
}

int UIEvent::pageX() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    if(impl->isMouseEvent())
        return static_cast< MouseEventImpl * >(impl)->pageX();
    else
        return 0;
}

int UIEvent::pageY() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    if(impl->isMouseEvent())
        return static_cast< MouseEventImpl * >(impl)->pageY();
    else
        return 0;
}

int UIEvent::layerX() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    if(impl->isMouseEvent())
        return static_cast< MouseEventImpl * >(impl)->layerX();
    return 0;
}

int UIEvent::layerY() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    if(impl->isMouseEvent())
        return static_cast< MouseEventImpl * >(impl)->layerY();
    return 0;
}

int UIEvent::which() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    if(impl->isMouseEvent())
        return static_cast< MouseEventImpl * >(impl)->button() + 1;
    else if(impl->isTextInputEvent() || impl->isKeyboardEvent())
    {
        // return 0 unless the key has an ascii value
        if(static_cast< KeyEventBaseImpl * >(impl)->keyVal())
            return static_cast< KeyEventBaseImpl * >(impl)->keyCode();
    }

    return 0;
}

void UIEvent::initUIEvent(const DOMString &typeArg, bool canBubbleArg, bool cancelableArg, const AbstractView &viewArg, long detailArg)
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    static_cast< UIEventImpl * >(impl)->initUIEvent(typeArg, canBubbleArg, cancelableArg, viewArg, detailArg);
}

// -----------------------------------------------------------------------------

MouseEvent::MouseEvent() : UIEvent()
{
}

MouseEvent::MouseEvent(const MouseEvent &other) : UIEvent(other)
{
}

MouseEvent::MouseEvent(const Event &other) : UIEvent()
{
    (*this) = other;
}

MouseEvent::MouseEvent(MouseEventImpl *impl) : UIEvent(impl)
{
}

MouseEvent &MouseEvent::operator=(const MouseEvent &other)
{
    UIEvent::operator=(other);
    return *this;
}

MouseEvent &MouseEvent::operator=(const Event &other)
{
    Event e;
    e = other;
    if(!e.isNull() && !e.handle()->isMouseEvent())
    {
        if(impl)
            impl->deref();
        impl = 0;
    }
    else
        UIEvent::operator=(other);
    return *this;
}

MouseEvent::~MouseEvent()
{
}

long MouseEvent::screenX() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    return static_cast< MouseEventImpl * >(impl)->screenX();
}

long MouseEvent::screenY() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    return static_cast< MouseEventImpl * >(impl)->screenY();
}

long MouseEvent::clientX() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    return static_cast< MouseEventImpl * >(impl)->clientX();
}

long MouseEvent::clientY() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    return static_cast< MouseEventImpl * >(impl)->clientY();
}

bool MouseEvent::ctrlKey() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    return static_cast< MouseEventImpl * >(impl)->ctrlKey();
}

bool MouseEvent::shiftKey() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    return static_cast< MouseEventImpl * >(impl)->shiftKey();
}

bool MouseEvent::altKey() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    return static_cast< MouseEventImpl * >(impl)->altKey();
}

bool MouseEvent::metaKey() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    return static_cast< MouseEventImpl * >(impl)->metaKey();
}

unsigned short MouseEvent::button() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    return static_cast< MouseEventImpl * >(impl)->button();
}

Node MouseEvent::relatedTarget() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    return static_cast< MouseEventImpl * >(impl)->relatedTarget();
}

void MouseEvent::initMouseEvent(const DOMString &typeArg, bool canBubbleArg, bool cancelableArg, const AbstractView &viewArg, long detailArg,
                                long screenXArg, long screenYArg, long clientXArg, long clientYArg, bool ctrlKeyArg, bool altKeyArg, bool shiftKeyArg,
                                bool metaKeyArg, unsigned short buttonArg, const Node &relatedTargetArg)
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    static_cast< MouseEventImpl * >(impl)->initMouseEvent(typeArg, canBubbleArg, cancelableArg, viewArg, detailArg, screenXArg, screenYArg,
                                                          clientXArg, clientYArg, ctrlKeyArg, altKeyArg, shiftKeyArg, metaKeyArg, buttonArg,
                                                          relatedTargetArg);
}

// -----------------------------------------------------------------------------

TextEvent::TextEvent() : UIEvent()
{
}

TextEvent::TextEvent(const TextEvent &other) : UIEvent(other)
{
}

TextEvent::TextEvent(const Event &other) : UIEvent()
{
    (*this) = other;
}

TextEvent::TextEvent(KeyEventBaseImpl *impl) : UIEvent(impl)
{
}

TextEvent &TextEvent::operator=(const TextEvent &other)
{
    UIEvent::operator=(other);
    return *this;
}

TextEvent &TextEvent::operator=(const Event &other)
{
    Event e;
    e = other;
    if(!e.isNull() && !(e.handle()->isTextInputEvent() || e.handle()->isKeyboardEvent()))
    {
        if(impl)
            impl->deref();
        impl = 0;
    }
    else
        UIEvent::operator=(other);
    return *this;
}

TextEvent::~TextEvent()
{
}

void TextEvent::initTextEvent(const DOMString &typeArg, bool canBubbleArg, bool cancelableArg, const AbstractView &viewArg, long /*detailArg*/,
                              const DOMString &outputStringArg, unsigned long keyValArg, unsigned long virtKeyValArg, bool /*inputGeneratedArg*/,
                              bool numPadArg)
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    if(impl->isTextInputEvent())
    {
        // Initialize based on the outputStringArg or virtKeyValArg.
        QString text = outputStringArg.string();
        if(outputStringArg.length() == 0 && virtKeyValArg)
        {
            text += QChar((unsigned short)virtKeyValArg);
        }

        TextEventImpl *tImpl = static_cast< TextEventImpl * >(impl);
        tImpl->initTextEvent(typeArg, canBubbleArg, cancelableArg, viewArg, text);
    }
    else
    {
        KeyboardEventImpl *kbImpl = static_cast< KeyboardEventImpl * >(impl);
        kbImpl->initKeyboardEvent(typeArg, canBubbleArg, cancelableArg, viewArg, keyValArg, virtKeyValArg, 0,
                                  numPadArg ? KeyboardEventImpl::DOM_KEY_LOCATION_NUMPAD : KeyboardEventImpl::DOM_KEY_LOCATION_STANDARD);
    }
}

unsigned long TextEvent::keyVal() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    return static_cast< KeyEventBaseImpl * >(impl)->keyVal();
}

DOMString TextEvent::outputString() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    KeyEventBaseImpl *ke = static_cast< KeyEventBaseImpl * >(impl);
    if(ke->isTextInputEvent())
        return static_cast< TextEventImpl * >(ke)->data();
    else
    {
        if(ke->keyVal())
            return QString(QChar((ushort)ke->keyVal()));
        else
            return DOMString();
    }
}

unsigned long TextEvent::virtKeyVal() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    return static_cast< KeyEventBaseImpl * >(impl)->virtKeyVal();
}

void TextEvent::initModifier(unsigned long modifierArg, bool valueArg)
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    return static_cast< KeyEventBaseImpl * >(impl)->initModifier(modifierArg, valueArg);
}

bool TextEvent::checkModifier(unsigned long modifierArg)
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    return static_cast< KeyEventBaseImpl * >(impl)->checkModifier(modifierArg);
}

bool TextEvent::inputGenerated() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    return static_cast< KeyEventBaseImpl * >(impl)->inputGenerated();
}

bool TextEvent::numPad() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    KeyEventBaseImpl *ke = static_cast< KeyEventBaseImpl * >(impl);
    if(ke->isKeyboardEvent())
        return static_cast< KeyboardEventImpl * >(ke)->keyLocation() == KeyboardEventImpl::DOM_KEY_LOCATION_NUMPAD;
    else
        return false;
}
// -----------------------------------------------------------------------------

MutationEvent::MutationEvent() : Event()
{
}

MutationEvent::MutationEvent(const MutationEvent &other) : Event(other)
{
}

MutationEvent::MutationEvent(const Event &other) : Event()
{
    (*this) = other;
}

MutationEvent::MutationEvent(MutationEventImpl *impl) : Event(impl)
{
}

MutationEvent &MutationEvent::operator=(const MutationEvent &other)
{
    Event::operator=(other);
    return *this;
}

MutationEvent &MutationEvent::operator=(const Event &other)
{
    Event e;
    e = other;
    if(!e.isNull() && !e.handle()->isMutationEvent())
    {
        if(impl)
            impl->deref();
        impl = 0;
    }
    else
        Event::operator=(other);
    return *this;
}

MutationEvent::~MutationEvent()
{
}

Node MutationEvent::relatedNode() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    return static_cast< MutationEventImpl * >(impl)->relatedNode();
}

DOMString MutationEvent::prevValue() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    return static_cast< MutationEventImpl * >(impl)->prevValue();
}

DOMString MutationEvent::newValue() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    return static_cast< MutationEventImpl * >(impl)->newValue();
}

DOMString MutationEvent::attrName() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    return static_cast< MutationEventImpl * >(impl)->attrName();
}

unsigned short MutationEvent::attrChange() const
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    return static_cast< MutationEventImpl * >(impl)->attrChange();
}

void MutationEvent::initMutationEvent(const DOMString &typeArg, bool canBubbleArg, bool cancelableArg, const Node &relatedNodeArg,
                                      const DOMString &prevValueArg, const DOMString &newValueArg, const DOMString &attrNameArg,
                                      unsigned short attrChangeArg)
{
    if(!impl)
        throw DOMException(DOMException::INVALID_STATE_ERR);

    static_cast< MutationEventImpl * >(impl)->initMutationEvent(typeArg, canBubbleArg, cancelableArg, relatedNodeArg, prevValueArg, newValueArg,
                                                                attrNameArg, attrChangeArg);
}
