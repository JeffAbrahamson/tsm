/*
  Copyright 2014  Jeff Abrahamson
  
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

#include <string>

#include "map_type.h"

using std::string;

ValueKey::ValueKey(double t) : dual_type_(t)
{
}

ValueKey::ValueKey(std::string t) : dual_type_(t)
{
}

void ValueKey::SetDouble(double t)
{
    return dual_type_.SetT1(t);
}

void ValueKey::SetString(std::string t)
{
    return dual_type_.SetT2(t);
}

const double& ValueKey::GetDouble() const
{
    return dual_type_.GetT1();
}

const string& ValueKey::GetString() const
{
    return dual_type_.GetT2();
}

bool ValueKey::IsDouble()
{
    return dual_type_.Is1();
}

bool ValueKey::IsString()
{
    return dual_type_.Is2();
}
