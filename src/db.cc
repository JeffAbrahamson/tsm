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

#include <cassert>
#include <leveldb/db.h>
#include <memory>
#include <string>

#include "db.h"

using std::string;
using std::unique_ptr;

namespace tsm {

    unique_ptr<Db> Db::ExistingDb(const std::string& db_path)
    {
	leveldb::DB* db;
	leveldb::Options options;
	options.create_if_missing = false;
	leveldb::Status status = leveldb::DB::Open(options, db_path, &db);
	assert(status.ok());
	unique_ptr<Db> this_store(new Db(db));
	//this_store->SetConfig("time-type", "time"); // JMA JMA JMA  
	//this_store->SetConfig("vaue-type", "double"); // JMA JMA JMA  
	return this_store;
    }

    unique_ptr<Db> Db::NewDb(const std::string& db_path, const string& key_type, const string& value_type)
    {
	leveldb::DB* db;
	leveldb::Options options;
	options.create_if_missing = true;
	leveldb::Status status = leveldb::DB::Open(options, db_path, &db);
	assert(status.ok());
	unique_ptr<Db> this_store(new Db(db));
	//this_store->SetConfig("time-type", "time"); // JMA JMA JMA  
	//this_store->SetConfig("vaue-type", "double"); // JMA JMA JMA  
	return this_store;
    }
    

    Db::Db(leveldb::DB* db) : db_(db)
    {}

    Db::~Db()
    {}

} // namespace tsm
