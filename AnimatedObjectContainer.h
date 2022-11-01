#pragma once
#include "AnimatedObject.h"
#include "system/string_id.h"


namespace AnimationSystem
{

    class IReadonlyAnimatedObjectContainer
    {
    public:
        virtual ~IReadonlyAnimatedObjectContainer() =default;

        virtual AnimatedObjectWrapper * GetAnimatedObject(const StringId id) const =0;
        virtual AnimatedObjectWrapper * GetAnimatedObject(std::string const & id) const =0;
        virtual bool AnimatedObject(const StringId id) const =0;
        virtual bool AnimatedObject(std::string const & id) const =0;
    };
    
    class AnimatedObjectContainer :public IReadonlyAnimatedObjectContainer
    {
    public:
        CreateEntityResult CreateAnimatedObject(string const& objectNameId, string const& filePath, gef::Platform& platform);
        
        AnimatedObjectWrapper * GetAnimatedObject(const StringId id) const override {return GetWrappedValueFromMap(map_, id);}
        AnimatedObjectWrapper * GetAnimatedObject(std::string const & id) const override {return GetWrappedValueFromMap(map_, gef::GetStringId(id));}
        bool AnimatedObject(const StringId id) const override {return GetAnimatedObject(id) != nullptr;}
        bool AnimatedObject(std::string const & id) const override {return GetAnimatedObject(id) != nullptr;}
        
    private:
        std::map<StringId, unique_ptr<AnimatedObjectWrapper>> map_;
        mutable gef::StringIdTable idTable;
    };
}