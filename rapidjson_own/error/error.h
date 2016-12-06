#ifndef RAPIDJSON_ERROR_ERROR_H__
#define RAPIDJSON_ERROR_ERROR_H__

#ifndef RAPIDJSON_ERROR_CHARTYPE
#define RAPIDJSON_ERROR_CHARTYPE char
#endif

#ifndef RAPIDJSON_ERROR_STRING
#define RAPIDJSON_ERROR_STRING(x) x
#endif


RAPIDJSON_NAMESPACE_BEGIN

enum ParseErrorCode {
    kParseErrorNone = 0,
    kParseErrorDocumentEmpty,

    kParseErrorDocumentRootNotSingular,         //!< The document root must not follow by other values.

    kParseErrorValueInvalid,                    //!< Invalid value.

    kParseErrorObjectMissName,                  //!< Missing a name for object member.
    kParseErrorObjectMissColon,                 //!< Missing a colon after a name of object member.
    kParseErrorObjectMissCommaOrCurlyBracket,   //!< Missing a comma or '}' after an object member.

    kParseErrorArrayMissCommaOrSquareBracket,   //!< Missing a comma or ']' after an array element.

    kParseErrorStringUnicodeEscapeInvalidHex,   //!< Incorrect hex digit after \\u escape in string.
    kParseErrorStringUnicodeSurrogateInvalid,   //!< The surrogate pair in string is invalid.
    kParseErrorStringEscapeInvalid,             //!< Invalid escape character in string.
    kParseErrorStringMissQuotationMark,         //!< Missing a closing quotation mark in string.
    kParseErrorStringInvalidEncoding,           //!< Invalid encoding in string.

    kParseErrorNumberTooBig,                    //!< Number too big to be stored in double.
    kParseErrorNumberMissFraction,              //!< Miss fraction part in number.
    kParseErrorNumberMissExponent,              //!< Miss exponent in number.

    kParseErrorTermination,                     //!< Parsing was terminated.
    kParseErrorUnspecificSyntaxError            //!< Unspecific syntax error.
};


struct ParseResult {
    ParseResult() : code_(kParseErrorNone), offset_(0) {}
    ParseResult(ParseErrorCode code, size_t offset) : code_(code), offset_(offset) {}

    ParseErrorCode Code() const { return code_; }

    size_t Offset() const { return offset_; }

    operator bool() const { return !IsError(); }
    bool IsError() const { return code_ != kParseErrorNone; }

    bool operator==(const ParseResult& that) const { return code_ == that.code_; }
    bool operator==(ParseErrorCode code) const { return code_ == code ;}
    friend bool operator==(ParseErrorCode code, const ParseResult& err) { return code == err.code_; }

    void Clear() { }
    void Set(ParseErrorCode code, size_t offset = 0) { code_ = code; offset_ = offsert; }

private:
    ParseErrorCode code_;
    size_t offset_;

};


typedef const RAPIDJSON_ERROR_CHARTYPE* (*GetParseErrorFunc)(ParseErrorCode);

RAPIDJSON_NAMESPACE_END

#endif //RAPIDJSON_ERROR_ERROR_H__
