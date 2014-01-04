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

#ifndef __STORE_H__
#define __STORE_H__ 1

#include <leveldb/db.h>
#include <memory>
#include <string>

namespace tsm {

    class Series;

    // Model a data store that maps T's (time or date values) to V's
    // (double or string).
    class Db {
    public:
	// Open an existing database.
	static std::unique_ptr<Db> ExistingDb(const std::string& db_path);
	// Create and open a new database.
	static std::unique_ptr<Db> NewDb(const std::string& db_path, const std::string& key_type, const std::string& value_type);

	~Db();

	// Set() and Get() will throw TypeError if called through an
	// interface that is not the correct interface for the data
	// in the db.
	//
	// TODO(JMA): Decide on time formats and conversion library.
	//
	// Can throw StorageError.
	void Set(time_t when, double value);
	void Set(time_t when, const std::string& value);
	void Set(int when, double value); // JMA JMA JMA  int as days since 1-1-1970 ?
	void Set(int when, const std::string& value);
	
	// Can throw OutofTemporalRange, StorageError.
	void Get(time_t when, double* value);
	void Get(time_t when, std::string* value);
	void Get(int when, double* value);
	void Get(int when, std::string* value);
	// JMA JMA JMA  Should I perhaps always return a Series, even it only has one point?
	// JMA JMA JMA  In that case, the second arg could default to the first.
	// JMA JMA JMA  (And Series should be an ABC for {time_t, int} x {std::string, double} Series.)
	Series Get(time_t begin, time_t end);
	Series Get(int begin, int end);

	explicit Db();	// JMA JMA JMA  fake.
    private:
	// Takes ownership of db.
	explicit Db(leveldb::DB* db);

	// Set a chunk of configuration.
	void SetConfig(const std::string& key, const std::string& value);
	void SetConfig(const std::string& key, const double value);

	// Retrieve a chunk of configuration.
	std::string GetConfigString(const std::string& key);
	double GetConfigDouble(const std::string& key);

	std::unique_ptr<leveldb::DB> db_;
    };

} /* namespace tsm */

#endif  /* __STORE_H__*/
