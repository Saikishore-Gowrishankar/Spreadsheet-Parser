/****************************************
 * DailyQA.cpp
 *
 * Uses the spreadsheet parser to format
 * the DailyQA sheet
 *
 ****************************************/

/* Standard dependencies */
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <iostream>

/* Local Dependencies */
#include "spreadsheet.h"

/**
 * DailyQA
 *
 * @class This class parses and formats the URL spinner daily report
 * and outputs the correctly formatted data so that the click rates
 * can easily be copy/pasted into the daily QA sheet.
 */
class DailyQA
{
public:
    /**
     * DailyQA()
     *
     * @param names File name of sheet with projects/companies
     */
    DailyQA(std::string_view names, std::string_view data)
             : names_sheet{names.data(), false}, data_sheet{data.data(), true}
    {
        // Insert data into a hash map for easy lookup
        for(auto&& line : data_sheet) data_entries.insert({line[0]+line[1], line});
    }

    /**
     * run()
     *
     * @brief Runs the main program and outputs into the outfile.
     */
    void run()
    {
        std::string cached_company_name;
        for(auto&& line : names_sheet)
        {
            auto company = line[0], project = line[1];
            //if(auto ch = company.find(','); ch) company[ch] == '_';

            // The projects don't all have a company name listed next to them
            // For example, if the company has multiple carriers it only lists the
            // name next to the first project. This caches that name so that all
            // projects under the same company have a company name listed when 
            // the data is entered into the output file.
            if(company != "") cached_company_name = company;
            else if(company == "" && project != "") company = line[0] = cached_company_name;
            else if(company == "" && project == "_") { outfile << '\n'; continue; }

            // The lookup uses the concatenation of the company and the project names
            auto str = company+project;
            if(auto search = data_entries.find(str); search != std::end(data_entries) && search->first != "")
            {
                #define out(x,y) ( (x)[y] == ""? "n/a" : (x)[y]  )
                outfile << out(search->second,0) << ',' << "->" << out(search->second,1) << ','
                        << out(search->second,4) << ',' << out(search->second,6) << ','<< out(search->second,8) << ',' << out(search->second,10) << "\n";
                #undef out
            }
            else if(project == "_") { std::cout << "Skipping ws\n"; outfile << '\n';}
            else
            {
                std::cout << "Could not find:    " << company << " - " << project << "    \n";
                outfile << company << ',' << project << ',' << "ERR,ERR,ERR,ERR\n";
            }
        }
    }
private:
    Spreadsheet names_sheet, data_sheet; //For the names and data
    std::unordered_map<std::string, Line> data_entries{}; //Stores data entries
    std::ofstream outfile{"output.csv"}; //Output file
};

int main()
{
    std::cout << "\nDaily QA Sheet generator\n"
              << "opening names.csv and data.csv\n\n";

    DailyQA doc{"names.csv", "data.csv"};
    doc.run();

    std::cout << "\nFinished. Find results in output.csv. Terminating.\n\n";

    return 0;
}