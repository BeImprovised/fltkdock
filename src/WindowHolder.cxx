//
// $Id: WindowHolder.cxx,v 1.3 2006/06/24 19:38:33 z_hossain Exp $
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

#include <fltk/WindowHolder.h>
#include <fltk/DockFlags.h>
#include <Utility.h>

#include <fltk/draw.h>
#include <fltk/layout.h>
#include <fltk/Symbol.h>
#include <fltk/events.h>
#include <fltk/Cursor.h>
#include <math.h>

#define TOLERANCE 2

using namespace fltk;

/*! \class fltk::WindowHolder

	This class provides the methods for docking and undocking fltk::ToolWindow.
*/

/*!
	\b See: DockHolder::DockHolder(int, int, int, int, Flags);
*/
WindowHolder::WindowHolder(int X, int Y, int W, int H, Flags a) : PlainHolder(X, Y, W, H, a) {
	_WindowHolder();
}

/*!
	\b See: DockHolder::DockHolder(int, int, Flags);
*/
WindowHolder::WindowHolder(int W, int H, Flags a) : PlainHolder(0, 0, W, H, a) {
	_WindowHolder();
}

void WindowHolder::_WindowHolder() {
	match_id(MATCH_WINDOWHOLDER);
	type(WINDOW_HOLDER);
	resizer_ = true;
	block_wide_ = 135;
}

bool WindowHolder::do_layout(uchar damage) {	
	Rectangle r(w(), h());
	box()->inset(r);
	DockWindow *dw;
	int X = r.x(), Y = r.y(), W, H;
	float rf;
	bool ret = true;
	
	if(orientation()) { // HORIZONTAL {{
	
	for(int i = 0; i < nblock_; i++) {
		rf = (float)(r.w()) / (float)(block_[i]->w());
		int j;
		for(j = 0; j < block_[i]->ndockwin_; j++) {
			dw = block_[i]->dockwin_[j];
			W = (int)(round(dw->w() * rf));
			dw->w(W < dw->min_h()? dw->min_h() : W);
		}
		X = r.x();
		if(align_holder() == HALIGN_BOTTOM) {
			block_[i]->lresizer_->layout(*block_[i], Y, ORIENTED_HORIZONTAL);
			Y += block_[i]->lresizer_->h();
		}
// 		block_[i]->h(block_wide_);
		block_[i]->set(X, Y, block_[i]->w(), block_[i]->proper_h());
		for(j = 0; j < block_[i]->ndockwin_; j++) {
			dw = block_[i]->dockwin_[j];
			dw->resize(X, Y, dw->w(), block_[i]->h());
			dw->layout_damage(dw->layout_damage() | damage);
			dw->layout();
			X += dw->w();
			if(j < block_[i]->ndockwin_ - 1) {
				block_[i]->resizer_[j]->layout(*block_[i], X, ORIENTED_VERTICAL);
				X += block_[i]->resizer_[j]->w();
			}
		}
		if(abs(block_[i]->w() - X + r.x()) > TOLERANCE) {
			relayout(LAYOUT_DAMAGE);
			ret = false;
		}
		block_[i]->w(X - r.x());
		Y += block_[i]->h();
	
		if(align_holder() == HALIGN_TOP) {
			block_[i]->lresizer_->layout(*block_[i], Y, ORIENTED_HORIZONTAL);
			Y += block_[i]->lresizer_->h();
		}
	}
	H = Y - r.y() + h() - r.h();
	if(align_holder() == HALIGN_BOTTOM) {
		if(abs(H - h()) > TOLERANCE) ret = false;
		Widget::resize(x(), y() - H + h(), w(), H);
	}
	else Widget::resize(w(), H);
		
	} // }} HORIZONTAL
	else { // VERTICAL {{
	
	for(int i = 0; i < nblock_; i++) {
		rf = double(r.h() / (double)(block_[i]->h()));
		int j;
		for(j = 0; j < block_[i]->ndockwin_; j++) {
			dw = block_[i]->dockwin_[j];
			H = (int)(round(dw->h() * rf));
			dw->h(H < dw->min_h()? dw->min_h() : H);
		}
		Y = r.y();
		if(align_holder() == HALIGN_RIGHT) {
			block_[i]->lresizer_->layout(*block_[i], X, ORIENTED_VERTICAL);
			X += block_[i]->lresizer_->w();
		}
// 		block_[i]->w(block_wide_);
		block_[i]->set(X, Y, block_[i]->proper_w(), block_[i]->h());
		for(j = 0; j < block_[i]->ndockwin_; j++) {
			dw = block_[i]->dockwin_[j];
			dw->resize(X, Y, block_[i]->w(), dw->h());
			dw->layout_damage(dw->layout_damage() | damage);
			dw->layout();
			Y += dw->h();
			if(j < block_[i]->ndockwin_ - 1) {
				block_[i]->resizer_[j]->layout(*block_[i], Y, ORIENTED_HORIZONTAL);
				Y += block_[i]->resizer_[j]->h();
			}
		}
		if(abs(block_[i]->h() - Y + r.y()) > TOLERANCE) {
			relayout(LAYOUT_DAMAGE);
			ret = false;
		}
		block_[i]->h(Y - r.y());
		X += block_[i]->w();
	
		if(align_holder() == HALIGN_LEFT) {
			block_[i]->lresizer_->layout(*block_[i], X, ORIENTED_VERTICAL);
			X += block_[i]->lresizer_->w();
		}
	}
	W = X - r.x() + w() - r.w();
	if(align_holder() == HALIGN_RIGHT) {
		if(abs(W - w()) > TOLERANCE) ret = false;
		Widget::resize(x() - W + w(), y(), W, h());
	}
	else Widget::resize(W, h());
	
	} // }} VERTICAL
	return ret;
}

void WindowHolder::layout() {
	if (!layout_damage()) return;

	if (!(layout_damage() & (LAYOUT_WH|LAYOUT_DAMAGE))) {
		DockHolder::layout();
		return;
	}
	int damage = layout_damage() & LAYOUT_DAMAGE;
	Widget::layout();
  
	do_layout(damage);
}

void WindowHolder::draw() {
	push_clip(Rectangle(w(), h()));
	DockHolder::draw();
	
	for(int i = 0; i < nblock_; i++) {
		for(int j = 0; j < block_[i]->ndockwin_ - 1; j++) {
			block_[i]->resizer_[j]->draw(orientation() ? ORIENTED_VERTICAL : ORIENTED_HORIZONTAL);
		}
		block_[i]->lresizer_->draw(orientation() ? ORIENTED_HORIZONTAL : ORIENTED_VERTICAL);
	}
	pop_clip();
	if(!descendant() && preview()->flag(Preview::VISIBLE)) preview()->draw(*this);
}

int WindowHolder::handle(int event) {
	static int oldx_, oldy_;
	static Resizer *dresizer_;
	static DockWindow *w1, *w2;
	static SubBlock *block;
	int i;

	switch(event) {
		case PUSH:
			for(i = 0; i < nblock_; i++) {
				for(int j = 0; j < block_[i]->ndockwin_ - 1; j++) {
					if(event_inside(*block_[i]->resizer_[j])) {
						dresizer_ = block_[i]->resizer_[j];
						w1 = block_[i]->dockwin_[j];
						w2 = block_[i]->dockwin_[j+1];
						oldx_ = event_x_root();
						oldy_ = event_y_root();
						block = 0;
						return 1;
					}
				}
				if(event_inside(*block_[i]->lresizer_)) {
					dresizer_ = block_[i]->lresizer_;
					block = block_[i];
					w1 = 0;
					oldx_ = event_x_root();
					oldy_ = event_y_root();
					return 1;
				}
			}
			break;
		case RELEASE:
			dresizer_ = 0;
			break;
		case DRAG:
			if(dresizer_ && w1) {
				if(orientation()) {
					dresizer_->move_v(event_x_root() - oldx_, w1, w2);
					layout_damage(LAYOUT_DAMAGE);
					layout();
					oldx_ = event_x_root();
				}
				else {
					dresizer_->move_h(event_y_root() - oldy_, w1, w2);
					layout_damage(LAYOUT_DAMAGE);
					layout();
					oldy_ = event_y_root();
				}
				return 1;
			}
			else if(dresizer_ && block) {
				if(orientation()) {
					switch(align_holder()) {
						case ALIGN_TOP: dresizer_->move_h(event_y_root() - oldy_, block); break;
						case ALIGN_BOTTOM: dresizer_->move_h(oldy_ - event_y_root(), block); break;
					}
					layout_damage(LAYOUT_DAMAGE);
					layout();
					oldy_ = event_y_root();
				}
				else {
					switch(align_holder()) {
						case ALIGN_LEFT: dresizer_->move_v(event_x_root() - oldx_, block); break;
						case ALIGN_RIGHT: dresizer_->move_v(oldx_ - event_x_root(), block); break;
					}
					layout_damage(LAYOUT_DAMAGE);
					layout();
					oldx_ = event_x_root();
				}
				return 1;
			}
			break;
		case MOVE:
			for(i = 0; i < nblock_; i++) {
				for(int j = 0; j < block_[i]->ndockwin_ - 1; j++) {
					if(event_inside(*block_[i]->resizer_[j])) {
						if(orientation()) cursor(CURSOR_WE);
						else cursor(CURSOR_NS);
						return 1;
					}
				}
				if(event_inside(*block_[i]->lresizer_)) {
					if(orientation()) cursor(CURSOR_NS);
					else cursor(CURSOR_WE);
					return 1;
				}
			}
			cursor(0);
			break;
		case LEAVE:
			cursor(0);
			return 1;
			break;	
	}
	return DockHolder::handle(event);
}
//
// End of $Id: WindowHolder.cxx,v 1.3 2006/06/24 19:38:33 z_hossain Exp $
//
