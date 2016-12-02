#ifndef RAPIDJSON_RAPIDJSON_H_
#define RAPIDJSON_RAPIDJSON_H_

#include <cstdlib>
#include <cstring>
///////////////////////////////////////////////////////////////
//RAPIDJSON_NAMESPACE
#ifndef RAPIDJSON_NAMESPACE
#define RAPIDJSON_NAMESPACE rapidjson
#endif

#ifndef RAPIDJSON_NAMESPACE_BEGIN
#define RAPIDJSON_NAMESPACE_BEGIN namespace RAPIDJSON_NAMESPACE {
#endif

#ifndef RAPIDJSON_NAMESPACE_END
#define RAPIDJSON_NAMESPACE_END }
#endif



//////////////////////////////////////////////////////////////
#ifndef RAPIDJSON_NO_INT64DEFINE
#include <stdint.h>
#include <inttypes.h>

#ifdef RAPIDJSON_DOXYGEN_RUNNING
#define RAPIDJSON_NO_INT64DEFINE
#endif

#endif

/////////////////////////////////////////////////////////////
//RAPIDJSON_FORCELINE
//This is reserved for  force inline
#define RAPIDJSON_FORCELINE


/////////////////////////////////////////////////////////////
//RAPIDJOSN_ENDIAN
#define RAPIDJSON_LITTLEENDIAN 0
#define RAPIDJSON_BIGENDIAN    1

////////////////////////////////////////////////////////////
//RAPIDJSON_ALIGN
#ifndef RAPIDJSON_ALIGN
#define RAPIDJSON_ALIGN(x) ((x + 3u) & ~3u)
#endif

////////////////////////////////////////////////////////////
//RAPIDJSON_NO_SIZETYPEDEFINE
#ifndef RAPIDJSON_NO_SIZETYPEDEFINE
RAPIDJSON_NAMESPACE_BEGIN

typedef unsigned SizeType;
RAPIDJSON_NAMESPACE_END
#endif

///////////////////////////////////////////////////////////
RAPIDJSON_NAMESPACE_BEGIN
using std::size_t;
RAPIDJSON_NAMESPACE_END

///////////////////////////////////////////////////////////
// RAPIDJSON_ASSERT

#ifndef RAPIDJSON_ASSERT
#include <cassert>
#define RAPIDJSON_ASSERT(x) assert(x)
#endif

///////////////////////////////////////////////////////////
// Helper
#define RAPIDJSON_MULTILINEMACRO_BEGIN do {
#define RAPIDJSON_MULTILINEMACRO_END \
} while((void)0, 0)

///////////////////////////////////////////////////////////
//C++11 feature
#define RAPIDJSON_NOEXCEPT

///////////////////////////////////////////////////////////
// new/delete
#ifndef RAPIDJSON_NEW
#define RAPIDJSON_NEW(x) new x
#endif
#ifndef RAPIDJSON_DELETE
#define RAPIDJSON_DELETE(x) delete x
#endif

///////////////////////////////////////////////////////////
//  Allocators and Encodings
#include "allocators.h"
#include "encodings.h"


///////////////////////////////////////////////////////////
RAPIDJSON_NAMESPACE_BEGIN
template<typename Stream>
struct StreamTraits {
    enum { copyOptimization = 0 };
};

template<typename Stream, typename Ch>
inline void PutN(Stream& stream, Ch c, size_t n) {
    for (size_t i = 0; i < n; i++)
        stream.Put(c);
}

///////////////////////////////////////////////////////////
// StringStream

template <typename Encoding>
struct GenericStringStream {
    typedef typename Encoding::Ch Ch;

    GenericStringStream(const Ch *src) : src_(src), head_(src) {}

    Ch Peek() const { return *src_; }
    Ch Take() { return *src_++; }
    size_t Tell() const { return static_cast<size_t>(src_ - head_); }

    Ch* PutBegin() { RAPIDJSON_ASSERT(false); return 0; }
    void Put(Ch) { RAPIDJSON_ASSERT(false); }
    void Flush() { RAPIDJSON_ASSERT(false); }
    size_t PutEnd(Ch *) { RAPIDJSON_ASSERT(false); return 0; }

    const Ch* src_;     //!< Current read position.
    const Ch* head_;    //!< Original head of the string.
};

template<typename Encoding>
struct StreamTraits<GenericStringStream<Encoding> > {
    enum { copyOptimization = 1 };
};

typedef GenericStringStream<UTF8<> > StringStream;


///////////////////////////////////////////////////////////
// Type
enum Type {
    kNullType   = 0,
    kFalseType  = 1,
    kTrueType   = 2,
    kObjectType = 3,
    kArrayType  = 4,
    kStringType = 5,
    kNumberType = 6
};

RAPIDJSON_NAMESPACE_END


#endif //RAPIDJSON_RAPIDJSON_H_
