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



#ifndef __TSM__MODE_H__
#define __TSM__MODE_H__ 1

namespace tsm {
        
    /*
      Maintain a map of modes.

      Written for verbose and testing, to avoid having to pass them around
      everywhere in case we need them deep down.  Especially verbose.
    */


    enum Mode {
	Invalid,        /* Invalid value */
                
	Verbose,        /* Provide information that the user
			   might consider informative. */
	Quiet,          /* Provide no output that is not
			   explicitly requested.  Some events
			   that might otherwise trigger
			   warnings may become errors. */
	Testing,        /* The code is being run as part of a
			   test. */
	Debug,          /* Emit information that might be
			   useful when debugging. */
	ReadOnly,       /* Persist nothing.  In particular,
			   don't even try to persist anything,
			   since either the user has indicated
			   we shouldn't or we have concluded
			   that it may not work. */
                
	ModeError,          /* last entry */
    };

    void mode(const Mode m, const bool new_state);
    const bool mode(const Mode m);

}      /* namespace tsm */

#endif  /* __TSM__MODE_H__*/
