//
//  CCBXNodeLoader.cpp
//  cocos2d_libs
//
//  Created by Sergey on 29.10.14.
//
//

#include "CCBXNodeLoader.h"
#include "CCBXSequence.h"
#include "CCBXAnimationManager.h"

NS_CC_BEGIN

namespace spritebuilder {
    
Vec2 getAbsoluteScale(float mainScale, float additionalScale, float scaleX, float scaleY, unsigned int type)
{
    
    float scaleXCoef = 1.0;
    float scaleYCoef = 1.0;
    
    if (type & static_cast<int>(ScaleType::MULTIPLY_RESOURCES_SCALE))
    {
        scaleXCoef *= CCBXReader::getResolutionScale();
        scaleYCoef *= CCBXReader::getResolutionScale();
    }
    
    if (type & static_cast<int>(ScaleType::MULTIPLY_MAIN_SCALE))
    {
        scaleXCoef *= mainScale;
        scaleYCoef *= mainScale;
    }
    
    if (type & static_cast<int>(ScaleType::MULTIPLY_ADDITION_SCALE_X))
    {
        scaleXCoef *= additionalScale;
    }
    
    if (type & static_cast<int>(ScaleType::MULTIPLY_ADDITION_SCALE_Y))
    {
        scaleYCoef *= additionalScale;
    }
    
    if (type & static_cast<int>(ScaleType::INVERT_SCALE))
    {
        scaleXCoef = 1.0f/scaleXCoef;
        scaleYCoef = 1.0f/scaleYCoef;
    }
    
    return Vec2(scaleX*scaleXCoef,scaleY*scaleYCoef);
}
    
float getAbsoluteScale(float mainScale, float additionalScale, float scale, unsigned int type)
{
    
    float scaleCoef = CCBXReader::getResolutionScale();
    
    if (type & static_cast<int>(ScaleType::MULTIPLY_RESOURCES_SCALE))
    {
        scaleCoef *= CCBXReader::getResolutionScale();
    }
    
    if (type & static_cast<int>(ScaleType::MULTIPLY_MAIN_SCALE))
    {
        scaleCoef *= mainScale;
    }
    
    if (type & static_cast<int>(ScaleType::INVERT_SCALE))
    {
        scaleCoef = 1.0f/scaleCoef;
    }
    
    return scale * scaleCoef;
}
    
Vec2 getAbsolutePosition(float mainScale, float additionalScale, const Point &pt, PositionReferenceCorner corner, PositionUnit xUnit, PositionUnit yUnit, const Size &containerSize)
{
    Point positionInPoints=Point(0,0);
    float x = 0;
    float y = 0;
    
    // Convert position to points
    if (xUnit == PositionUnit::POINTS) x = pt.x * CCBXReader::getResolutionScale();
    else if (xUnit == PositionUnit::UIPOINTS) x = pt.x * mainScale * CCBXReader::getResolutionScale();
    else if (xUnit == PositionUnit::NORMALIZED) x = pt.x * containerSize.width;
    
    if (yUnit == PositionUnit::POINTS) y = pt.y * CCBXReader::getResolutionScale();
    else if (yUnit == PositionUnit::UIPOINTS) y = pt.y * mainScale * CCBXReader::getResolutionScale();
    else if (yUnit == PositionUnit::NORMALIZED) y = pt.y * containerSize.height;
    
    // Account for reference corner
    //CCPositionReferenceCorner corner = type.corner;
    if (corner == PositionReferenceCorner::BOTTOMLEFT)
    {
        // Nothing needs to be done
    }
    else if (corner == PositionReferenceCorner::TOPLEFT)
    {
        // Reverse y-axis
        y = containerSize.height - y;
    }
    else if (corner == PositionReferenceCorner::TOPRIGHT)
    {
        // Reverse x-axis and y-axis
        x = containerSize.width - x;
        y = containerSize.height - y;
    }
    else if (corner == PositionReferenceCorner::BOTTOMRIGHT)
    {
        // Reverse x-axis
        x = containerSize.width - x;
    }
    
    positionInPoints.x = x;
    positionInPoints.y = y;
    
    return positionInPoints;
}

Size getAbsoluteSize(float mainScale, float additionalScale, const Size &contentSize, SizeUnit widthUnit, SizeUnit heightUnit, const Size &containerSize)
{
    Size size = Size::ZERO;
    
    // Width
    if (widthUnit == SizeUnit::POINTS)
    {
        size.width = CCBXReader::getResolutionScale() * contentSize.width;
    }
    else if (widthUnit == SizeUnit::UIPOINTS)
    {
        size.width = CCBXReader::getResolutionScale() * mainScale * contentSize.width;
    }
    else if (widthUnit == SizeUnit::NORMALIZED)
    {
        size.width = contentSize.width * containerSize.width;
    }
    else if (widthUnit == SizeUnit::INSETPOINTS)
    {
        size.width = containerSize.width - contentSize.width * CCBXReader::getResolutionScale();
    }
    else if (widthUnit == SizeUnit::INSETUIPOINTS)
    {
        size.width = containerSize.width - contentSize.width * CCBXReader::getResolutionScale() * mainScale;
    }
    
    // Height
    if (heightUnit == SizeUnit::POINTS)
    {
        size.height = CCBXReader::getResolutionScale() * contentSize.height;
    }
    else if (heightUnit == SizeUnit::UIPOINTS)
    {
        size.height = CCBXReader::getResolutionScale() * mainScale * contentSize.height;
    }
    else if (heightUnit == SizeUnit::NORMALIZED)
    {
        size.height = contentSize.height * containerSize.height;
    }
    else if (heightUnit == SizeUnit::INSETPOINTS)
    {
        size.height = containerSize.height - contentSize.height * CCBXReader::getResolutionScale();
    }
    else if (heightUnit == SizeUnit::INSETUIPOINTS)
    {
        size.height = containerSize.height - contentSize.height * CCBXReader::getResolutionScale() * mainScale;
    }
    
    return size;
}


#define ASSERT_FAIL_UNEXPECTED_PROPERTY(PROPERTY) log("Unexpected property: '%s'!\n", PROPERTY.c_str()); assert(false)

static std::string PROPERTY_POSITION("position");
static std::string PROPERTY_CONTENTSIZE("contentSize");
static std::string PROPERTY_SKEW("skew");
static std::string PROPERTY_ANCHORPOINT("anchorPoint");
static std::string PROPERTY_SCALE("scale");
static std::string PROPERTY_ROTATION("rotation");
static std::string PROPERTY_TAG("tag");
static std::string PROPERTY_VISIBLE("visible");
static std::string PROPERTY_NAME("name");
static std::string PROPERTY_COLOR("color");
static std::string PROPERTY_CASCADECOLOR("cascadeColorEnabled");
static std::string PROPERTY_OPACITY("opacity");
static std::string PROPERTY_CASCADEOPACITY("cascadeOpacityEnabled");

NodeLoader *NodeLoader::create()
{
    NodeLoader *ret = new NodeLoader();
    ret->autorelease();
    return ret;
}
    
Node *NodeLoader::createNode(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, CCBAnimationManager *manager,  Node *rootNode, const CreateNodeFunction &createNodeFunction, const std::function<void(cocos2d::Node*, AnimationCompleteType)> &defaultAnimationCallback)
{
    Node *ret;
    if(createNodeFunction)
        ret = createNodeFunction(parentSize, mainScale, additionalScale);
    else
        ret = createNodeInstance(parentSize, mainScale, additionalScale, owner, rootNode);
    if(!ret)
        return nullptr;
    
    if(!loadNode(ret, parentSize, mainScale, additionalScale, owner, manager, rootNode, defaultAnimationCallback))
        return nullptr;

    return ret;
}
    
bool NodeLoader::loadNode(Node *node, const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, CCBAnimationManager *manager, Node *rootNode, const std::function<void(cocos2d::Node*, AnimationCompleteType)> &defaultAnimationCallback)
{
    if(!node)
        return false;
    setProperties(node, parentSize, mainScale, additionalScale);
    setCallbacks(node, owner, rootNode);
    if(!_memberVarAssignmentName.empty())
    {
        switch (_memberVarAssignmentType) {
            case TargetType::NONE:
                CCLOG("no assigment type for name:%s", _memberVarAssignmentName.c_str());
                break;
                
            case TargetType::DOCUMENT_ROOT:
            {
                CCBXReaderOwner *rootOwner = dynamic_cast<CCBXReaderOwner*>(rootNode);
                if(rootOwner)
                {
                    if(!rootOwner->onAssignCCBMemberVariable(_memberVarAssignmentName, node))
                    {
                        CCLOG("variable not assigned for name:%s", _memberVarAssignmentName.c_str());
                    }
                }
                else
                {
                    CCLOG("assigment document_root but root node is not CCBXReaderOwner for name:%s", _memberVarAssignmentName.c_str());
                }
            }
                break;
                
            case TargetType::OWNER:
                if(owner)
                {
                    if(!owner->onAssignCCBMemberVariable(_memberVarAssignmentName, node))
                    {
                        CCLOG("variable not assigned for name:%s", _memberVarAssignmentName.c_str());
                    }
                }
                else
                {
                    CCLOG("assigment type owner but no owner for name:%s", _memberVarAssignmentName.c_str());
                }
                break;
        }
    }
    else
    {
        if(_memberVarAssignmentType != TargetType::NONE)
        {
            CCLOG("variable name set but no assigment type for name:%s", _memberVarAssignmentName.c_str());
        }
    }
    
    if(!rootNode)
    {
        rootNode = node;
    }
    
    if(manager == nullptr)
    {
        manager = new CCBAnimationManager(mainScale, additionalScale, node ,owner);
        manager->setSequences(_sequences);
        manager->setAutoPlaySequenceId(_autoPlaySequenceId);
        node->setUserObject(manager);
        manager->release();
    }
    for(const auto &it:_baseValues)
    {
        manager->setBaseValue(it.second, node, it.first);
    }
    setAnimation(node, manager);
#if CC_USE_PHYSICS
    if(_physicsLoader)
        node->setPhysicsBody(_physicsLoader->createBody(node));
#endif
    for(auto child:_children)
    {
        node->addChild(child->createNode(node->getContentSize(), mainScale, additionalScale, owner, manager, rootNode));
    }
    if(rootNode)
    {
        if (node && _autoPlaySequenceId != -1)
        {
            // Auto play animations
            manager->runAnimationsForSequenceIdTweenDuration(_autoPlaySequenceId, 0, defaultAnimationCallback);
        }
        else if (defaultAnimationCallback)
        {
            rootNode->scheduleOnce([rootNode,defaultAnimationCallback](float) { defaultAnimationCallback(rootNode, AnimationCompleteType::COMPLETED); }, 0, "defaultAnimationCallback");
        }
    }
    
    return true;
}

void NodeLoader::setProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale)
{
    node->setVisible(_visible);
    node->setPosition(getAbsolutePosition(mainScale, additionalScale, _position.pos, _position.referenceCorner, _position.xUnits , _position.yUnits, parentSize));
    cocos2d::Vec2 scales = getAbsoluteScale(mainScale, additionalScale, _scale.xScale, _scale.yScale, _scale.type);
    if(_anchorPointLoaded)
        node->setAnchorPoint(_anchorPoint);
    if(_sizeLoaded)
        node->setContentSize(getAbsoluteSize(mainScale, additionalScale, _size.size, _size.widthUnits, _size.heightUnits, parentSize));
    node->setScaleX(scales.x);
    node->setScaleY(scales.y);
    node->setRotation(_rotation);
    node->setTag(_tag);
    node->setName(_name);
    node->setCascadeColorEnabled(_cascadeColorEnabled);
    node->setCascadeOpacityEnabled(_cascadeOpacityEnabled);
    node->setColor(_color);
    node->setOpacity(_opacity);
    node->setSkewX(_skew.x);
    node->setSkewY(_skew.y);
    setSpecialProperties(node, parentSize, mainScale, additionalScale);
}
    
void NodeLoader::setAnimation(Node* node, CCBAnimationManager *manager)
{
    manager->addNode(node, _nodeSequences);
}

Node *NodeLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode)
{
    return Node::create();
}
    
void NodeLoader::setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale)
{
    
}
    
void NodeLoader::setCallbacks(Node* node, CCBXReaderOwner *owner, Node *rootNode)
{
    
}
    
void NodeLoader::setMemberVarAssignment(TargetType type, const std::string &name)
{
    _memberVarAssignmentType = type;
    _memberVarAssignmentName = name;
}
    
void NodeLoader::setPhysicsLoader(PhysicsBodyLoader *loader)
{
    CC_SAFE_RELEASE(_physicsLoader);
    _physicsLoader = loader;
    CC_SAFE_RETAIN(_physicsLoader);
}

void NodeLoader::addChild(NodeLoader *loader)
{
    _children.pushBack(loader);
}
    
void NodeLoader::setSequences(const Vector<CCBSequence*> &sequences)
{
    _sequences = sequences;
}
    
void NodeLoader::setAutoPlaySequenceId(int id)
{
    _autoPlaySequenceId = id;
}
    
void NodeLoader::setBaseValues(const std::unordered_map<std::string, cocos2d::Value> &values)
{
    _baseValues = values;
}
    
void NodeLoader::setObjects(const std::unordered_map<std::string, cocos2d::Ref*> &objects)
{
    _objects = objects;
}
    
void NodeLoader::setNodeSequences(const std::unordered_map<int, Map<std::string, CCBSequenceProperty*>> &sequences)
{
    _nodeSequences = sequences;
}
    
NodeLoader::NodeLoader()
    :_position(PositionDescription{PositionReferenceCorner::BOTTOMLEFT, PositionUnit::POINTS, PositionUnit::POINTS, Vec2(0, 0)})
    ,_anchorPointLoaded(false)
    ,_anchorPoint(0,0)
    ,_sizeLoaded(false)
	,_size(SizeDescription{SizeUnit::POINTS, SizeUnit::POINTS, Size(0, 0)})
	,_scale(ScaleDescription{0, 1.0f, 1.0f})
    ,_opacity(255)
    ,_rotation(0)
    ,_skew(0,0)
    ,_tag(0)
    ,_visible(true)
    ,_cascadeColorEnabled(false)
    ,_cascadeOpacityEnabled(false)
    ,_color(Color3B::WHITE)
    ,_memberVarAssignmentType(TargetType::NONE)
    ,_physicsLoader(nullptr)
    ,_autoPlaySequenceId(-1)
{
}

NodeLoader::~NodeLoader()
{
     CC_SAFE_RELEASE(_physicsLoader);
}

void NodeLoader::onHandlePropTypePosition(const std::string &propertyName, bool isExtraProp, const PositionDescription &value)
{
    if(propertyName == PROPERTY_POSITION) {
        _position = value;
    } else {
        ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
    }
}
    
void NodeLoader::onHandlePropTypePoint(const std::string &propertyName, bool isExtraProp, const Point &value)
{
    if(propertyName == PROPERTY_ANCHORPOINT) {
        _anchorPoint = value;
        _anchorPointLoaded = true;
    } else {
        ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
    }
}

void NodeLoader::onHandlePropTypeSize(const std::string &propertyName, bool isExtraProp, const SizeDescription &value)
{
    if(propertyName == PROPERTY_CONTENTSIZE) {
        _size = value;
        _sizeLoaded = true;
    } else {
        ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
    }
}
    
void NodeLoader::onHandlePropTypeScale(const std::string &propertyName, bool isExtraProp, const ScaleDescription &value)
{
    if(propertyName == PROPERTY_SCALE) {
        _scale = value;
    } else {
        ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
    }
}

void NodeLoader::onHandlePropTypeFloat(const std::string &propertyName, bool isExtraProp, float value)
{
    if(propertyName == PROPERTY_OPACITY) {
        float opacity = value * 255.0f;
        _opacity = (opacity<0.0f)?0:((opacity>255.0f)?255:static_cast<GLubyte>(opacity));
    } else {
        // It may be a custom property, add it to custom property dictionary.
        if(isExtraProp){
            _customProperties[propertyName] = Value(value);
        }
        else {
            ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
        }
    }
}
    
void NodeLoader::onHandlePropTypeDegrees(const std::string &propertyName, bool isExtraProp, float value)
{
    if(propertyName == PROPERTY_ROTATION) {
        _rotation = value;
    } else {
        ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
    }
}
    
void NodeLoader::onHandlePropTypeFloatScale(const std::string &propertyName, bool isExtraProp, const FloatScaleDescription &value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}

void NodeLoader::onHandlePropTypeInteger(const std::string &propertyName, bool isExtraProp, int value)
{
    if(propertyName == PROPERTY_TAG) {
        _tag = value;
    } else {
        // It may be a custom property, add it to custom property dictionary.
        if(isExtraProp){
            _customProperties[propertyName] = Value(value);
        }
        else {
            ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
        }
    }
}

void NodeLoader::onHandlePropTypeIntegerLabeled(const std::string &propertyName, bool isExtraProp, int value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}

void NodeLoader::onHandlePropTypeFloatVar(const std::string &propertyName, bool isExtraProp, const Vec2 &value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}


void NodeLoader::onHandlePropTypeCheck(const std::string &propertyName, bool isExtraProp, bool value)
{
    if(propertyName == PROPERTY_VISIBLE) {
        _visible = value;
    }  else if(propertyName == PROPERTY_CASCADECOLOR) {
        _cascadeColorEnabled = value;
    } else if(propertyName == PROPERTY_CASCADEOPACITY) {
        _cascadeOpacityEnabled = value;
    } else {
        // It may be a custom property, add it to custom property dictionary.
        if(isExtraProp){
            _customProperties[propertyName] = Value(value);
        }
        else {
            ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
        }
    }
}

void NodeLoader::onHandlePropTypeSpriteFrame(const std::string &propertyName, bool isExtraProp, const SpriteFrameDescription &value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}

void NodeLoader::onHandlePropTypeTexture(const std::string &propertyName, bool isExtraProp, Texture2D * value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}

void NodeLoader::onHandlePropTypeColor3(const std::string &propertyName, bool isExtraProp, const Color3B &value)
{
    if(propertyName == PROPERTY_COLOR) {
        _color = value;
    }
    else {
        ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
    }
}

void NodeLoader::onHandlePropTypeColor4FVar(const std::string &propertyName, bool isExtraProp, const std::pair<Color4F, Color4F> &value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}
    
void NodeLoader::onHandlePropTypeFlip(const std::string &propertyName, bool isExtraProp, const std::pair<bool,bool> &flip)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}

void NodeLoader::onHandlePropTypeColor4(const std::string &propertyName, bool isExtraProp, const Color4B &value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}

void NodeLoader::onHandlePropTypeBlendFunc(const std::string &propertyName, bool isExtraProp, const BlendFunc &value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}

void NodeLoader::onHandlePropTypeFntFile(const std::string &propertyName, bool isExtraProp, const std::string &value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}

void NodeLoader::onHandlePropTypeText(const std::string &propertyName, bool isExtraProp, const std::string &value)
{
    if(propertyName == PROPERTY_NAME) {
        _name = value;
    } else
    {
        // It may be a custom property, add it to custom property dictionary.
        if(isExtraProp){
            _customProperties[propertyName] = Value(value);
        }
        else {
            ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
        }
    }
}

void NodeLoader::onHandlePropTypeFontTTF(const std::string &propertyName, bool isExtraProp, const std::string &value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}
    
void NodeLoader::onHandlePropTypeClickCallback(const std::string &propertyName, bool isExtraProp, const CallbackDescription &value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}

void NodeLoader::onHandlePropTypeTouchCallback(const std::string &propertyName, bool isExtraProp, const CallbackDescription &value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}

void NodeLoader::onHandlePropTypeCCBFile(const std::string &propertyName, bool isExtraProp, NodeLoader *value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}
    
void NodeLoader::onHandlePropTypeFloatXY(const std::string &propertyName, bool isExtraProp, const Vec2 &value)
{
    if(propertyName == PROPERTY_SKEW) {
        _skew = value;
    }
    else {
        ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
    }
}
    
void NodeLoader::onHandlePropTypeSoundFile(const std::string &propertyName, bool isExtraProp, const std::string &value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}

    
}

NS_CC_END
