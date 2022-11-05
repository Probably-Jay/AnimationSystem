#pragma once
#include "AnimatedObject.h"
#include "system/string_id.h"


namespace AnimationSystem
{
    using CreateAnimatedObjectResult = ValueResult<std::shared_ptr<IAnimatedObject>>;
  
    class AnimatedObjectFactory
    {
    public:
        CreateAnimatedObjectResult CreateAnimatedObject(string const& objectNameId, string const& filePath, gef::Platform& platform);
        ValueResult<std::reference_wrapper<AnimatedObject>> GetObject(IAnimatedObject const& animatedObject);
        
    private:
        std::map<StringId, std::shared_ptr<AnimatedObject>> map_;
        gef::StringIdTable id_table_;
    };
}