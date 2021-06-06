#pragma once

#include <fstream>
#include <string_view>
#include <string>
#include <iterator>
#include <iostream>
#include <regex>
#include <algorithm>

class csv : public std::fstream
{
public:
    csv(std::string_view s) : std::fstream{s.data()}
    {
        //Regular expressions
        std::regex symbol_with_comma{ R"~("([^"]+)(,)([^"]+)")~" };
        std::regex quotes_symbol{ R"~(".")~" };

        //Read line into buffer
        std::getline(*this, buf_, (char)EOF);

        //Replace commas in symbols with underscore
        buf_ = std::regex_replace(buf_, symbol_with_comma, "$1_$3");

        //Erase all double quote characters (remove-erase idiom)
        buf_.erase(std::remove(std::begin(buf_), std::end(buf_), '"'), std::end(buf_));

        //Truncate all contents of file and write new contents.
//        close(); open(s.data(), std::fstream::out | std::fstream::trunc);
        seekp(0); seekg(0);
        *this << buf_;
        seekp(0); seekg(0);
    }
private:
    std::string buf_;
};