/*
  Copyright 2013  Jeff Abrahamson
  
  This file is part of tsm.
  
  tsm is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  tsm is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with tsm.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <boost/program_options.hpp>
#include <stdexcept>
#include <iostream>
#include <string>


using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::string;


namespace BPO = boost::program_options;

class help_exception : public exception {};


namespace {

        BPO::variables_map parse_options(int, char *[]);


        BPO::variables_map parse_options(int argc, char *argv[])
        {
                BPO::options_description general("General options");
                general.add_options()
                        ("help,h",
                         "Produce help message")
                        ("read-only,R",
                         "Read-only, do not persist data")
                        ("database-dir", BPO::value<string>(),
                         "Name of directory to use for database instead of default")
                        ("verbose,v",
                         "Emit debugging information");

                BPO::options_description actions("Actions (if none, add data point)");
                actions.add_options()
                        ("modify", BPO::value<double>(),
                         "Modify data point.  Except in quiet mode, it is enough "
                         "to add a data value for an existing time point.")
                        ("delete,x",
                         "Delete data point")
                        ("create", BPO::value<string>(),
                         "Create new database.")
                        ("import", BPO::value<string>(),
                         "Import a text file.  The format is the same as that output "
                         "by -f:  each line must a date or time followed by whitespace "
                         "followed by a value.")
                        ("validate,V",
                         "Confirm that all databases are loadable and consistent");

                BPO::options_description matching("Matching options");
                matching.add_options()
                        ("time,t", BPO::value<string>(),
                         "Specify the date or time to which the current operation applies.")
                        ("begin", BPO::value<string>(),
                         "Specify the beginning of a date or time range for data retrieval.")
                        ("end", BPO::value<string>(),
                         "Specify the end of a date or time range for data retrieval.");

                BPO::options_description output("Output options");
                output.add_options()
                        ("plot,p",
                         "Plot data.")
                        ("text-dump,f",
                         "Display full data, tab-separated.");

                BPO::options_description test("Test options (don't use except for regression tests)"); 
                test.add_options()
                        ("TEST", BPO::value<string>(),
                         "Test mode, use local data directory");
        
                BPO::options_description options("Allowed options");
                options.add(general).add(actions).add(matching).add(output).add(test);
        
                BPO::positional_options_description pos;
                pos.add("db-name", -1);
                pos.add("data-point", -1);
        
                BPO::variables_map opt_map;
                BPO::store(BPO::command_line_parser(argc, argv).options(options).positional(pos).run(),
                           opt_map);
                BPO::notify(opt_map);

                if(opt_map.count("help")) {
                        cout << "tsm <options> [data-point]" << endl << endl;
                        cout << "  To add a new point, specify the database and the point value.";
                        cout << endl;
                
                        cout << options << endl;
                        throw help_exception();
                }
        
                return opt_map;
        }

} // namespace


/*
  Do that thing that we do.
*/
int main(int argc, char *argv[])
{
        BPO::variables_map options;
        try {
                options = parse_options(argc, argv);
        }
        catch(help_exception) {
                return 0;       // User asked for help, done
        }
        catch(exception& e) {
                // Something went wrong.  Say so and exit with error.
                cerr << e.what() << endl;
                // Options aren't available, so be verbose to be clear.
                cerr << "(Error is fatal, quitting before doing anything.)" << endl;
                return 1;
        }
        return 0;
}
