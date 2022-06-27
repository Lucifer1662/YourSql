#pragma once
#include <string>


// trim from left
inline std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v")
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

// trim from right
inline std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v")
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

// trim from left & right
inline std::string& trim(std::string& s, const char* t = " \t\n\r\f\v")
{
    return ltrim(rtrim(s, t), t);
}



// trim from left
inline std::string_view& ltrim(std::string_view& s, const char* t = " \t\n\r\f\v")
{
    s = s.substr(s.find_first_not_of(t));
    return s;
}

// trim from right
inline std::string_view& rtrim(std::string_view& s, const char* t = " \t\n\r\f\v")
{
    s = s.substr(0, s.find_last_not_of(t)+1);
    return s;
}

// trim from left & right
inline std::string_view& trim(std::string_view& s, const char* t = " \t\n\r\f\v")
{
    return ltrim(rtrim(s, t), t);
}

