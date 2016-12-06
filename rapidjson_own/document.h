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

template<typename CharType>
inline GenericStringRef<CharType> StringRef(const CharType*str, size_t length) {
    return GenericStringRef<CharType>(str, SizeType(length));
}

////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Encoding, typename Allocator = MemoryPoolAllocator<> >
class GenericValue {
    public:
        typedef GenericMember<Encoding, Allocator> Member;
        typedef Encoding EncodingType;
        typedef Allocator AllocatorType;
        typedef typename Encoding::Ch Ch;
        typedef GenericStringRef<Ch> StringRefType;
        //typedef typename GenericMemberIterator<false, Encoding, Allocator>::Iterator MemberIterator;
        //typedef typename GenericMemberIterator<true, Encoding, Allocator>::Iterator ConstMemberIterator;
        //typedef GenericValue* ValueIterator;
        //typedef const GenericValue* ConstValueIterator;

        GenericValue() : data_(), flags_(kNullFlag) { }
    private:
        GenericValue(const GenericValue& rhs);

    public:
        explicit GenericValue(Type type) :data_(), flags_() {
            static const unsigned defaultFlags[7] = {
                kNullFlag, kFalseFlag, kTrueFlag, kObjectFlag, kArrayFlag, kShortStringFlag,
                kNumberAnyFlag
            };
            RAPIDJSON_ASSERT(type <= kNumberType);
            flags_ = defaultFlags[type];

            if (type == kStringType)
                data_.ss.SetLength(0);

        }

        template <typename SourceAllocator>
        GenericValue(const GenericValue<Encoding, SourceAllocator>& rhs, Allocator& allocator);

        explicit GenericValue(bool b) : data_(), flags_(b ? kTrueFlag : kFalseFlag) { }

        explicit GenericValue(int i) : data_(), flags_(kNumberIntFlag) {
            data_.n.i64 = i;
            if (i >= 0)
                flags_ |= kUintFlag | kUint64Flag;
        }

        explicit GenericValue(unsigned u) : data_(), flags_(kNumberUintFlag) {
            data_.n.u64 = u;
            if (!(u & 0x80000000))
                flags_ |= kIntFlag | kInt64Flag;
        }
        //////////////////////////////
        //
        //
        //////////////////////////////

        explicit GenericValue(double d) : data_(), flags_(kNumberDoubleFlag) { data_.n.d = d; }

        GenericValue(const Ch* s, SizeType length) : data_(), flags_() { SetStringRaw(StringRef(s, length)); }




    private:
        template <typename, typename> friend class GenericValue;
        template <typename, typename, typename> friend class GenericDocument;

        enum {
            kBoolFlag = 0x100,
            kNumberFlag = 0x200,
            kIntFlag = 0x400,
            kUintFlag = 0x800,
            kInt64Flag = 0x1000,
            kUint64Flag = 0x2000,
            kDoubleFlag = 0x4000,
            kStringFlag = 0x100000,
            kCopyFlag = 0x200000,
            kInlineStrFlag = 0x400000,

            kNullFlag = kNullType,
            kTrueFlag = kTrueType | kBoolFlag,
            kFalseFlag = kFalseType | kBoolFlag,
            kNumberIntFlag = kNumberType | kNumberFlag | kIntFlag | kInt64Flag,
            kNumberUintFlag = kNumberType | kNumberFlag | kUintFlag | kUint64Flag | kInt64Flag,
            kNumberInt64Flag = kNumberType | kNumberFlag | kInt64Flag,
            kNumberUint64Flag = kNumberType | kNumberFlag | kUint64Flag,
            kNumberDoubleFlag = kNumberType | kNumberFlag | kDoubleFlag,
            kNumberAnyFlag = kNumberType | kNumberFlag | kIntFlag | kInt64Flag | kUintFlag | kUint64Flag | kDoubleFlag,
            kConstStringFlag = kStringType | kStringFlag,
            kCopyStringFlag = kStringType | kStringFlag | kCopyFlag,
            kShortStringFlag = kStringType | kStringFlag | kCopyFlag | kInlineStrFlag,
            kObjectFlag = kObjectType,
            kArrayFlag = kArrayType,

            kTypeMask = 0xFF
        };

        static const SizeType kDefaultArrayCapacity = 16;
        static const SizeType kDefaultObjectCapacity = 16;

        struct String {
            const Ch* str;
            SizeType length;
            unsigned hashcode;
        };

        struct ShortString {
            enum { MaxChars = sizeof(String) / sizeof(Ch), MaxSize = MaxChars - 1, LenPos = MaxSize};
            Ch str[MaxChars];
            inline static bool  Usable(SizeType len) { return            (MaxSize >= len); }
            inline void      SetLength(SizeType len) { str[LenPos] = (Ch)(MaxSize -  len); }
            inline  SizeType GetLength() const       { return  (SizeType)(MaxSize - str[LenPos]); }
        };

        union Number {
            struct I {
                char padding[4];
                int i;
            }i;
            struct U {
                char padding2[4];
                unsigned u;
            }u;

            int64_t i64;
            uint64_t u64;
            double d;
        };

        struct Object {
            Member* members;
            SizeType size;
            SizeType capacity;
        };

        struct Array {
            GenericValue* elements;
            SizeType size;
            SizeType capacity;
        };

        union Data {
            String s;
            ShortString ss;
            Number n;
            Object o;
            Array a;
        };

        void SetArrayRaw(GenericValue* values, SizeType count, Allocator& allocator) {
            flags_ = kArrayFlag;
            data_.a.elements = (GenericValue*) allocator.Malloc(count * sizeof(GenericValue));
            std::memcpy(data_.a.elements, values, count * sizeof(GenericValue));
            data_.a.size = data_.a.capacity = count;
        }

        void SetObjectRaw() {}



        Data data_;
        unsigned flags_;

};




RAPIDJSON_NAMESPACE_END
#include <new>
