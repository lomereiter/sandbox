#ifndef STRING_SPLITTER_H
#define STRING_SPLITTER_H

#include <cstring>
#include <vector>

template <char Delimiter>
class StringSplitter {
public:

    size_t length;
    char** data;

    StringSplitter(const char* str) : length(0) {
        size_t i;
        std::vector<size_t> beg;
        std::vector<size_t> end;
/*
 *    beg[0]     beg[1]  beg[2]  beg[3]
 *    v          v       v        v
 *  **--------***-------*--------*---------
 *            ^         ^        ^         ^
 *           end[0]    end[1]   end[2]  end[3]
 */
        for (i = 0; str[i] != '\0'; ++i) {
            if (str[i] != Delimiter) {
                beg.push_back(i++);
                for ( ; str[i] != '\0' && str[i] != Delimiter; ++i);
                end.push_back(i);
            }
        }

        length = beg.size();
        data = new char*[length];
          
        for (i = 0; i < length; ++i) {
            size_t len = end[i] - beg[i];
            data[i] = new char[len + 1];
            strncpy(data[i], 
                    static_cast<const char*>(str + beg[i]), 
                    len + 1);
            data[i][len] = '\0';
        }
    }

    ~StringSplitter() {
        for (size_t i = 0; i < length; ++i)
            delete[] data[i];
        delete[] data;
    }
};

#endif
