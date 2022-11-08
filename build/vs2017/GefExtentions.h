#pragma once
#include "system/string_id.h"

namespace GefExtensions
{
    static bool HasValue(gef::StringIdTable & idTable, gef::StringId const val)
    {
        std::string _;
        return idTable.Find(val, _);
    }
    static bool HasValue(gef::StringIdTable & idTable, std::string const &val) {return HasValue(idTable, gef::GetStringId(val));}
}
