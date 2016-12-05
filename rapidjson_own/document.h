#include "reader.h"
#include "internal/strfunc.h"

RAPIDJSON_NAMESPACE_BEGIN

template <typename Encoding, typename Allocator>
class GenericValue;

template <typename Encoding, typename Allocator>
struct GenericMember {
    GenericValue<Encoding, Allocator> name;
    GenericValue<Encoding, Allocator> value;
};

template <typename CharType>
struct GenericStringRef {

    typedef CharType Ch;

    template<SizeType N>
    GenericStringRef(const CharType (&str)[N]) RAPIDJSON_NOEXCEPT
        :s(str), length(N-1) { }

    explicit GenericStringRef(const CharType* str)
        :s(str), length(internal::StrLen(str)) { RAPIDJSON_ASSERT(s != NULL); }
    GenericStringRef(const CharType* str, SizeType len)
        :s(str), length(len) { RAPIDJSON_ASSERT( s != NULL);}

    operator const Ch* () const { return s; }


    const Ch* const s;
    const SizeType length;
private:
    GenericStringRef operator=(const GenericStringRef&);

    template<SizeType N>
    GenericStringRef(CharType (&str)[N]);
};

template<typename CharType>
inline GenericStringRef<CharType> StringRef(const CharType* str) {
    return GenericStringRef<CharType>(str, internal::StrLen(str));
}




RAPIDJSON_NAMESPACE_END
#include <new>
