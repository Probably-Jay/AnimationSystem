#pragma once
#include "AnimatedObject.h"
#include "system/string_id.h"


namespace AnimationSystem
{

    // class IReadonlyAnimatedObjectContainer
    // {
    // public:
    //     virtual ~IReadonlyAnimatedObjectContainer() =default;
    //
    //     virtual IAnimatedObjectWrapper * GetIAnimatedObject(const StringId id) const =0;
    //     virtual IAnimatedObjectWrapper * GetIAnimatedObject(std::string const & id) const =0;
    //     virtual bool HasAnimatedObject(const StringId id) const =0;
    //     virtual bool HasAnimatedObject(std::string const & id) const =0;
    // };
    
    class AnimatedObjectContainer
    {
    public:
        CreateEntityResult CreateAnimatedObject(string const& objectNameId, string const& filePath, gef::Platform& platform);
        
        AnimatedObjectWrapper * GetAnimatedObject(std::string const & id) const {return GetAnimatedObject(gef::GetStringId(id));}
        AnimatedObjectWrapper * GetAnimatedObject(const StringId id) const {return GetWrappedValueFromMap(map_, id);}

        bool HasAnimatedObject(const StringId id) const {return GetAnimatedObject(id) != nullptr;}
        bool HasAnimatedObject(std::string const & id) const {return GetAnimatedObject(id) != nullptr;}

        
    private:
        std::map<StringId, unique_ptr<AnimatedObjectWrapper>> map_;
        mutable gef::StringIdTable idTable;
    };
}