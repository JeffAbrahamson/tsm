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

#include <algorithm>
#include <boost/program_options.hpp>
#include <chrono>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>

#include "db.h"
#include "mode.h"

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::move;
using std::string;
using std::unique_ptr;
using std::vector;
using tsm::Mode;

namespace BPO = boost::program_options;

namespace {
    class help_exception : public exception {};

    // Set mode flags.
    // This is the part of responding to the commandline that
    // is global, i.e., does not need to be done in main().
    void set_flags(BPO::variables_map &options)
    {
	mode(Mode::Verbose, options.count("verbose") > 0);
	mode(Mode::Quiet, options.count("quiet") > 0);
	mode(Mode::Testing, options.count("TEST") > 0);
	mode(Mode::Debug, options.count("debug") > 0);
	mode(Mode::ReadOnly, options.count("read-only") > 0);
    }

    // Parse the commandline.
    BPO::variables_map parse_options(int argc, char *argv[])
    {
	BPO::options_description general("General");
	general.add_options()
	    ("help,h",
	     "Produce help message")
	    ("read-only,R",
	     "Read-only, do not persist data")
	    ("basedir", BPO::value<string>(),
	     "Name of base directory in which to find databases")
	    ("dbname", BPO::value<string>(),
	     "Time series name.  May also be specified as first positional argument.  "
	     "Expect to find in --basedir unless this is a relative or absolute pathname.")
	    ("create", BPO::value<string>(),
	     "Create new database: arg is the key type (\"daily\" or \"continuous\"), "
	     "a comma, and the value type (\"double\" or \"string\").")
	    ("verbose,v",
	     "Emit extra information")
	    ("debug",
	     "Emit debugging information")
	    ("quiet,q",
	     "Emit no information apart status code.  Some warnings become errors.");

	BPO::options_description actions("Actions (at most one; if none and no output actions, \"add\")");
	actions.add_options()
	    ("modify", BPO::value<double>(),
	     "Modify an existing data point with value arg.  Uses the current date or time"
	     "unless specified with --time.  Except in quiet mode, "
	     "it is enough to add a data value for an existing time point.")
	    ("add", BPO::value<double>(),
	     "Add a new data point with value arg.  Uses the current date or time"
	     "unless specified with --time.  Except in quiet mode, "
	     "it is enough to add a data value for an existing time point.")
	    ("delete,x", BPO::value<string>(),
	     "Delete data point at date or time arg")
	    ("import", BPO::value<string>(),
	     "Import a text file.  The format is the same as that output "
	     "by --text-dump:  each line must be a date or time followed by"
	     "whitespace followed by a value.")
	    ("validate,V",
	     "Confirm that all databases are loadable and consistent");

	BPO::options_description matching("Matching (for output actions)");
	matching.add_options()
	    ("time,t", BPO::value<string>(),
	     "Specify the date or time to which the current operation applies.")
	    ("begin", BPO::value<string>(),
	     "Specify the beginning of a date or time range for data retrieval.")
	    ("end", BPO::value<string>(),
	     "Specify the end of a date or time range for data retrieval.")
	    ("value", BPO::value<double>(),
	     "Value of new or modified point.  May also be specified as second positional argument.");

	BPO::options_description output("Output actions (may be specified with other actions)");
	output.add_options()
	    ("plot,p",
	     "Plot data.")
	    ("text-dump",
	     "Output time series as text, tab-separated.")
	    ("list-series",
	     "Output the time series we know about.");

	BPO::options_description test("Test options (don't use except for regression tests)"); 
	test.add_options()
	    ("TEST",
	     "Test mode, use local data directory.  Ignore --basedir.");
        
	BPO::options_description options("Allowed options");
	options.add(general).add(actions).add(matching).add(output).add(test);
        
	BPO::positional_options_description pos;
	pos.add("dbname", 1);
	pos.add("value", 2);
        
	BPO::variables_map opt_map;
	BPO::store(BPO::command_line_parser(argc, argv).options(options).positional(pos).run(),
		   opt_map);
	BPO::notify(opt_map);

	if(opt_map.count("help")) {
	    cout << "tsm <options> [time-series-name] [data-point]" << endl << endl;
	    cout << "  To add a new point, specify the database and the point value." << endl;
	    cout << "  Mostly, this doesn't work yet.";
	    cout << endl << endl;
	    cout << options << endl;
	    throw help_exception();
	}

	set_flags(opt_map);
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
    catch(help_exception& e) {
	return 0;
    }
    catch(exception& e) {
	// Something went wrong.  Say so and exit with error.
	cerr << e.what() << endl;
	// Options aren't available, so be verbose to be clear.
	cerr << "(Error is fatal, quitting before doing anything.)" << endl;
	return 1;
    }

    // Scaffolding for what comes next
    if(options.count("help") > 0)
	return 0;
    if(options.count("time")) {
	cout << "set time, value=" << options["time"].as<string>() << endl;
	auto seconds_since_epoch =
	    std::chrono::duration_cast<std::chrono::seconds>(
		std::chrono::system_clock::now().time_since_epoch());
	cout << "seconds=" << seconds_since_epoch.count() << endl;
	auto hours_since_epoch =
	    std::chrono::duration_cast<std::chrono::hours>(
		std::chrono::system_clock::now().time_since_epoch());
	cout << "days=" << hours_since_epoch.count() / 24 << endl;
    }
    if(options.count("begin"))
	cout << "set begin time, value=" << options["begin"].as<string>() << endl;
    if(options.count("end"))
	cout << "set end time, value=" << options["end"].as<string>() << endl;
    if(options.count("dbname"))
	cout << "dbname, value=" << options["dbname"].as<string>() << endl;
    if(options.count("value"))
	cout << "value, value=" << options["value"].as<double>() << endl;

    // Open a db, creating if appropriate.
    unique_ptr<tsm::Db> db;
    if(options.count("create")) {
	string create_spec(options["create"].as<string>());
	size_t pos = create_spec.find(",");
	assert(string::npos != pos);
	string key_type(create_spec.substr(0, pos));
	if(key_type != "daily" && key_type != "continuous") {
	    cerr << "Invalid key_type: " << key_type << endl;
	    exit(1);
	}
	string value_type(create_spec.substr(pos + 1));
	if(value_type != "double" && value_type != "string") {
	    cerr << "Invalid value_type: " << key_type << endl;
	    exit(1);
	}
	cout << "create: " << key_type << " -- " << value_type << endl;
	db = std::move(tsm::Db::NewDb(options["dbpath"].as<string>(), key_type, value_type));
	// Create db.
	// Else error.
    } else {
	// Open existing db.
	db = move(tsm::Db::ExistingDb(options["dbpath"].as<string>()));
    }
    
    // Actions we can take.
    if(options.count("modify")) {
	cout << "modify, value=" << options["modify"].as<double>() << endl;
	// Open db.
	// Check if (time, value) exists.
	// If so, modify it.  If not quiet, emit message about change.
	// If not and not quiet, add it and emit message.
	// If not and quiet, don't modify but emit error.
	return 0;
    }
    if(options.count("add")) {
	cout << "add, value=" << options["add"].as<double>() << endl;
	// Open db.
	// Check if (time, value) exists.
	// If not, add it.
	// If so and not quiet, add it and emit message about change.
	// If so and quiet, don't add but emit error.
	return 0;
    }
    if(options.count("delete")) {
	cout << "delete, date/time=" << options["delete"].as<string>() << endl;
	// Open db.
	// Check if time point exists.
	// If yes, delete it.  If not quiet, note old value.
	// If not, error.
	return 0;
    }
    if(options.count("import")) {
	cout << "import, value=" << options["import"].as<string>() << endl;
	// Read and parse file.
	// If format is bad, report any errors and exit.
	// Open db.
	// Add data.
	return 0;
    }
    if(options.count("validate")) {
	cout << "validate" << endl;
	// If a db is specified, use it.  Otherwise, discover all db's.
	// For each db, print name and validate.
	return 0;
    }
    if(options.count("plot")) {
	cout << "plot" << endl;
	// Open db.
	// Plot the data.
	// Note that somehow I need to be able to specify plot command (DSL) or derived series.
	// This command may be merely for plotting default series from one db.
	return 0;
    }
    if(options.count("text-dump")) {
	cout << "text-dump" << endl;
	// Open db.
	// Print the data to stdout.
	return 0;
    }
    if(options.count("list-series")) {
	cout << "list-series" << endl;
	// Discover all db's.
	// Print their names to stdout.
	return 0;
    }

    // No other command, so this is data input.
    // Open db.
    // Get input filter and validator.
    // If validation passes, add point to database.
        
    return 0;
}

