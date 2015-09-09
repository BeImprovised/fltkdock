//
// $Id: DockBase.h,v 1.5 2006/06/23 21:32:50 z_hossain Exp $
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

#ifndef fltk_DockBase_h
#define fltk_DockBase_h

#include <fltk/Group.h>

namespace fltk {

class DockBase : public Group {
	int tmuID_;
	int *sizes_;
		
protected:
	DockBase(int, int, int, int, const char* = 0);
	enum {
		MATCH_DOCKHOLDER   = 0x00000,
		MATCH_BARHOLDER    = 0x01000,
		MATCH_TABHOLDER    = 0x02000,
		MATCH_WINDOWHOLDER = 0x04000,
		MATCH_REGIONHOLDER = 0x08000,
	};
	static int id_count_;
	
public:
	enum {
		UNDEFINE_ID = -1,
	};
	enum {
		WHEN_DOCK = 8,
		WHEN_UNDOCK = 4,
	};
	enum {
		UNIQUEID_MASK 	= 0x000FFF,
		MATCHID_MASK 		= 0x0FF000,
	};
	~DockBase();
	
	int unique_id() const { return UNIQUEID_MASK & tmuID_; }
	void unique_id(int id) { tmuID_ = ~UNIQUEID_MASK & tmuID_ | id; }
	int match_id() const { return MATCHID_MASK & tmuID_; }
	void match_id(int id) { tmuID_ = ~MATCHID_MASK & tmuID_ | id; }
	
	void push_sizes();
	void pop_sizes();
	
	bool resize(int x, int y, int w, int h) { return Widget::resize(x, y, w, h); }
	bool resize(int w, int h) { return Widget::resize(w, h); }
	Window *parent_window();
	
#if 0 //improve polymorphism
	virtual void add(Widget *o) { Group::add(o); }
	virtual void add(Widget &o) { Group::add(o); }
	virtual void end() { Group::end(); }
	virtual void add(Widget *o) { Group::add(o); }
	virtual void add(Widget &o) { Group::add(o); }
	virtual void label(const char *a) { Group::label(a); }
	viatual const char* label() const { Group::label(); }
#endif
};

class FL_API Global {
	static uchar option_;
	static float delay_;
	static const char *app_;
	
	Global();
	
public:
	enum {
		HIGHLIGHT_TITLE = 0x01,
		SAVE_STATUS 		= 0x02,
		FILL_VERTICAL 	= 0x04,
	};
	
	static bool save_status() { return option_ & SAVE_STATUS; }
	static void save_status(bool b)
	{ option_ = b ? (option_ | SAVE_STATUS) : (option_ & ~SAVE_STATUS); }
	
	static bool highlight_title() { return option_ & HIGHLIGHT_TITLE; }
	static void highlight_title(bool b)
	{ option_ = b ? (option_ | HIGHLIGHT_TITLE) : (option_ & ~HIGHLIGHT_TITLE); }
	
	static bool fill_vertical() { return option_ & FILL_VERTICAL; }
	static void fill_vertical(bool b)
	{ option_ = b ? (option_ | FILL_VERTICAL) : (option_ & ~FILL_VERTICAL); }
	
	static float preview_delay() { return delay_; }
	static void preview_delay(float d) { delay_ = d; }
	
	static void app_name(const char *c) { app_ = c; }
	static const char* app_name() { return app_; }
};

}
#endif
//
// End of $Id: DockBase.h,v 1.5 2006/06/23 21:32:50 z_hossain Exp $
//

