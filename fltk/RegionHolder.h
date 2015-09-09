//
// $Id: RegionHolder.h,v 1.2 2006/06/24 20:01:29 z_hossain Exp $
//
// A dockable window framework for Fast Light Tool Kit (FLTK).
//
// Copyright (C) 2006 MD. Z. Hossain.
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

#ifndef fltk_RegionHolder_h
#define fltk_RegionHolder_h

#include <fltk/DockWindow.h>
#include <fltk/DockHolder.h>

namespace fltk {

class FL_API RegionHolder : public DockHolder {
	DockHolder **holder_;
	
	bool in_range(DockWindow &, DockHolder &);
	int find_holder(Widget *);
	
public:
	RegionHolder( int, int, int, int);
	~RegionHolder();
	
	void layout();
	void draw();
	void init_child();
	void end() { DockHolder::end(); init_child(); }
	
	void dock(DockWindow *, Flags = DOCKED_XY);
	void undock(DockWindow *);
	void try_dock(DockWindow *);
	void try_undock(DockWindow *, int, int);
	void preview_dock(DockWindow *);
	void clear_preview();
	
	DockHolder *holder(int i) { return holder_[i]; };
	int holder_id(DockWindow *);
	void initiate();
};

}
#endif

//
// End of $Id: RegionHolder.h,v 1.2 2006/06/24 20:01:29 z_hossain Exp $.
//
