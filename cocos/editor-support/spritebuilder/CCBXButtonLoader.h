#ifndef _CCBX_CCBUTTONLOADER_H_
#define _CCBX_CCBUTTONLOADER_H_

#include "CCBXWidgetLoader.h"

NS_CC_BEGIN

namespace spritebuilder {

class CC_DLL ButtonLoader : public WidgetLoader {
public:
    
    static ButtonLoader *create();
    virtual Node *createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *rootOwner) const override;
    virtual void setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *rootOwner) const override;
    
CC_CONSTRUCTOR_ACCESS:
    ButtonLoader();
    ~ButtonLoader();

protected:
    virtual void onHandlePropTypeText(const std::string &propertyName, bool isExtraProp, const std::string &value) override;
    virtual void onHandlePropTypeFontTTF(const std::string &propertyName, bool isExtraProp, const std::string &value) override;
    virtual void onHandlePropTypeFloat(const std::string &propertyName, bool isExtraProp, float value) override;
    virtual void onHandlePropTypeFloatScale(const std::string &propertyName, bool isExtraProp, const FloatScaleDescription &value) override;
    virtual void onHandlePropTypeSize(const std::string &propertyName, bool isExtraProp, const SizeDescription &value) override;
    virtual void onHandlePropTypeSpriteFrame(const std::string &propertyName, bool isExtraProp, const SpriteFrameDescription &value) override;
    virtual void onHandlePropTypeColor3(const std::string &propertyName, bool isExtraProp, const Color3B &value) override;
    virtual void onHandlePropTypeColor4(const std::string &propertyName, bool isExtraProp, const Color4B &value) override;
    virtual void onHandlePropTypePosition(const std::string &propertyName, bool isExtraProp, const PositionDescription &value) override;
    virtual void onHandlePropTypeIntegerLabeled(const std::string &propertyName, bool isExtraProp, int value) override;
    virtual void onHandlePropTypeCheck(const std::string &propertyName, bool isExtraProp, bool value) override;
    virtual void onHandlePropTypeOffsets(const std::string &propertyName, bool isExtraProp, const Vec4 &value) override;

private:
    Vec4 _margins;
    Vec4 _offset;
    Color4B _outlineColor;
    Color4B _shadowColor;
    FloatScaleDescription _outlineWidth;
    FloatScaleDescription _shadowBlurRadius;
    PositionDescription _shadowOffset;
    float _zoomOnClick;
    
    Color3B _normalLabelColor;
    Color3B _normalBackgroundColor;
    GLubyte _normalLabelOpacity;
    GLubyte _normalBackgroundOpacity;
    
    Color3B _selectedLabelColor;
    Color3B _selectedBackgroundColor;
    GLubyte _selectedLabelOpacity;
    GLubyte _selectedBackgroundOpacity;
    
    Color3B _disabledLabelColor;
    Color3B _disabledBackgroundColor;
    GLubyte _disabledLabelOpacity;
    GLubyte _disabledBackgroundOpacity;
    
    std::string _label;
    std::string _font;
    FloatScaleDescription _fontSize;
    
    FloatScaleDescription _leftPadding;
    FloatScaleDescription _rightPadding;
    FloatScaleDescription _topPadding;
    FloatScaleDescription _bottomPadding;
    
    TextHAlignment _textHAlignment;
    TextVAlignment _textVAlignment;
    bool _adjustsFontSizeToFit;
    
    int _overflowLabel;
    bool _wordWrapLabel;
    
    SpriteFrameDescription _normalSpriteFrame;
    SpriteFrameDescription _selectedSpriteFrame;
    SpriteFrameDescription _disabledSpriteFrame;
    
    FloatScaleDescription _imageScale;
    
};

}

NS_CC_END

#endif
