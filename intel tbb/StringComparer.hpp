#ifndef STRINGCOMPARER_H
#define STRINGCOMPARER_H

#include <string>

struct StringComparer 
{
    static size_t hash( const std::string& x ) 
    {
        size_t h = 0;
        for( const char* s = x.c_str(); *s; ++s )
		{
			h = (h*17)^*s;
		}
		return h;
    }
    //! True if strings are equal
    static bool equal( const std::string& x, const std::string& y ) 
    {
        return x==y;
    }
};

#endif