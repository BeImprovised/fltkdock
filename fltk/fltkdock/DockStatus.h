//
// $Id: DockStatus.h,v 1.2 2006/06/24 19:57:21 z_hossain Exp $
//
// A dockable window framework for Fast Light Tool Kit (FLTK).
//
// Copyright (C) 2006 by MD. Z. Hossain
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to: 
//
//             z_hossain@users.sf.net
//

#ifndef fltk_DockStatus_h
#define fltk_DockStatus_h

#include <fltk/DockWindow.h>

#include <fltk/Preferences.h>

namespace fltk {

class DockStatus {
	DockHolder *holder_;
	Preferences *pref_;
	
public:
	int ndockwin_;
	DockWindow **dockwin_;
	
	DockStatus(DockHolder *);
	
	void add(DockWindow *);
	void read_parent(Window *);
	void write_status(DockHolder *);
	void read_status(DockWindow *);
	void delete_status();
};

}
#endif
//
// End of $Id: DockStatus.h,v 1.2 2006/06/24 19:57:21 z_hossain Exp $
//

