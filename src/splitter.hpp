#ifndef STRING_SPLITTER_H
#define STRING_SPLITTER_H

#include <string>

template <char Delimiter>
class StringSplitter {

    size_t beg[12];
    size_t end[12];

    const std::string& str;

public:

    size_t length;

    StringSplitter(const std::string& str) : str(str), length(0) {
/*
 *    beg[0]     beg[1]  beg[2]  beg[3]
 *    v          v       v        v
 *  **--------***-------*--------*---------
 *            ^         ^        ^         ^
 *           end[0]    end[1]   end[2]  end[3]
 */
        for (size_t i = 0; i < str.length(); ++i) {
            if (str[i] != Delimiter) {
                assert(length < 9);
                beg[length] = i++;
                for ( ; i < str.length() && str[i] != Delimiter; ++i);
                end[length++] = i;
            }
        }
    }

    std::string operator[](size_t i) const {
        return str.substr(beg[i], end[i] - beg[i]);
    }

};

#endif
