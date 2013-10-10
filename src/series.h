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


#ifndef __SERIES_H__
#define __SERIES_H__ 1

#include <map>
#include <string>
#include <vector>

namespace tsm {

        // T represents a date or a time.
        template<typename T>
        class Series {
        public:
                class TPtr {
                public:
                        // TODO: TPtr is a sort of a pointer to a T,
                        // but at the same time it is tightly bound to
                        // its Series so that it can know if its range is valid,
                        // as well as to know the next() actual temporal point.
                        TPtr();
                        ~TPtr();

                        T when();
                        double value();
                        // next() moves to the next defined point.
                        // operator++() moves to the next possible point, interpolating if necessary.
                        TPtr next();
                        TPtr operator++();
                        bool valid();
                };
                
        public:
                // TODO: pass default interpolator to constructor?
                Series();
                ~Series();

                std::vector<std::string> names();
                TPtr first(std::string name);
                // Note that when need not be a temporal point in the series:
                // value() interpolates between neighboring points if needed.
                double value(std::string name, T when);

        private:
                std::map<std::string, std::map<T, double> > series_;
        };

}

#endif  /* __SERIES_H__*/
