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



#endif //RAPIDJSON_RAPIDJSON_H_
