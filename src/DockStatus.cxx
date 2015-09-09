//
// $Id: DockStatus.cxx,v 1.3 2006/06/24 19:57:47 z_hossain Exp $
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

#include <fltk/RegionHolder.h>
#include <DockStatus.h>

#include <fltk/Preferences.h>
#include <fltk/run.h>
#include <stdio.h>
#include <string.h>

using namespace fltk;

enum {
	VISIBLE = 0x100
};

DockStatus::DockStatus(DockHolder *dh): ndockwin_(0), dockwin_(0) {
	char buffer[15];
	sprintf(buffer, "fltkdock%d", dh->unique_id());
	pref_ = new Preferences(Preferences::USER, Global::app_name(), buffer);
	holder_ = dh;
}

void DockStatus::add(DockWindow *dw) {
	for(int i = ndockwin_; i--; ) {
		if(dockwin_[i] == dw) return;
	}
	if (!ndockwin_) dockwin_ = new DockWindow*[1];
	else if (!(ndockwin_ & (ndockwin_-1))) {
		DockWindow** newdockwin_ = new DockWindow*[ndockwin_ << 1];
		memcpy(newdockwin_, dockwin_, ndockwin_*sizeof(DockWindow*));
		delete[] dockwin_;
		dockwin_ = newdockwin_;
	}
	dockwin_[ndockwin_] = dw;
	ndockwin_++;
}

void DockStatus::read_parent(Window *win) {
	int r[4];
	Preferences p(pref_, "Parent");
	p.get("X", r[0], win->x());
	p.get("Y", r[1], win->y());
	p.get("Width", r[2], win->w());
	p.get("Height", r[3], win->h());
	win->resize(r[0], r[1], r[2], r[3]);
}

void DockStatus::delete_status() {
	for(int i = pref_->groups(); i--; ) {
		pref_->deleteGroup(pref_->group(i));
	}
	pref_->flush();
}

void DockStatus::read_status(DockWindow *dw) {
	int r[4];
	Widget *temp;
	char buffer[15];
	sprintf(buffer, "DW%d", dw->unique_id());
	Preferences a(pref_, buffer);
	
	a.get("User-Flags", r[0], dw->user_flags() | (dw->visible() ? VISIBLE : 0));
	dw->state(r[0] & DOCKED_MASK);
	dw->orientation(r[0] & ORIENTED_MASK);
	
	if(r[0] & VISIBLE) {
		dw->set_visible();
		if(dw->state() == DOCKED_NONE) {
			dw->showWindow();
		}
	} else {
		if(dw->parent()) dw->parent()->remove(dw);
		dw->clear_visible();
		dw->window()->hide();
	}
	
	if(dw->state() == DOCKED_NONE) temp = dw->window();
	else temp = dw;
	a.get("X", r[0], temp->x());
	a.get("Y", r[1], temp->y());
	a.get("Width", r[2], temp->w());
	a.get("Height", r[3], temp->h());
	temp->resize(r[0], r[1], r[2], r[3]);
	
	a.get("Holder-ID", r[0], dw->dock_in() ? dw->dock_in()->unique_id() : 0);
	dw->dock_in(holder_->child_holder(r[0]));
	if(dw->dock_in()) {
		dw->window()->hide();
		dw->dock_in()->add(dw);
	}
}

void DockStatus::write_status(DockHolder *dh) {
	Widget *temp;
	char buffer[15];
	sprintf(buffer, "fltkdock%d", dh->unique_id());
	Preferences dock(Preferences::USER, Global::app_name(), buffer);
	Preferences p(dock, "Parent");
	Window *pwin = dh->parent_window();
	p.set("X", pwin->x());
	p.set("Y", pwin->y());
	p.set("Width", pwin->w());
	p.set("Height", pwin->h());
	
	for(int i = 0; i < ndockwin_; i++) {
		sprintf(buffer, "DW%d", dockwin_[i]->unique_id());
		Preferences a(dock, buffer);
		a.set("User-Flags", (int)dockwin_[i]->user_flags() 
				| (dockwin_[i]->visible() ? VISIBLE : 0));
		if(dockwin_[i]->state() == DOCKED_NONE) temp = dockwin_[i]->window();
		else temp = dockwin_[i];
		a.set("X", temp->x());
		a.set("Y", temp->y());
		a.set("Width", temp->w());
		a.set("Height", temp->h());
		a.set("Holder-ID", dockwin_[i]->dock_in() ? dockwin_[i]->dock_in()->unique_id() : 0);
	}
}

//
// End of $Id: DockStatus.cxx,v 1.3 2006/06/24 19:57:47 z_hossain Exp $
//
