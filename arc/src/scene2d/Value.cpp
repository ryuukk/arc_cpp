#include "Value.h"
#include "Table.h"

float arc::NewValue::get(arc::Actor* context) {
    switch (type)
    {
        case ValueType::Fixed:
            return value;
        case ValueType::BackgroundTop:
        {
            auto* background = dynamic_cast<Table*>(context)->getBackground();
            return background == nullptr ? 0 : background->getTopHeight();
        }
        case ValueType::BackgroundBottom:
        {
            auto* background = dynamic_cast<Table*>(context)->getBackground();
            return background == nullptr ? 0 : background->getBottomHeight();
        }
        case ValueType::BackgroundLeft:
        {
            auto* background = dynamic_cast<Table*>(context)->getBackground();
            return background == nullptr ? 0 : background->getLeftWidth();
        }
        case ValueType::BackgroundRight:
        {
            auto* background = dynamic_cast<Table*>(context)->getBackground();
            return background == nullptr ? 0 : background->getRightWidth();
        }
        case ValueType::MinWidth:
        {
            if (auto* layout = dynamic_cast<ILayout*>(context))
                return layout->getMinWidth();
            return context == nullptr ? 0 : context->getWidth();
        }
        case ValueType::MinHeight:
        {
            if (auto* layout = dynamic_cast<ILayout*>(context))
                return layout->getMinHeight();
            return context == nullptr ? 0 : context->getHeight();
        }
        case ValueType::PrefWidth:
        {
            if (auto* layout = dynamic_cast<ILayout*>(context))
                return layout->getPrefWidth();
            return context == nullptr ? 0 : context->getWidth();
        }
        case ValueType::PrefHeight:
        {
            if (auto* layout = dynamic_cast<ILayout*>(context))
                return layout->getPrefHeight();
            return context == nullptr ? 0 : context->getHeight();
        }
        case ValueType::MaxWidth:
        {
            if (auto* layout = dynamic_cast<ILayout*>(context))
                return layout->getMaxWidth();
            return context == nullptr ? 0 : context->getWidth();
        }
        case ValueType::MaxHeight:
        {
            if (auto* layout = dynamic_cast<ILayout*>(context))
                return layout->getMaxHeight();
            return context == nullptr ? 0 : context->getHeight();
        }
    }
}

arc::NewValue arc::NewValue::fixed(float value) {
    arc::NewValue ret;
    ret.type = arc::ValueType::Fixed;
    ret.value = value;
    return ret;
}


arc::NewValue arc::NewValue::backgroundTop()
 {
    arc::NewValue ret;
    ret.type = arc::ValueType::BackgroundTop;
    return ret;
 }
arc::NewValue arc::NewValue::backgroundBottom()
 {
    arc::NewValue ret;
    ret.type = arc::ValueType::BackgroundBottom;
    return ret;
 }
arc::NewValue arc::NewValue::backgroundLeft()
 {
    arc::NewValue ret;
    ret.type = arc::ValueType::BackgroundLeft;
    return ret;
 }
arc::NewValue arc::NewValue::backgroundRight()
 {
    arc::NewValue ret;
    ret.type = arc::ValueType::BackgroundRight;
    return ret;
 }
arc::NewValue arc::NewValue::minWidth()
 {
    arc::NewValue ret;
    ret.type = arc::ValueType::MinWidth;
    return ret;
 }
arc::NewValue arc::NewValue::minHeight()
 {
    arc::NewValue ret;
    ret.type = arc::ValueType::MinHeight;
    return ret;
 }
arc::NewValue arc::NewValue::prefWidth()
 {
    arc::NewValue ret;
    ret.type = arc::ValueType::PrefWidth;
    return ret;
 }
arc::NewValue arc::NewValue::prefHeight()
 {
    arc::NewValue ret;
    ret.type = arc::ValueType::PrefHeight;
    return ret;
 }
arc::NewValue arc::NewValue::maxWidth()
 {
    arc::NewValue ret;
    ret.type = arc::ValueType::MaxWidth;
    return ret;
 }
arc::NewValue arc::NewValue::maxHeight()
 {
    arc::NewValue ret;
    ret.type = arc::ValueType::MaxHeight;
    return ret;
 }

arc::NewValue::NewValue() {

}
