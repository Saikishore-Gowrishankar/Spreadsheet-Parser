#include "spreadsheet.h"
int main()
{
    Spreadsheet doc{"input.csv"};

    for(auto&& a : doc) std::cout << a << '\n';
    return 0;
}