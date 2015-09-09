//
// $Id: DockFlags.h,v 1.3 2006/06/23 11:58:28 z_hossain Exp $
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

#ifndef fltk_DockFlags_h
#define fltk_DockFlags_h

namespace fltk {

enum {
	HALIGN_UNKNOWN = 0x0,
	HALIGN_TOP = 0x01,
	HALIGN_BOTTOM = 0x02,
	HALIGN_LEFT = 0x04,
	HALIGN_RIGHT = 0x08,
	HALIGN_CENTER = 0x10,
	HALIGN_MASK = 0x1F,
};

enum {
	DOCKABLE_TOP = HALIGN_TOP,
	DOCKABLE_BOTTOM = HALIGN_BOTTOM,
	DOCKABLE_LEFT = HALIGN_LEFT,
	DOCKABLE_RIGHT = HALIGN_RIGHT,
	DOCKABLE_CENTER = HALIGN_CENTER,
	DOCKABLE_XY = 0x20,
	DOCKABLE_ALL =		DOCKABLE_TOP 
									| DOCKABLE_BOTTOM 
									| DOCKABLE_LEFT 
									| DOCKABLE_RIGHT 
									| DOCKABLE_CENTER 
									| DOCKABLE_XY,
	DOCKABLE_NONE = 0x0,
	DOCKABLE_MASK = DOCKABLE_ALL,
	
	FLOATABLE = 0x40,
	FILLABLE = 0x80,
	
	ORIENTABLE_VERTICAL = 0x100,
	ORIENTABLE_HORIZONTAL = 0x200,
	ORIENTABLE_BOTH =		ORIENTABLE_VERTICAL 
										| ORIENTABLE_HORIZONTAL,
	ORIENTABLE_NONE = 0x0,
	ORIENTABLE_MASK = ORIENTABLE_BOTH,
};

enum {
	DOCKED_TOP = DOCKABLE_TOP,
	DOCKED_BOTTOM = DOCKABLE_BOTTOM,
	DOCKED_LEFT = DOCKABLE_LEFT,
	DOCKED_RIGHT = DOCKABLE_RIGHT,
	DOCKED_CENTER = DOCKABLE_CENTER,
	DOCKED_XY = DOCKABLE_XY,
	
	DOCKED_NONE = 0x3E, //greater than other
	DOCKED_MASK = 0x3F,

	ORIENTED_VERTICAL = 0x00,
	ORIENTED_HORIZONTAL = 0x40,
	ORIENTED_MASK = 0x40,
};

enum {
	DOCK_BASE				= 0x01,
	DOCK_HOLDER 		= 0x02,
	BAR_HOLDER 			= 0x03,
	TAB_HOLDER			= 0x04,
	WINDOW_HOLDER 	= 0x05,
	REGION_HOLDER 	= 0x06,
	
	DOCK_WINDOW 		= 0x07,
	TOOL_WINDOW 		= 0x08,
	GRIPPER_BAR 		= 0x09,
	GRIP_MENUBAR		= 0x0A,
	TOOLBAR 				= 0x0B,
};

}

#endif

//
// End of $Id: DockFlags.h,v 1.3 2006/06/23 11:58:28 z_hossain Exp $
//

