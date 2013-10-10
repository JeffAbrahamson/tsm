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


#include <memory>


#ifndef __STORE_H__
#define __STORE_H__ 1

namespace tsm {

        class Database;
        class Series;

        class Store {
        public:
                // Takes ownership of the database pointer.
                Store(Database *db);
                ~Store();

                // Can throw StorageError;
                template<typename T> void set(T when, double value);
                // Can throw OutofTemporalRange, StorageError.
                template<typename T> double get(T when);
                template<typename T> Series get(T begin, T end);

        private:
                std::unique_ptr<Database> db_;
        };

} /* namespace tsm */

#endif  /* __STORE_H__*/
