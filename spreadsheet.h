/*
    Spreadsheet Parser (spreadsheet.cpp)

    Author: Saikishore G.
    Date: 05/26/2021

    All trademarks belong to their respective owners. Lawyers love tautologies.
*/

#pragma once

//Includes
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <sstream>
#include <algorithm>
#include <iterator>

using cell = std::string;
class Spreadsheet;

/*
    Line

    Represents the interface for a line in the spreadsheet
*/
class Line
{
public:
    friend class Spreadsheet;
    using iterator = std::vector<cell>::iterator;
    using const_iterator = std::vector<cell>::const_iterator;

    Line(cell const& raw)
    {
        std::istringstream ss{raw};
        cell c;
        while(std::getline(ss, c, ',')) cells.push_back(c);
    }

    cell to_raw() const
    {
        std::ostringstream ss;
        std::copy(std::begin(cells), std::end(cells), std::ostream_iterator<cell>(ss, ","));
        auto val = ss.str();
        val.pop_back();
        return val;
    }

    friend std::ostream& operator<<(std::ostream& os, Line const& l)
    {
        std::copy(std::begin(l.cells), std::end(l.cells), std::ostream_iterator<cell>(os, " "));
        return os;
    }

    //Iterator methods for range-based for
    iterator       begin()  { return std::begin(cells);  }
    iterator       end()    { return std::end(cells);    }
    const_iterator cbegin() { return std::cbegin(cells); }
    const_iterator cend()   { return std::cend(cells);   }

    cell operator[] (std::ptrdiff_t off) { return cells[off]; }


private:
    std::vector<cell> cells{};
};

/*
    Spreadsheet

    Defines the interface for a spreadsheet parser
*/
class Spreadsheet
{
public:

    using iterator = std::vector<Line>::iterator;
    using const_iterator = std::vector<Line>::const_iterator;

    Spreadsheet(std::string_view in) : infile{in.data()}
    {
        if(!infile) return;

        std::string raw_line{};
        while(std::getline(infile, raw_line)) {
            lines.push_back(raw_line);}
    }

    friend std::ostream& operator<<(std::ostream& os, Spreadsheet const& s)
    {
        for(auto&& l : s.lines) os << l << '\n';
        return os;
    }

    //Iterator methods for range-based for
    iterator       begin()  { return std::begin(lines);  }
    iterator       end()    { return std::end(lines);    }
    const_iterator cbegin() { return std::cbegin(lines); }
    const_iterator cend()   { return std::cend(lines);   }

private:
    std::ifstream infile;
    std::vector<Line> lines{};
};
