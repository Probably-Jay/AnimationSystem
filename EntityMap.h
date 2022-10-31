// #pragma once
// #include "ViewingProtectedWrapper.h"
// #include "system/string_id.h"
// using std::unique_ptr;
// namespace AnimationSystem
// {
//     template <class T>
//     class EntityMap
//     {
//     public:
//         
//         template <typename ...Vals>
//         void Emplace(Vals&&... vals) {map_.emplace(std::forward<Vals>(vals)...);}
//         
//         T * GetWrappedValue(unsigned key) const
//         {
//             const auto iter = map_.find(key);
//             if(iter == map_.end())
//                 return nullptr;
//             return iter->second.get();
//         }
//         T * GetWrappedValue(const std::string & key) const
//         {
//            return GetWrappedValue(gef::GetStringId(key));
//         }
//     private:
//         const std::map<StringId, unique_ptr<T>> map_;
//     };
// }