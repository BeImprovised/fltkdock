//
// $Id: DockWindow.h,v 1.4 2006/06/23 11:59:36 z_hossain Exp $
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


#ifndef fltk_DockWindow_h
#define fltk_DockWindow_h

#include <fltk/DockFlags.h>
#include <DockBase.h>

#include <fltk/Window.h>
#include <fltk/Menu.h>
#include <fltk/Button.h>


namespace fltk {

class DockHolder;

class ThinWindow : public Window {
	Flags flags_;
	int ext[2];
	
public:
	enum {
		RESIZER = 0x01,
		TRANSIENT = 0x02,
	};
	ThinWindow(Group *, bool = true);
	void layout();
	int handle(int);
	void draw();
	bool resizer() { return flags_ & RESIZER; }
	void resizer(bool a) { flags_ = a ? (flags_ | RESIZER) : (flags_ & ~RESIZER); }
	bool transient() { return flags_ & TRANSIENT; }
	void transient(bool a) { flags_ = a ? (flags_ | TRANSIENT) : (flags_ & ~TRANSIENT); }
};

class FL_API DockWindow : public DockBase {
	bool fix_problem_window_hide;
	struct MIndex { 
		int index_; DockWindow *dwindow_;
		MIndex(int i, DockWindow *o) { index_ = i; dwindow_ = o; }
	};
	
	DockHolder *holder_[2];
	ThinWindow *window_;
	Rectangle *dragger_;
	Menu *menu_;
	Button *docktab_;
	
	Flags user_;
	Flags able_;
	int sizes_[8];
	
	static void menu_cb(Widget*, void*);
	static void preview_dock_cb(DockWindow *);
	
protected:
	void build_menu();
	Rectangle *dragger() const { return dragger_; }
	void dragger(Rectangle *r) { dragger_ = r; }
	void dock_callback();
	void undock_callback();
	
public:	
	DockWindow(int, int, int, int, const char* = 0);
	~DockWindow();
	
	int handle(int);
	void hide();
	
	Flags dockable() const { return able_ & DOCKABLE_MASK; }
	void dockable(Flags a) { able_ = (able_ & ~DOCKABLE_MASK) | a;}	
	bool floatable() const { return able_ & FLOATABLE; }
	void floatable(bool a) { able_ = a ? (able_ | FLOATABLE) : (able_ & ~FLOATABLE); }
	bool fillable() const { return able_ & FILLABLE; }
	void fillable(bool a) { able_ = a ? (able_ | FILLABLE) : (able_ & ~FILLABLE); }
	Flags orientable() const { return able_ & ORIENTABLE_MASK; }
	void orientable(Flags a) { able_ = (able_ & ~ORIENTABLE_MASK) | a; }
	
	Flags state() const { return user_ & DOCKED_MASK; }
	virtual void state(Flags a);
	Flags orientation() const { return user_ & ORIENTED_MASK; }
	virtual void orientation(Flags a, bool = false);
		
	void sizes(int, int, int, int, bool = false);
	void sizes(int W1, int H1, bool b = false) { sizes(W1, H1, W1, H1, b); }
	int min_w() const { return orientation()? sizes_[4] : sizes_[6]; }
	int min_h() const { return orientation()? sizes_[5] : sizes_[7]; }
	void min_sizes(int, int, int, int);
	void min_sizes(int W1, int H1) { min_sizes(W1, H1, W1, H1); }
	void min_sizes(int W, int H, Flags o);
	
	void holder(DockHolder *);
	DockHolder *holder() const { return holder_[0]; }
	void dock_in(DockHolder *dh) { holder_[1] = dh; }
	DockHolder *dock_in() const { return holder_[1]; }
	void dock_tab(Button *t) { docktab_ = t; }
	Button *dock_tab() const { return docktab_; }
	
	Menu *menu() const { return menu_; }
	ThinWindow *window() { return window_; }
	void border(bool b, const char *l = 0) { window_->border(b); window_->label(l); }
	bool border() { return window_->border(); }
	void showWindow();
	
	void try_dock();
	void dock(Flags = DOCKED_XY);
	void try_undock(int, int);
	void undock();
	void preview_dock(bool = false);
	
	Flags user_flags() const { return user_; }
	
	void resizer(bool a) { window_->resizer(a); }
	bool resizer() const { return window_->resizer(); }
};

}
#endif

//
// End of $Id: DockWindow.h,v 1.4 2006/06/23 11:59:36 z_hossain Exp $.
//
