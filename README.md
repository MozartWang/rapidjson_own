This project just for learning.

add something. test.




/////////////////////////////////////////////////////////////
Something Need to study.

reader.h
LINE 248-- internal::StreamLocalCopy<InputStream> copy(is); 模板解析规则，需要去熟悉下模板匹配。

document.h
1865     template <typename,typename,typename> friend class GenericReader; // for parsing
1866     template <typename, typename> friend class GenericValue; // for deep copying
需要去熟悉下模板友元函数

//隐式类型转换
1615     std::memcpy(str, s, s.length * sizeof(Ch));
319      operator const Ch *() const { return s; }

