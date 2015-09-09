//
// $Id: DockHolder.h,v 1.4 2006/06/23 21:31:11 z_hossain Exp $
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

#ifndef fltk_DockHolder_h
#define fltk_DockHolder_h

#include <fltk/DockWindow.h>
#include <fltk/DockFlags.h>
#include <DockStatus.h>
#include <DockBase.h>

namespace fltk {

class Preview : public Rectangle {
	uchar flags_;
	Rectangle *redraw_;
	
public:
	enum {
		X_BEFORE = 0x01,
		X_AFTER = 0x02,
		Y_BEFORE = 0x04,
		Y_AFTER = 0x08,
		XY_MASK = 0x0F,
		VISIBLE = 0x10,
	};
	Preview(int x, int y, int w, int h);
	~Preview();
	
	void draw(Widget &);
	Rectangle &translate(Widget &, Rectangle &);
	Rectangle &redraw(Widget &to) { translate(to, *redraw_).inset(-10); return *redraw_; }
	void set(int x, int y, int w, int h) { Rectangle::set(x, y, w, h); }
	void set(Rectangle &r) { set(r.x(), r.y(), r.w(), r.h()); }
	Flags set_flag(int c) { return flags_ |= c; }
	Flags clear_flag(int c) { return flags_ &= ~c; }
	bool flag(int c) { return flags_ & c; }
	void move_preview();
};

class FL_API DockHolder : public DockBase {
	int *range_;
	Flags flags_;
	Preview *preview_;
	DockStatus *status_;
	DockWindow *recent_;
	
	void _DockHolder();
	
protected:
	DockHolder *parent_holder();
	bool in_range(DockWindow *);
	void notify_dock(DockWindow *dw);
	void notify_undock(DockWindow *dw);
	
public:
	enum {
		RANGE_TOP = 0,
		RANGE_BOTTOM = 1,
		RANGE_LEFT = 2,
		RANGE_RIGHT = 3,
	};
	enum {
#if 0
		HALIGN_TOP = 0x01,
		HALIGN_BOTTOM = 0x02,
		HALIGN_LEFT = 0x04,
		HALIGN_RIGHT = 0x08,
		HALIGN_CENTER = 0x10,
		HALIGN_MASK = 0x1F,
#endif
		DESCENDANT = 0x20,
	};
	
	DockHolder(int, int, int, int, Flags = HALIGN_UNKNOWN);
	DockHolder(int, int, Flags);
	~DockHolder();
	void layout();
	void draw();
	
	void range(int *r) { range_ = r; }
	int *range() { return range_; }
	
	virtual void dock(DockWindow *, Flags = DOCKED_XY);
	virtual void undock(DockWindow *);
	virtual void try_dock(DockWindow *);
	virtual void try_undock(DockWindow *, int, int);
	virtual void preview_dock(DockWindow * = 0);
	
	void align_holder(Flags a) { flags_ = (flags_ & (~HALIGN_MASK)) | a; }
	Flags align_holder() const { return flags_ & HALIGN_MASK; }
	void descendant(bool a) { flags_ = a ? (flags_ | DESCENDANT) : (flags_ & ~DESCENDANT); }
	bool descendant() const { return flags_ & DESCENDANT; }
	uchar orientation() const { return align_holder() & (HALIGN_LEFT | HALIGN_RIGHT) ? ORIENTED_VERTICAL : ORIENTED_HORIZONTAL; }
	Preview *preview() const { return preview_; }
	DockHolder *child_holder(int) const;
	DockStatus *status() const { return status_; }
	void write_status() { if(Global::save_status()) status_->write_status(this); }
	void read_status() { throw "Not Implemented"; }
	void delete_status() { status_->delete_status(); }
	DockWindow *recent() const { return recent_; }
};

}
#endif

//
// End of $Id: DockHolder.h,v 1.4 2006/06/23 21:31:11 z_hossain Exp $.
//
