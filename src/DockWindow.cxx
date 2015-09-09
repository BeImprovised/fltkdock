//
// $Id: DockWindow.cxx,v 1.2 2006/06/23 12:00:12 z_hossain Exp $
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

#include <fltk/DockFlags.h>
#include <fltk/DockWindow.h>
#include <fltk/RegionHolder.h>
#include <Utility.h>

#include <fltk/Item.h>
#include <fltk/Divider.h>
#include <fltk/layout.h>
#include <fltk/events.h>
#include <fltk/damage.h>
#include <fltk/run.h>
#include <string.h>
#include <math.h>

#define VERTICAL_MENUINDEX 10
#define HORIZONTAL_MENUINDEX 11
#define FLOAT_MENUINDEX 12
#define HIDE_MENUINDEX 13

#define DEFAULT_MIN 10

using namespace fltk;

/*! \class fltk::DockWindow

	The base class for various dockable window and bar window. 
	fltk::Window is not a parent class of fltk::DockWindow.
	Callbacks are done when a it is docked in or undocked from holder. 
	The values for when() to control the behavior of callback are:
	\li \b fltk::WHEN_DOCK: Callback is done after it is docked.
	\li \b fltk::WHEN_UNDOCK: Callback is done after it is undocked.
	
	Default value is (fltk::WHEN_DOCK | fltk::WHEN_UNDOCK)
*/

/*! \fn void DockWindow::resizer(bool a)
	Show a resize corner to the window if it is undocked.
*/

/*! This constructor takes values for x(), y(), w(), h() and label().
	When it is construct as a child of a holder, \a X, \a Y, \a W, \a H is 
	used as position ans size hint for layout.
*/
DockWindow::DockWindow(int X, int Y, int W, int H, const char *l) 
	: DockBase(X, Y, W, H, l) {

	holder_[0] = holder_[1] = 0;
	sizes_[0] = sizes_[1] = sizes_[2] = sizes_[3] = -1;
	sizes_[4] = sizes_[5] = sizes_[6] = sizes_[7] = DEFAULT_MIN;
	able_ = ~DOCKABLE_ALL | ~FLOATABLE | ~FILLABLE | ~ORIENTABLE_BOTH;
	user_ = DOCKED_NONE | ORIENTED_HORIZONTAL;
	docktab_ = 0;
	match_id(MATCH_DOCKHOLDER);
	type(DOCK_WINDOW);
	Group *cur = Group::current();
	Group::current(0);
	window_ = new ThinWindow(this);
	Group::current(cur);
	fix_problem_window_hide = false;
}

DockWindow::~DockWindow() {
	delete window_;
	for(int i = 0; i < menu_->children(); i++) {
		delete static_cast<MIndex *>(menu_->child(i)->user_data());
	}
}

void DockWindow::build_menu() {
	int lst = 0;
	Group *cur = Group::current();
	Group::current(0);
	menu_ = new Menu(0, 0, 0, 0, 0);
	menu_->begin();
	Item* o;
	if(dockable() & DOCKABLE_TOP) {
		o = new Item("Dock &Top");
		o->callback(menu_cb, new MIndex(DOCKED_TOP, this));
	}
	if(dockable() & DOCKABLE_BOTTOM) {
		o = new Item("Dock &Bottom");
		o->callback(menu_cb, new MIndex(DOCKED_BOTTOM, this));
	}
	if(dockable() & DOCKABLE_LEFT) {
		o = new Item("Dock &Left");
		o->callback(menu_cb, new MIndex(DOCKED_LEFT, this));
	}
	if(dockable() & DOCKABLE_RIGHT) {
		o = new Item("Dock &Right");
		o->callback(menu_cb, new MIndex(DOCKED_RIGHT, this));
	}
	if(lst < menu_->children()) { new Divider(); lst = menu_->children(); }
	
	if(orientable() & ORIENTABLE_VERTICAL) {
		o = new Item("&Vertical");
		o->callback(menu_cb, new MIndex(VERTICAL_MENUINDEX, this));
	}
	if(orientable() & ORIENTABLE_HORIZONTAL) {
		o = new Item("&Horizontal");
		o->callback(menu_cb, new MIndex(HORIZONTAL_MENUINDEX, this));
	}
	if(lst < menu_->children()) { new Divider(); lst = menu_->children(); }
	
	if(floatable()) {
		o = new Item("&Float");
		o->callback(menu_cb, new MIndex(FLOAT_MENUINDEX, this));
	}
	if(lst < menu_->children()) { new Divider(); lst = menu_->children(); }
	
	o = new Item("&Hide");
	o->callback(menu_cb, new MIndex(HIDE_MENUINDEX, this));
	menu_->end();
	Group::current(cur);
}

void DockWindow::menu_cb(Widget* o, void* p) {
	MIndex *mi = (MIndex*)p;
	DockWindow *dw = dynamic_cast<DockWindow*>(mi->dwindow_);
	
	switch(mi->index_) {
		case DOCKED_TOP:
		case DOCKED_BOTTOM:
		case DOCKED_LEFT:
		case DOCKED_RIGHT:
			dw->dock(mi->index_);
			break;
		case VERTICAL_MENUINDEX:
			dw->orientation(ORIENTED_VERTICAL);
			break;
		case HORIZONTAL_MENUINDEX:
			dw->orientation(ORIENTED_HORIZONTAL);
			break;
		case FLOAT_MENUINDEX:
			if(dw->state() != DOCKED_NONE) {
				dw->undock();
			}
			break;
		case HIDE_MENUINDEX:
			dw->hide();
	}
}

void DockWindow::dock_callback() {
	if(state() < DOCKED_NONE) {
		if(when() & WHEN_DOCK) do_callback();
		else set_changed();
	}
}

void DockWindow::undock_callback() {
	if(state() == DOCKED_NONE) {
		if(when() & WHEN_UNDOCK) do_callback();
		else set_changed();
	}
}

/*! \fn Flags DockWindow::state() const
	Returns the dock or undock state value.
	\sa DockWindow::state(Flags)
*/

/*! Sets the dock or undock state value.
	The value can be any of these following constants \n
	For docked with alignment
	\li DOCKED_TOP
	\li	DOCKED_BOTTOM
	\li DOCKED_LEFT
	\li	DOCKED_RIGHT
	\li	DOCKED_CENTER

	For docked without alignment
	\li DOCKED_XY

	For not docked or undocked 
	\li DOCKED_NONE
*/
void DockWindow::state(Flags a) {
	void *data;
	user_ = (user_ & (~DOCKED_MASK)) | a;
	for(int i = 0; i < menu_->children(); i++) {
		data = menu_->child(i)->user_data();
		if(!data) continue;
		switch((static_cast<MIndex *>(data))->index_) {
			case DOCKED_TOP:
			case DOCKED_BOTTOM:
			case DOCKED_LEFT:
			case DOCKED_RIGHT:
				if(a != DOCKED_NONE) menu_->child(i)->deactivate();
				else menu_->child(i)->activate();
		}
	}
}

int DockWindow::handle(int event) {
	static int oldx = MAX_INT, oldy = MAX_INT;
		
	switch(event) {
		case PUSH:
			if(event_inside(*dragger_)) {
				if(event_button() == LeftButton) {
					if(state() == DOCKED_NONE) {
						oldx = event_x_root() - window_->x();
						oldy = event_y_root() - window_->y();
					}
					else {
						oldx = event_x_root();
						oldy = event_y_root();
					}
					return 1;
				}
				else if(event_button() == RightButton) {
					menu()->popup(Rectangle(dragger_->x(), dragger_->y(), 10, dragger_->h()), 0);
					return 1;
				}
			}
			break;
		case RELEASE:
			oldx = oldy = MAX_INT;
			try_undock(MAX_INT, MAX_INT);
			preview_dock(true);
			if(!event_state(CTRL)) {
				if(state() == DOCKED_NONE) try_dock();
				dock_callback();
			}
			break;
		case DRAG:
			if(oldx != MAX_INT && oldy != MAX_INT) {
				fltk::remove_timeout((TimeoutHandler)preview_dock_cb, this);
				if(event_state(CTRL)) preview_dock(true);
				else fltk::add_timeout(Global::preview_delay(), (TimeoutHandler)preview_dock_cb, this);
				
				if(state() < DOCKED_NONE) {
					try_undock(oldx = event_x_root() - oldx, oldy = event_y_root() - oldy);
					if(state() == DOCKED_NONE) {
						oldx = event_x_root() - window_->x();
						oldy = event_y_root() - window_->y();
						undock_callback();
					}
					else {
						oldx = event_x_root();
						oldy = event_y_root();
					}
				}
				else window_->position(event_x_root() - oldx, event_y_root() - oldy);
				return 1;
			}
			break;
	}
	return Group::handle(event);
}

void DockWindow::showWindow() {
	if(orientation() && sizes_[0] != -1) resize(sizes_[0], sizes_[1]);
	else if(sizes_[2] != -1) resize(sizes_[2], sizes_[3]);
	
	Rectangle ar;
	get_absolute_rect(&ar);
	window_->resize(ar.x(), ar.y(), ar.w(), ar.h());
	window_->add(this);
	
	if(type() == TOOL_WINDOW) window_->hotspot(window_->w()/2, dragger_->center_y());
	else window_->hotspot(dragger_->center_x(), dragger_->center_y());
	
	if(!fix_problem_window_hide) window_->show();
}

void DockWindow::orientation(Flags a, bool init) {
	if(init) { user_ = (user_ & (~ORIENTED_MASK)) | a; return;}
	if(!orientable() || orientation() == a) return;
	
	if(orientation()) {
		if(sizes_[0] == -1) { sizes_[0] = w(); sizes_[1] = h(); }
	} else {
		if(sizes_[2] == -1) { sizes_[2] = w(); sizes_[3] = h(); }
	}
	if(a) {
		if(sizes_[0] != -1) resize(sizes_[0], sizes_[1]);
		else resize(h(), w());
	}
	else {
		if(sizes_[2] != -1) resize(sizes_[2], sizes_[3]);
		else resize(h(), w());
	}
	window_->resize(w(), h());
	user_ = (user_ & (~ORIENTED_MASK)) | a;
}

inline void DockWindow::try_dock() {
	holder_[0]->try_dock(this);
}

inline void DockWindow::dock(Flags f) {
	holder_[0]->dock(this, f);
}

inline void DockWindow::try_undock(int dx, int dy) {
	if(dock_in()) holder_[0]->try_undock(this, dx, dy);
}

inline void DockWindow::undock() {
	if(state() < DOCKED_NONE) holder_[0]->undock(this); 
}

void DockWindow::preview_dock(bool clear) {
	if(clear) holder_[0]->preview_dock();
	else if(state() == DOCKED_NONE) holder_[0]->preview_dock(this);
}

inline void DockWindow::preview_dock_cb(DockWindow *dw) {
	dw->preview_dock();
}

/*! Sets the dimensions for both orientation. \a W1 and \a H1 used for 
	horizontal w() and h(). \a W2 and \a H2 used for vertical w() and h().
	Call void min_sizes(int, int, int, int) with these values 
	if \a b is true.
*/
void DockWindow::sizes(int W1, int H1, int W2, int H2, bool b) {
	sizes_[0] = W1; sizes_[1] = H1; sizes_[2] = W2; sizes_[3] = H2;
	if(b) min_sizes(W1, H1, W2, H2);
}

/*! Sets the minimum dimensions for both orientation. \a W1 and \a H1 used 
	for horizontal minimum w() and h(). \a W2 and \a H2 used for vertical 
	minimum w() and h().
*/
void DockWindow::min_sizes(int W1, int H1, int W2, int H2) {
	sizes_[4] = W1; sizes_[5] = H1;
	sizes_[6] = W2; sizes_[7] = H2;
}

void DockWindow::min_sizes(int W, int H, Flags o) {
	if(o) { 
		sizes_[4] = W; sizes_[5] = H;
	} else { 
		sizes_[6] = W; sizes_[7] = H;
	}
}

/*! Sets top most holder where this dock window can dock.
*/
void DockWindow::holder(DockHolder *dh) {
	holder_[0] = dh;
	if(window_->transient()) window_->child_of(dh->parent_window());
	if(dh->status()) dh->status()->add(this);
}

void DockWindow::hide() {
	fix_problem_window_hide = true;
	undock();
	window_->hide();
	clear_visible();
	fix_problem_window_hide = false;
}
//
// End of $Id: DockWindow.cxx,v 1.2 2006/06/23 12:00:12 z_hossain Exp $.
//
