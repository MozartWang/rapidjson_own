#ifndef RAPIDJSON_INTERNAL_STRFUNC_H_
#define RAPIDJSON_INTERNAL_STRFUNC_H_

RAPIDJSON_NAMESPACE_BEGIN
namespace internal {
    template<typename Ch>
    inline SizeType StrLen(const Ch* s) {
        const Ch* p = s;
        while (*p) ++p;
        return SizeType(p - s);
    }

} //namespace internal

RAPIDJSON_NAMESPACE_END


#endif//RAPIDJSON_INTERNAL_STRFUNC_H_
