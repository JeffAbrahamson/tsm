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
    template<typename K, typename V>
    class Series {
    public:
	// TODO: pass default interpolator to constructor?
	// TODO: Somehow provide the series with its data!
	//       We shouldn't have to pull everything from the
	//       store, but rather the store should back the series.
	Series();
	~Series();

	// Return true if the series is empty (has no points).
	bool Empty() const;
                
	// Return the first time point in the series.
	K First() const;

	// Return the last time point in the series.
	K Last() const;

	// Return the least time point greater than the
	// provided time point "when".
	K Next() const;

	// Note that "when" need not be a time point in the
	// series: value() interpolates between neighboring
	// points if needed.
	V Value(K when) const;

    private:
	// TODO: This is wrong.  Some series are backed
	// directly by the store.  Other series are computed,
	// perhaps on the fly.  Some series might cache their
	// data as they go to avoid recomputation.  So
	// probably I should replace this with an ABC,
	// providing StoredSeries, CachedSeries, and
	// ComputedSeries.
	std::map<K, double> series_;

	// TODO(JMA): I have {Stored,Cached,Computed}Series.  And then
	// it is also parameterized by key type and value type.  Which
	// is all a bit much to do with inheritance.
	//
	// Maybe there's a role for templating to handle key and value
	// type, though any class that returns a Series probably wants
	// to be declared to return the base class unless it knows
	// ahead of time which it's returning.
	//
	// The Series might have a member function that knows how to
	// fetch data -- from another Series, from a DB, but first
	// maybe from a cache.  Interpolation becomes a Series type
	// that checks cache, then its underlying Series, then
	// calculates based on what it found.  These things might be
	// Sources (a DB, a Cache, another Series).  And a Series has
	// a queue of Sources.
    };

}  // namespace tsm

#endif  /* __SERIES_H__*/
