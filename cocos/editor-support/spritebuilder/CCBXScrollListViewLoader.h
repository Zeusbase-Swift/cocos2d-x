#ifndef _CCBX_CCSCROLLLISTVIEWLOADER_H_
#define _CCBX_CCSCROLLLISTVIEWLOADER_H_

#include "base/CCRef.h"

#include "CCBXWidgetLoader.h"

NS_CC_BEGIN
namespace spritebuilder {

class CC_DLL ScrollListViewLoader : public WidgetLoader {
public:
    
    static ScrollListViewLoader *create();
    virtual Node *createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *rootOwner) override;
    virtual void setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *rootOwner) override;
    
CC_CONSTRUCTOR_ACCESS:
    ScrollListViewLoader();
    ~ScrollListViewLoader();

protected:
    
    virtual void onHandlePropTypeSize(const std::string &propertyName, bool isExtraProp, const SizeDescription &value) override;
    virtual void onHandlePropTypeCCBFile(const std::string &propertyName, bool isExtraProp, NodeLoader *value) override;
    virtual void onHandlePropTypeCheck(const std::string &propertyName, bool isExtraProp, bool value) override;
    virtual void onHandlePropTypeIntegerLabeled(const std::string &propertyName, bool isExtraProp, int value) override;
    
private:
    bool _horizontal;
    bool _clipping;
    bool _inertial;
    bool _scrollBar;
    bool _bounce;
    int _gravity;
    int _magnetic;
    NodeLoader *_file;
};

}

NS_CC_END

#endif
