#ifndef PATCH_H
#define PATCH_H

#include <sstream>

namespace patch
{
    template <typename T> std::string to_string(const T& n)
    {
        std::ostringstream stm;
        stm << n;
        return stm.str();
    }

    inline float to_float(const std::string& s)
    {
        std::istringstream stm(s);
        float result;
        stm >> result;
        return result;
    }
}

#endif
