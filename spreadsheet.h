/******************************************************************************
 *  Spreadsheet Parser (spreadsheet.cpp)
 *
 *  Author: Saikishore G.
 *  Date: 05/26/2021
 *
 *  All trademarks belong to their respective owners. Lawyers love tautologies.
 *
 *  This is a lightweight spreadsheet (CSV) parser for use with the DailyQA
 *  program.
 ******************************************************************************/

#pragma once

/* Standard dependencies */
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <iterator>

/* Local dependencies */
#include "csv.h"

using cell = std::string;
class Spreadsheet;

/******************************************************************************
 *  Line
 *
 *  @class Represents the interface for a line in the spreadsheet
 ******************************************************************************/
class Line
{
public:

    /* Implementation details*/
    friend class Spreadsheet;
    using       iterator = std::vector<cell>::      iterator;
    using const_iterator = std::vector<cell>::const_iterator;

    /**
     * Line()
     *
     * @brief Extracts cells from raw line
     * @param raw Raw line data
     */
    Line(cell const& raw)
    {
        using namespace std::string_literals;

        std::istringstream ss{raw};
        cell c;
        auto str = ss.str();
        if(str[0] == '"')
        {
            // @todo Fix output of names w/ comma
            ss.get();
            std::getline(ss, c, '"');

//            c[c.find(',')] = '_';

            cells.push_back("\""s+c+"\""s);
            ss.get();
        }
        while(std::getline(ss, c, ',')) cells.push_back(c);
    }

    /**
     * to_raw()
     *
     * @brief Extracts cells from raw line
     * @return std::string holding raw data
     */
    cell to_raw() const
    {
        std::ostringstream ss;
        std::copy(std::begin(cells), std::end(cells), std::ostream_iterator<cell>(ss, ","));
        auto val = ss.str();
        val.pop_back();
        return val;
    }

    /**
     * ::operator<<()
     *
     * @brief Output formatted line
     */
    friend std::ostream& operator<<(std::ostream& os, Line const& l)
    {
        std::copy(std::begin(l.cells), std::end(l.cells), std::ostream_iterator<cell>(os, " "));
        return os;
    }

    /**
     * @brief Iterator methods for range-based for
     */
    iterator       begin()  { return std::begin(cells);  }
    iterator       end()    { return std::end(cells);    }
    const_iterator cbegin() { return std::cbegin(cells); }
    const_iterator cend()   { return std::cend(cells);   }

    /**
     * operator[]()
     *
     * @brief Access individual cells
     */
    cell&       operator[] (std::ptrdiff_t off)         { return cells[off]; }
    cell const& operator[] (std::ptrdiff_t off) const   { return cells[off]; }


private:
    /* Parsed cells */
    std::vector<cell> cells{};
};

/******************************************************************************
 *  Spreadsheet
 *
 *  @class Defines the interface for a spreadsheet parser
 ******************************************************************************/
class Spreadsheet
{
public:

    /* Implementation details*/
    using iterator       = std::vector<Line>::      iterator;
    using const_iterator = std::vector<Line>::const_iterator;

    /**
     * Spreadsheet()
     *
     * @param in The input file name
     *
     **/
    Spreadsheet(std::string_view in) : infile{in}
    {
        if(!infile) { std::cerr << "could not open file " << in.data() << ". Abort.\n"; std::exit(1); }

        std::string raw_line{};
        infile.seekg(0); infile.seekp(0);
        while(std::getline(infile, raw_line)) lines.push_back(raw_line);
    }

    /**
     * ::operator<< ()
     *
     * @brief Outputs formatted spreadsheet
     *
     **/
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
    csv infile;			//Input file
    std::vector<Line> lines{};	//Parsed lines
};
