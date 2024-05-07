#include <common/String.h>

#include <algorithm>
#include "String.h"

void String::ToLower()
{
    std::transform(begin(), end(), begin(), ::tolower);
}

String::operator std::string()
{
    return *this;
}
