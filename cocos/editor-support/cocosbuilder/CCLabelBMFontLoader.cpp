#include "CCLabelBMFontLoader.h"

using namespace cocos2d;

namespace cocosbuilder {

#define PROPERTY_COLOR "color"
#define PROPERTY_OPACITY "opacity"
#define PROPERTY_BLENDFUNC "blendFunc"
#define PROPERTY_FNTFILE "fntFile"
#define PROPERTY_STRING "string"

void LabelBMFontLoader::onHandlePropTypeColor3(Node * pNode, Node * pParent, const char * pPropertyName, Color4B pColor4B, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_COLOR) == 0) {
        ((Label *)pNode)->setColor(Color3B(pColor4B));
    } else {
        NodeLoader::onHandlePropTypeColor3(pNode, pParent, pPropertyName, pColor4B, ccbReader);
    }
}

void LabelBMFontLoader::onHandlePropTypeByte(Node * pNode, Node * pParent, const char * pPropertyName, unsigned char pByte, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_OPACITY) == 0) {
        ((Label *)pNode)->setOpacity(pByte);
    } else {
        NodeLoader::onHandlePropTypeByte(pNode, pParent, pPropertyName, pByte, ccbReader);
    }
}

void LabelBMFontLoader::onHandlePropTypeFloat(Node * pNode, Node * pParent, const char* pPropertyName, float pFloat, CCBReader * ccbReader){
    if(strcmp(pPropertyName, PROPERTY_OPACITY) == 0) {
        ((LayerColor *)pNode)->setOpacity(static_cast<GLubyte>(pFloat*255.0));
    } else {
        NodeLoader::onHandlePropTypeFloat(pNode, pParent, pPropertyName, pFloat, ccbReader);
    }
}

void LabelBMFontLoader::onHandlePropTypeBlendFunc(Node * pNode, Node * pParent, const char * pPropertyName, BlendFunc pBlendFunc, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_BLENDFUNC) == 0) {
        ((Label *)pNode)->setBlendFunc(pBlendFunc);
    } else {
        NodeLoader::onHandlePropTypeBlendFunc(pNode, pParent, pPropertyName, pBlendFunc, ccbReader);
    }
}

void LabelBMFontLoader::onHandlePropTypeFntFile(Node * pNode, Node * pParent, const char * pPropertyName, const char* pFntFile, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_FNTFILE) == 0) {
        ((Label *)pNode)->setBMFontFilePath(pFntFile);
    } else {
        NodeLoader::onHandlePropTypeFntFile(pNode, pParent, pPropertyName, pFntFile, ccbReader);
    }
}

void LabelBMFontLoader::onHandlePropTypeText(Node * pNode, Node * pParent, const char * pPropertyName, const char* pText, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_STRING) == 0) {
        ((Label *)pNode)->setString(pText);
    } else {
        NodeLoader::onHandlePropTypeText(pNode, pParent, pPropertyName, pText, ccbReader);
    }
}

}