/*
    Spreadsheet Parser (spreadsheet.cpp)

    Author: Saikishore G.
    Date: 05/26/2021

    All trademarks belong to their respective owners. Lawyers love tautologies.
*/

//Includes
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

/*
    Line

    Represents the interface for a line in the spreadsheet
*/
using cell = std::string;
class Spreadsheet;
class Line
{
    friend class Spreadsheet;

    Line(cell const& raw)
    {
      std::istringstream ss{raw};
      cell c;
      while(std::getline(ss, c, ',')) cells.push_back(c);
    }

    friend std::ostream& operator<<(std::ostream& os, Line const& l)
    {
        std::copy(std::begin(l.cells), std::end(l.cells), std::ostream_iterator<cell>(os, " "));
        return os;
    }

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
    Spreadsheet()
    {
        if(!infile || !outfile) return;

        std::string raw_line{};
        while(std::getline(infile, raw_line)) {
            lines.push_back(raw_line);}
    }

    friend std::ostream& operator<<(std::ostream& os, Spreadsheet const& s)
    {
        for(auto&& l : s.lines) os << l << '\n';
        return os;
    }

private:
    std::ifstream infile{"input.csv"};
    std::ofstream outfile{"output.csv", std::fstream::out};

    std::vector<Line> lines{};
};

int main()
{
    Spreadsheet doc;
    std::cout << doc;

    return 0;
}