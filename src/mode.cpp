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
#include <map>

#include "mode.h"


using std::cerr;
using std::endl;
using std::map;


namespace tsm {
        static map<Mode, bool> modes;
}


void tsm::mode(const Mode mode, const bool new_state)
{
        modes[mode] = new_state;
}


const bool tsm::mode(const Mode mode)
{
        map<Mode, bool>::const_iterator it = modes.find(mode);
        if(it == modes.end()) {
                cerr << "Failed to find mode " << mode << endl;
                cerr << "This is a bug whose effects are undefined." << endl;
                cerr << "Continuing as though the result had been false." << endl;
                return false;
        }
        return it->second;
}


