#pragma once
#include "PureResult.h"
#include "system/string_id.h"

namespace GefExtensions
{
    static bool HasValue(gef::StringIdTable & idTable, gef::StringId const val)
    {
        std::string _;
        return idTable.Find(val, _);
    }
    static bool HasValue(gef::StringIdTable & idTable, std::string const &val) {return HasValue(idTable, gef::GetStringId(val));}

    /**
     * \brief  Adds the value "val" to "idTable", unless it is already a key, in which case an Error object will be returned
     */
    static AnimationSystem::ValueResult<StringId> TryAddNew(gef::StringIdTable & idTable, std::string const & val)
    {
        if(HasValue(idTable, val))
            return AnimationSystem::ValueResult<StringId>::Error(ERROR_TAG+"String ID '" + val + "' already exists!");
        return AnimationSystem::ValueResult<StringId>::OK(idTable.Add(val));
    }
}
