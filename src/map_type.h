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

#ifndef __MAP_TYPE_H__
#define __MAP_TYPE_H__ 1

#include <stdexcept>

class BadType : public std::exception {};

// Class to represent two types with affinity.
template<typename T1, typename T2>
class DualType {
public:
    DualType(T1 t1) : t1_(t1), is_1_(true) {}
    DualType(T2 t2) : t2_(t2), is_1_(false) {};
    ~DualType() {};

    void SetT1(T1 t1) {
	if(is_1_)
	    t1_ = t1;
	else
	    throw BadType();
    }

    void SetT2(T2 t2) {
	if(!is_1_)
	    t2_ = t2;
	else
	    throw BadType();
    }

    const T1& GetT1() const {
	if(is_1_)
	    return t1_;
	throw BadType();
    }

    const T2& GetT2() const {
	if(!is_1_)
	    return t2_;
	throw BadType();
    }

    bool Is1() const { return is_1_; }
    bool Is2() const { return !is_1_; }

private:
    T1 t1_;
    T2 t2_;
    const bool is_1_;
};

class ValueKey {
    ValueKey(double t);
    ValueKey(std::string t);

    void SetDouble(double t);
    void SetString(std::string t);

    const double& GetDouble() const;
    const std::string& GetString() const;

    bool IsDouble();
    bool IsString();

private:
    DualType<double, std::string> dual_type_;
};

#endif  /* __MAP_TYPE_H__*/
