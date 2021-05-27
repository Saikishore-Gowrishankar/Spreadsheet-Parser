#include "spreadsheet.h"

#include <string_view>

class DailyQA
{
public:
    DailyQA(std::string_view filename) : sheet{filename.data()}
    {
        for(auto&& line : sheet) data_entries.insert({line[1], line});
    }
private:
    Spreadsheet sheet;
    std::unordered_map<std::string,Line> data_entries;
};

int main()
{
    Spreadsheet doc{"input.csv"};

    for(auto&& a : doc) std::cout << a << '\n';
    return 0;
}