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

#ifndef __SERIES_COLLECTION_H__
#define __SERIES_COLLECTION_H__ 1

#include <map>
#include <string>
#include <vector>

#include "series.h"

namespace tsm {

    // T represents a date or a time.
    template<typename T> class SeriesCollectionIterator;

    // T represents a date or a time.
    template<typename T>
        class SeriesCollection {
    public:
	SeriesCollection();
	~SeriesCollection();

	// Return the names of the series in this collection.
	// Name order is not guaranteed.
	std::vector<std::string> names();

	// Provide an iterator into the series in the collection.
	SeriesCollectionIterator<T> first(std::string &name);

	// Return a map of series name to value at the time
	// point when.  "When" need not be a time point in the
	// series: the component series will interpolate as
	// needed, each according to its own interpolation
	// policy.
	std::map<std::string, double> value(T when);

    private:
	// A map from series name to series.
	std::map<std::string, Series<T>> collection_;
                
    };


    // A SeriesCollectionIterator provides forward iteration over a SeriesCollection.
    // Notably, it provides for advancing by one time point (seconds or days),
    // by a fixed number of time points, or to the next point that
    // exists in one of the component series.
    template<typename T>
        class SeriesCollectionIterator
    {
    public:
	SeriesCollectionIterator();
	~SeriesCollectionIterator();

	// The time point here.
	T when() const;

	// The value of the series name at this time point.
	double value(const std::string &name) const;

	// Advance to the next time point that is defined in
	// one of the component series.  Return true if such a
	// point exists, false if we are at or beyond the end
	// of each component series.
	bool next();

	// Advance one or multiple time points, interpolating
	// as needed.  No component series is required to have
	// a time point at the resulting location.  Return
	// true if we are within the domain of at least one
	// component series.
	bool incr(int num = 1);

	// Return true if the iterator is valid (within the
	// time bounds of at least one component series).
	bool valid();
    };

};    // namespace tsm
        
#endif  /* __SERIES_COLLECTION_H__*/
