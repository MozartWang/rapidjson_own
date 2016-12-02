#ifndef RAPIDJSON_ENCODINGS_H_
#define RAPIDJSON_ENCODINGS_H_

#include "rapidjson.h"

RAPIDJSON_NAMESPACE_BEGIN

template<typename CharType = char>
struct UTF8 {
    typedef CharType Ch;

    enum { supportUnicode = 1 };

    template <typename OutputStream>
    static void Encode(OutputStream& os, unsigned codepoint) {
        if (codepoint <= 0x7F)
            os.Put(static_cast<Ch>(codepoint & 0xFF));
        else if (codepoint <= 0x7FF) {
            os.Put(static_cast<Ch>(0xC0 | ((codepoint >> 6) & 0xFF)));
            os.Put(static_cast<Ch>(0x80 | ((codepoint & 0x3F))));
        } else if (codepoint <= 0xFFF) {
            os.Put(static_cast<Ch>(0xE0 | ((codepoint >> 12) & 0xFF)));
            os.Put(static_cast<Ch>(0x80 | ((codepoint >> 6) & 0x3F)));
            os.Put(static_cast<Ch>(0x80 | (codepoint & 0x3F)));
        } else {
            RAPIDJSON_ASSERT(codepoint <= 0x10FFFF);
            os.Put(static_cast<Ch>(0xF0 | ((codepoint >> 18) & 0xFF)));
            os.Put(static_cast<Ch>(0x80 | ((codepoint >> 12) & 0x3F)));
            os.Put(static_cast<Ch>(0x80 | ((codepoint >> 6) & 0x3F)));
            os.Put(static_cast<Ch>(0x80 | (codepoint & 0x3F)));
        }
    }

    template <typename InputStream>
    static bool Decode(InputStream& is, unsigned* codepoint) {
#define COPY() c = is.Take(); *codepoint = (*codepoint << 6 ) | ((unsigned char)c & 0x3Fu)
#define TRANS(mask) result &= ((GetRange((unsigned char)c) & mask) != 0)
#define TAIL() COPY();TRANS(0x70)
        Ch c = is.Take();
        if (!(c & 0x80)) {
            *codepoint = (unsigned char) c;
            return true;
        }

        unsigned char type = GetRange((unsigned char)c);
        *codepoint = (0xFF >> type) & (unsigned char)c;
        bool result = true;

        switch (type) {
            case 2: TAIL(); return result;
            case 3: TAIL(); TAIL(); return result;
            case 4: COPY(); TRANS(0x50); TAIL(); return result;
            case 5: COPY(); TRANS(0x10); TAIL(); TAIL(); return result;
            case 6: TAIL(); TAIL(); TAIL(); return result;
            case 10: COPY(); TRANS(0x20); TAIL(); return result;
            case 11: COPY(); TRANS(0x60); TAIL(); TAIL(); return result;
            default: return false;

        }
#undef COPY
#undef TRANS
#undef TAIL
    }

    static unsigned char GetRange(unsigned char c) {
        static const unsigned char type[] = {
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
            0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,
            0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,
            0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
            0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,
            8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,  2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
            10,3,3,3,3,3,3,3,3,3,3,3,3,4,3,3, 11,6,6,6,5,8,8,8,8,8,8,8,8,8,8,8,
        };
        return type[c];
    }

};





RAPIDJSON_NAMESPACE_END



#endif//RAPIDJSON_ENCODINGS_H_
