//
// $Id: BarHolder.cxx,v 1.3 2006/06/24 19:37:33 z_hossain Exp $
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

#include <fltk/BarHolder.h>
#include <fltk/DockFlags.h>
#include <Utility.h>

#include <fltk/Box.h>
#include <fltk/layout.h>
#include <math.h>
#include <string.h>

#define TOLERANCE 2
#define MOVE_TOLERANCE 120
#define RANGE_EXT 15

using namespace fltk;

/*! \class fltk::BarHolder

	This class provides the methods for docking and undocking fltk::GripperBar
	and fltk::ToolBar.
*/

/*!
	\b See: DockHolder::DockHolder(int, int, int, int, Flags);
*/
BarHolder::BarHolder(int X, int Y, int W, int H, Flags a) : PlainHolder(X, Y, W, H, a) {
	match_id(MATCH_BARHOLDER);
	type(BAR_HOLDER);
}

/*!
	\b See: DockHolder::DockHolder(int, int, Flags);
*/
BarHolder::BarHolder(int W, int H, Flags a) : PlainHolder(0, 0, W, H, a) {
	match_id(MATCH_BARHOLDER);
	type(BAR_HOLDER);
}

void BarHolder::layout() {
	if (!layout_damage()) return;

	if (!(layout_damage() & (LAYOUT_WH|LAYOUT_DAMAGE))) {
		DockHolder::layout();
		return;
	}
	int damage = layout_damage() & LAYOUT_DAMAGE;
	Widget::layout();
 
	do_layout(damage);
}

bool BarHolder::do_layout(uchar damage) {
	Rectangle r(w(), h());
	box()->inset(r);
	SubBlock *blk;
	DockWindow *dw = 0;
	int X = r.x(), Y = r.y(), temp;
	bool ret = true;
	
	if(orientation()) { // HORIZONTAL {{
	
	for(int i = 0; i < nblock_; i++) {
		blk = block_[i];
		temp = blk->r() - this->w();
		X = r.x();
		blk->h(block_wide_);
		blk->set(X, Y, blk->w(), blk->proper_h());
		for(int j = 0; j < blk->ndockwin_; j++) {
			dw = blk->dockwin_[j];
			if(dw->fillable()) dw->resize(blk->x(), blk->y(), r.w()/*blk->w()*/, blk->h());
			else if(temp > 0) dw->position(X - nonpositive(temp - nonnegative(dw->x() - X)), Y);
			else dw->position(max(dw->x(), X), Y);
			dw->layout();
			X = dw->r();
		}
		temp = max((dw ? dw->r() : r.x()) - r.x(), r.w());
		if(abs(blk->w() - temp) > TOLERANCE) {
			relayout(LAYOUT_DAMAGE);
			ret = false;
		}
		blk->w(temp);
		Y += blk->h();
	}
	temp = Y - r.y() + h() - r.h();
	if(align_holder() == HALIGN_BOTTOM) {
		if(abs(temp - h()) > TOLERANCE) ret = false;
		Widget::resize(x(), y() - temp + h(), w(), temp);
	}
	else Widget::resize(w(), temp);
	
	} // }} HORIZONTAL
	else { // VERTICAL {{
		
	for(int i = 0; i < nblock_; i++) {
		blk = block_[i];
		temp = blk->b() - this->h();
		Y = r.y();
		blk->w(block_wide_);
		blk->set(X, Y, blk->proper_w(), blk->h());
		for(int j = 0; j < blk->ndockwin_; j++) {
			dw = blk->dockwin_[j];
			if(dw->fillable()) dw->resize(blk->x(), blk->y(), blk->w(), r.h()/*blk->h()*/);
			else if(temp > 0) dw->position(X, Y - nonpositive(temp - nonnegative(dw->y() - Y)));
			else dw->position(X, max(dw->y(), Y));
			dw->layout();
			Y = dw->b();
		}
		temp = max((dw ? dw->b() : r.y()) - r.y(), r.h());
		if(abs(blk->h() - temp) > TOLERANCE) {
			relayout(LAYOUT_DAMAGE);
			ret = false;
		}
		blk->h(temp);
		X += blk->w();
	}
	temp = X - r.x() + w() - r.w();
	if(align_holder() == HALIGN_RIGHT) {
		if(abs(temp - w()) > TOLERANCE) ret = false;
		Widget::resize(x() - temp + w(), y(), temp, h());
	}
	else Widget::resize(temp, h());
		
	} // }} VERTICAL
	return ret;
}

void BarHolder::try_undock(DockWindow *dw, int dx, int dy) {
	static int lidx = 0, widx = -1;
	static int sx = 0, sy = 0;
	if(dx == MAX_INT && dy == MAX_INT) { sx = sy = 0; widx = -1; return; }
	if(widx == -1 /*? true : (block_[lidx]->dockwin_[widx] != dw)*/) {
		sx = 0;
		find_block(dw, lidx, widx);
		if(lidx == nblock_) return;
	}
	int xx = dw->x() + dx, yy = dw->y() + dy, temp = 0;
		
	if(orientation()) {
		if(abs(dx) > abs(dy)) {
			if(dx > 0) {
				for(int i = widx; i < block_[lidx]->ndockwin_; i++) {
					temp += block_[lidx]->dockwin_[i]->w();
				}
				if(xx < w() - temp) dw->position(xx, dw->y());
				else if(dx + sx > MOVE_TOLERANCE) {
					if(widx < block_[lidx]->ndockwin_ - 1) {
						block_[lidx]->dockwin_[widx] = block_[lidx]->dockwin_[widx + 1];
						block_[lidx]->dockwin_[++widx] = dw;
						temp = dw->w();
						dw->x(block_[lidx]->dockwin_[widx - 1]->x());
						block_[lidx]->dockwin_[widx - 1]->x(temp);
						layout_damage(LAYOUT_DAMAGE);
						layout();
					}
					else {
						widx = -1; sx = sy = 0;
						undock(dw); return;
					}
					sx = 0;
				}
				else sx += dx;
			}
			else {
				for(int i = widx; i--; ) {
					temp += block_[lidx]->dockwin_[i]->w();
				}
				if(xx > temp) {
					for(int i = 0; i < widx; i++) {
						block_[lidx]->dockwin_[i]->x(block_[lidx]->dockwin_[i]->x() + xx - dw->x());
					}
					dw->position(xx, dw->y());
				}
				else if(abs(dx + sx) > MOVE_TOLERANCE) {
					if(widx > 0) {
						block_[lidx]->dockwin_[widx] = block_[lidx]->dockwin_[widx - 1];
						block_[lidx]->dockwin_[--widx] = dw;
						temp = dw->x();
						dw->x(block_[lidx]->dockwin_[widx + 1]->x());
						block_[lidx]->dockwin_[widx + 1]->x(temp);
						layout_damage(LAYOUT_DAMAGE);
						layout();
					}
					else {
						widx = -1; sx = sy = 0;
						undock(dw); return;
					}
					sx = 0;
				}
				else sx += dx;
			}
		}
		else {
			if(dy + sy > block_[lidx]->h() >> 1) {
				if(((lidx < nblock_ - 1 ? block_[lidx + 1]->dockwin_[0]->fillable() : true)
								&& block_[lidx]->ndockwin_ == 1) || dw->fillable()) {
					widx = -1; sx = sy = 0;
					undock(dw); return;
				}
				block_[lidx]->remove(widx, orientation());
				if(!block_[lidx]->ndockwin_) {
					dw->position(xx, block_[lidx]->b());
					remove_block(lidx);
					adjust(dw);
					widx = -1;
				}
				else { 
					dw->position(xx, dw->y() + (block_[lidx]->h() >> 1)); 
					adjust(dw);
					widx = -1;
				}
				sy = 0;
			}
			else if(dy + sy < -block_[lidx]->h() >> 1) {
				if(((lidx > 0 ? block_[lidx - 1]->dockwin_[0]->fillable() : true) 
								&& block_[lidx]->ndockwin_ == 1) || dw->fillable()) {
					widx = -1; sx = sy = 0;
					undock(dw); return;
				}
				block_[lidx]->remove(widx, orientation());
				if(!block_[lidx]->ndockwin_) {
					dw->position(xx, lidx > 0 ? block_[lidx - 1]->y() : dw->y() - block_[lidx]->b());
					remove_block(lidx);
					adjust(dw);
					widx = -1;
				}
				else { 
					dw->position(xx, dw->y() - 1); 
					adjust(dw);
					widx = -1;
				}
				sy = 0;
			}
			else sy += dy;
		}
	}
	else {
		if(abs(dy) > abs(dx)) {
			if(dy > 0) {
				for(int i = widx; i < block_[lidx]->ndockwin_; i++) {
					temp += block_[lidx]->dockwin_[i]->h();
				}
				if(yy < h() - temp) dw->position(dw->x(), yy);
				else if(dy + sy > MOVE_TOLERANCE) {
					if(widx < block_[lidx]->ndockwin_ - 1) {
						block_[lidx]->dockwin_[widx] = block_[lidx]->dockwin_[widx + 1];
						block_[lidx]->dockwin_[++widx] = dw;
						temp = dw->w();
						dw->y(block_[lidx]->dockwin_[widx - 1]->y());
						block_[lidx]->dockwin_[widx - 1]->y(temp);
						layout_damage(LAYOUT_DAMAGE);
						layout();
					}
					else {
						widx = -1; sx = sy = 0;
						undock(dw); return;
					}
					sy = 0;
				}
				else sy += dy;
			}
			else {
				for(int i = widx; i--; ) {
					temp += block_[lidx]->dockwin_[i]->h();
				}
				if(yy > temp) {
					for(int i = 0; i < widx; i++) {
						block_[lidx]->dockwin_[i]->y(block_[lidx]->dockwin_[i]->y() + yy - dw->y());
					}
					dw->position(dw->x(), yy);
				}
				else if(abs(dy + sy) > MOVE_TOLERANCE) {
					if(widx > 0) {
						block_[lidx]->dockwin_[widx] = block_[lidx]->dockwin_[widx - 1];
						block_[lidx]->dockwin_[--widx] = dw;
						temp = dw->x();
						dw->y(block_[lidx]->dockwin_[widx + 1]->y());
						block_[lidx]->dockwin_[widx + 1]->y(temp);
						layout_damage(LAYOUT_DAMAGE);
						layout();
					}
					else {
						widx = -1; sx = sy = 0;
						undock(dw); return;
					}
					sx = 0;
				}
				else sx += dx;
			}
		}
		else {
			if(dx + sx > block_[lidx]->w() >> 1) {
				if(((lidx < nblock_ - 1 ? block_[lidx + 1]->dockwin_[0]->fillable() : true)
								&& block_[lidx]->ndockwin_ == 1) || dw->fillable()) {
					widx = -1; sx = sy = 0;
					undock(dw); return;
				}
				block_[lidx]->remove(widx, orientation());
				if(!block_[lidx]->ndockwin_) {
					dw->position(block_[lidx]->r(), yy);
					remove_block(lidx);
					adjust(dw);
					widx = -1;
				}
				else { 
					dw->position(dw->x() + (block_[lidx]->w() >> 1), yy); 
					adjust(dw);
					widx = -1;
				}
				sy = 0;
			}
			else if(dx + sx < -block_[lidx]->w() >> 1) {
				if(((lidx > 0 ? block_[lidx - 1]->dockwin_[0]->fillable() : true) 
								&& block_[lidx]->ndockwin_ == 1) || dw->fillable()) {
					widx = -1; sx = sy = 0;
					undock(dw); return;
				}
				block_[lidx]->remove(widx, orientation());
				if(!block_[lidx]->ndockwin_) {
					dw->position(lidx > 0 ? block_[lidx - 1]->x() : dw->x() - block_[lidx]->r(), yy);
					remove_block(lidx);
					adjust(dw);
					widx = -1;
				}
				else { 
					dw->position(dw->x() - 1, yy); 
					adjust(dw);
					widx = -1;
				}
				sy = 0;
			}
			else sx += dx;
		}
	}
}
//
// End of $Id: BarHolder.cxx,v 1.3 2006/06/24 19:37:33 z_hossain Exp $
//
