//
// $Id: DockHolder.cxx,v 1.5 2006/06/24 19:38:06 z_hossain Exp $
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

#include <fltk/DockHolder.h>
#include <fltk/DockFlags.h>
#include <Utility.h>

#include <fltk/Symbol.h>
#include <fltk/layout.h>
#include <fltk/events.h>

#define UNDOCK_TOLERANCE 10
#define CENTER_RATIO 0.05

using namespace fltk;

/*! \class fltk::DockHolder

	The base class for all dock holders in FLTK Dock. It is required to add 
	non floatable and non dockable widgets in fltk::RegionHolder.\n
	Callbacks are done when a fltk::DockWindow is docked in or undocked from 
	holder. The values for when() to control the behavior of callback are:
	\li \b fltk::WHEN_DOCK: Callback is done after a fltk::DockWindow is 
	docked.
	\li \b fltk::WHEN_UNDOCK: Callback is done after a fltk::DockWindow is 
	undocked.
	
	Default value is (fltk::WHEN_DOCK | fltk::WHEN_UNDOCK)\n
	\sa DockHolder::recent()
*/

/*! \fn Flags DockHolder::align_holder() const
	Returns the alignment value
	\sa DockHolder::align_holder(Flags a)
*/

/*! \fn void DockHolder::align_holder(Flags a)
	Sets the alignment value
	The value can be any of these constants
	\li HALIGN_TOP
	\li HALIGN_BOTTOM
	\li HALIGN_LEFT
	\li HALIGN_RIGHT
	\li HALIGN_CENTER

	\sa DockHolder::align_holder() const
*/

/*! \fn DockWindow *DockHolder::recent() const
	Returns the last fltk::DockWindow that is docked in or undocked from 
	holder. DockWindow::state() is used to determine either it is docked 
	or undocked.
*/

/*! \fn void DockHolder::write_status()
	Stores the user changes of status or perspective in disk for future 
	use.
*/

/*! \fn void DockHolder::delete_status()
	Deletes stored status or perspective in disk.
	\sa DockHolder::write_status()
*/

/*!
	This constructor takes values for x(), y(), w(), h() and align_holder().
	\a X, \a Y, \a W, \a H are position and size hints for layout if it is a 
	nested holder.\n
	An autodetect mechanism is used for \c HALIGN_UNKNOWN alignment value and 
	it is adequate for simple layout.\n
	Providing appropriate alignment value is recommended instead of using 
	the default value.
	\sa DockHolder::align_holder(Flags a)
*/
DockHolder::DockHolder(int X, int Y, int W, int H, Flags a) : DockBase(X, Y, W, H) {
	_DockHolder();
	double ratio;
	if(!a && descendant()) {
		ratio = double(w()) / double(parent()->w()) - double(h()) / double(parent()->h());
		if(abs(ratio) < CENTER_RATIO 
				 || Rectangle::contains(parent()->center_x(), parent()->center_y()))
			align_holder(HALIGN_CENTER);
		else if(ratio > 0) {
			if(y() < parent()->center_y()) align_holder(HALIGN_TOP);
			else align_holder(HALIGN_BOTTOM);
		} else {
			if(x() < parent()->center_x()) align_holder(HALIGN_LEFT);
			else align_holder(HALIGN_RIGHT);
		}
	}
	else align_holder(a);
}

/*!
	This constructor takes values for w(), h() and align_holder().
	When it is a nested holder, \a W and \a H are width hint and height hint 
	for layout.
*/
DockHolder::DockHolder(int W, int H, Flags a) : DockBase(0, 0, W, H) {
	_DockHolder();
	align_holder(a);
}

void DockHolder::_DockHolder() {
	range_ = new int[4];
	range_[RANGE_TOP] = range_[RANGE_BOTTOM] = range_[RANGE_LEFT] = range_[RANGE_RIGHT] = 0;
	flags_ = NO_FLAGS;
	box(FLAT_BOX);
	match_id(MATCH_DOCKHOLDER);
	type(fltk::DOCK_HOLDER);
	DockHolder *dh = dynamic_cast<DockHolder *>(parent());
	if(dh) {
		descendant(true);
		preview_ = dh->preview_;
		status_ = dh->status_;
	}	else {
		preview_ = new Preview(0, 0, 0, 0);
		status_ = Global::save_status() ? new DockStatus(this) : 0;
	}
}

DockHolder::~DockHolder() {
	delete [] range_;
	delete preview_;
}

void DockHolder::preview_dock(DockWindow *dw) {
	if(!dw) {
		preview_->clear_flag(Preview::VISIBLE);
		if(!descendant()) redraw(*preview_);
		return;
	}
	Rectangle r1, r2;
	get_absolute_rect(&r1);
	dw->get_absolute_rect(&r2);
	preview_->set(r2.x() - r1.x(), r2.y() - r1.y(), dw->w(), dw->h());
	preview_->set_flag(Preview::VISIBLE);
	if(!descendant()) redraw(*preview_);
}

bool DockHolder::in_range(DockWindow *dw) {
	Rectangle r1, r2;
	get_absolute_rect(&r1);
	dw->get_absolute_rect(&r2);
	
	return dw->dockable() && r2.x() >= r1.x() + range_[RANGE_LEFT] && r2.x() < r1.r() + range_[RANGE_RIGHT] && r2.y() >= r1.y() + range_[RANGE_TOP] && r2.y() < r1.b() + range_[RANGE_BOTTOM];
}

void DockHolder::try_dock(DockWindow *dw) {
	if(in_range(dw)) dock(dw, HALIGN_CENTER);
}

void DockHolder::dock(DockWindow *dw, Flags) {
	Rectangle r1, r2;
	get_absolute_rect(&r1);
	dw->get_absolute_rect(&r2);
	
	dw->window()->hide();
	dw->position(r2.x() - r1.x(), r2.y() - r1.y());
	dw->orientation(orientation());
	add(dw);
	dw->state(DOCKED_XY);
	dw->dock_in(this);
	notify_dock(dw);
}

void DockHolder::try_undock(DockWindow *dw, int dx, int dy) {
	static int sx = 0, sy = 0;
	if(dx == MAX_INT && dy == MAX_INT) { sx = sy = 0; return; }
	if(abs(dx + sx) > UNDOCK_TOLERANCE || abs(dy + sy) > UNDOCK_TOLERANCE) {
		sx = sy = 0;
		if(dw->floatable()) undock(dw);
	}
	else { sx += dx; sy += dy; }
}

void DockHolder::undock(DockWindow *dw) {
	remove(dw);
	dw->showWindow();
	dw->state(DOCKED_NONE);
	dw->dock_in(0);
	notify_undock(dw);
}

DockHolder *DockHolder::parent_holder() {
	DockHolder *dh = dynamic_cast<DockHolder *>(this), *ret;
	do {
		ret = dh;
	} while((dh = dynamic_cast<DockHolder *>(dh->parent())));
	return ret;
}

DockHolder *DockHolder::child_holder(int id) const {
	DockHolder *dh;
	for(int i = children(); i--; ) {
		dh = dynamic_cast<DockHolder *>(child(i));
		if(dh) if(dh->unique_id() == id) return dh;
	}
	return 0;
}

void DockHolder::layout() {
	if (!layout_damage()) return;
	if (!(layout_damage() & (LAYOUT_WH|LAYOUT_DAMAGE)) || !children()) {
		Group::layout();
		return;
	}
	int damage = layout_damage() & LAYOUT_DAMAGE;
	Widget::layout();
  
	Rectangle r(w(), h());
	box()->inset(r);
	
	for (int i = 0; i < children(); i++) {
		Widget* widget = child(i);
		if (!widget->visible()) continue;
		widget->resize(r.x(), r.y(), r.w(), r.h());
		widget->layout_damage(widget->layout_damage()|damage);
		widget->layout();
	}
}

void DockHolder::draw() {
	Group::draw();
	if(!descendant() && preview()->flag(Preview::VISIBLE)) preview()->draw(*this);
}

void DockHolder::notify_dock(DockWindow *dw) {
	//dw->take_focus(); //not working why?
	focus(dw);
	recent_ = dw;
	if(when() & WHEN_DOCK) do_callback();
	else set_changed();
}

void DockHolder::notify_undock(DockWindow *dw) {
	recent_ = dw;
	if(when() & WHEN_UNDOCK) do_callback();
	else set_changed();
}
//
// End of $Id: DockHolder.cxx,v 1.5 2006/06/24 19:38:06 z_hossain Exp $.
//
