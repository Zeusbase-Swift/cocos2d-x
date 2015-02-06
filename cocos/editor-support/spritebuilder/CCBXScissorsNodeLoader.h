#ifndef _CCB_CCCCSCISSORSNODELOADER_H_
#define _CCB_CCCCSCISSORSNODELOADER_H_

#include "CCBXWidgetLoader.h"

NS_CC_BEGIN
namespace spritebuilder {

/* Forward declaration. */
class CCBReader;

class ScissorsNodeLoader : public WidgetLoader {
public:
    
    static ScissorsNodeLoader *create();
    virtual Node *createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode) override;
    virtual void setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale) override;
    
CC_CONSTRUCTOR_ACCESS:
    ScissorsNodeLoader();
    ~ScissorsNodeLoader();
};

}

NS_CC_END

#endif