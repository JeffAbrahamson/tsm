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


#include <iostream>

#include "mode.h"


using std::cout;
using std::endl;

using tsm::Mode;
using tsm::mode;


int check_mode(Mode m, bool b);


/*
  Mode is awfully simple...
*/
int main(int argc, char *argv[])
{
        cout << "Mode::Testing mode.cpp" << endl;

        int error_count = 0;

        mode(Mode::Verbose, false);
        error_count += check_mode(Mode::Verbose, false);
        mode(Mode::Verbose, true);
        error_count += check_mode(Mode::Verbose, true);
        mode(Mode::Verbose, false);
        error_count += check_mode(Mode::Verbose, false);

        mode(Mode::Testing, false);
        error_count += check_mode(Mode::Testing, false);
        mode(Mode::Testing, true);
        error_count += check_mode(Mode::Testing, true);
        mode(Mode::Testing, false);
        error_count += check_mode(Mode::Testing, false);
        mode(Mode::Testing, true);
        mode(Mode::Testing, true);
        mode(Mode::Testing, true);
        error_count += check_mode(Mode::Testing, true);
                
        error_count += check_mode(Mode::Verbose, false);
        mode(Mode::Verbose, false);
        error_count += check_mode(Mode::Verbose, false);
        mode(Mode::Verbose, true);
        error_count += check_mode(Mode::Verbose, true);
        mode(Mode::Verbose, false);
        error_count += check_mode(Mode::Verbose, false);
}



int check_mode(Mode the_mode, bool the_value)
{
        if(mode(the_mode) == the_value)
                return 0;
        return 1;
}
