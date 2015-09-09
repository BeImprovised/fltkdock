//
// $Id: PlainHolder.cxx,v 1.2 2006/06/24 19:59:13 z_hossain Exp $
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

#include <PlainHolder.h>
#include <Utility.h>

#include <fltk/layout.h>
#include <fltk/events.h>
#include <string.h>

#define TOLERANCE 5

using namespace fltk;

/*! \class fltk::PlainHolder

	This class contains common part for both fltk::BarHolder and fltk::WindowHolder
*/

/*! \fn void PlainHolder::end();
	Perform initialization and arrangement of new widgets after addition to the parent. 
	Appropriate version of this method called must be ensured since it is not virtual.
*/

PlainHolder::PlainHolder(int X, int Y, int W, int H, Flags a) 
	: DockHolder(X, Y, W, H, a), mark_(0) {
	nblock_ = 0;
	block_ = 0;
	resizer_ = false;
	block_wide_ = 0;
}

PlainHolder::~PlainHolder() {
	delete [] block_;
}

void PlainHolder::insert_block(SubBlock *l, int index) {
	if (!nblock_) {
		block_ = new SubBlock*[1];
	} 
	else {
		if (!(nblock_ & (nblock_-1))) {
			SubBlock** newblock_ = new SubBlock*[nblock_<<1];
			memcpy(newblock_, block_, nblock_*sizeof(SubBlock*));
			delete[] block_;
			block_ = newblock_;
		}
		for (int j = nblock_; j > index; --j) block_[j] = block_[j-1];
	}
	block_[index] = l;
	++nblock_;
}

void PlainHolder::remove_block(int index) {
	if (index >= nblock_) return;
	nblock_--;
	delete block_[index];
	for (int i = index; i < nblock_; i++) block_[i] = block_[i+1];
}

void PlainHolder::find_block(DockWindow *dw, int &b, int &p) {
	for(b = 0; b < nblock_; b++) {
		if((p = block_[b]->find(dw)) < block_[b]->ndockwin_) return;
	}
	b = nblock_; p = -1;
}

void PlainHolder::try_dock(DockWindow * dw) {
	if(in_range(dw)) dock(dw, HALIGN_CENTER);
}

void PlainHolder::dock(DockWindow *dw, Flags a) {	
	dw->window()->hide();
	dw->orientation(orientation());
	adjust(dw);
	add(dw);
	dw->state(DOCKED_XY);
	dw->dock_in(this);
	notify_dock(dw);
}

void PlainHolder::undock(DockWindow *dw) {
	int b, p;
	find_block(dw, b, p);
	if(b < nblock_) {
		block_[b]->remove(p, orientation());
		if(!block_[b]->ndockwin_) remove_block(b);
	}
	remove(dw);
	dw->showWindow();
	dw->state(DOCKED_NONE);
	dw->dock_in(0);
	notify_undock(dw);
}

void PlainHolder::preview_dock(DockWindow *dw) {
	if(dw == 0) { preview()->clear_flag(Preview::VISIBLE); return; }
	
	int t1, t2;
	preview()->clear_flag(Preview::XY_MASK);
	Flags ort = dw->orientation();
	dw->push_sizes();
	push_sizes();
	dw->orientation(orientation());
	adjust(dw, true);
	add(dw);
	while(!do_layout(0));
	dw->get_absolute_rect(preview());
	find_block(dw, t1, t2);
	if(t1 < nblock_) {
		block_[t1]->remove(t2, orientation());
		if(block_[t1]->ndockwin_ == 0) remove_block(t1);
	}
	remove(dw);
	dw->window()->add(dw);
	dw->orientation(ort);
	dw->pop_sizes();
	pop_sizes();
	
	preview()->move_preview();
	preview()->set_flag(Preview::VISIBLE);
	if(!descendant()) redraw(preview()->redraw(*this));
}

void PlainHolder::adjust(DockWindow *dw, bool bpreview) {
	Rectangle r1, r2;
	int xy[2];
	get_absolute_rect(&r1);
	dw->get_absolute_rect(&r2);
	dw->position(r2.x() - r1.x(), r2.y() - r1.y());
	
	xy[0] = event_x_root() - r1.x();
	xy[1] = event_y_root() - r1.y();
	
	int i, next;
	for(i = 0; i < nblock_; i++) {
		if(block_[i]->can_accept(xy, orientation()) && !dw->fillable()) {
			if((next = block_[i]->adjust(dw, orientation())) < block_[i]->ndockwin_ && !bpreview) {
				insert_block(new SubBlock(block_wide_, h(), resizer_), i + 1);
				for(int j = next; j < block_[i]->ndockwin_; j++) {
					block_[i+1]->adjust(block_[i]->dockwin_[j], orientation());
					block_[i]->remove(j, orientation());
				}
			}
			break;
		}
		else if(orientation()) { if(xy[1] < block_[i]->y() + EDGE_GAP) {
			if(i > 0) preview()->set_flag(align_holder() == HALIGN_TOP ? Preview::Y_BEFORE : Preview::Y_AFTER);
			insert_block(new SubBlock(w(), block_wide_, resizer_), i);
			block_[i]->adjust(dw, orientation());
			break;
		} }
		else { if(xy[0] < block_[i]->x() + EDGE_GAP) {
			if(i > 0) preview()->set_flag(align_holder() == HALIGN_LEFT ? Preview::X_BEFORE : Preview::X_AFTER);
			insert_block(new SubBlock(block_wide_, h(), resizer_), i);
			block_[i]->adjust(dw, orientation());
			break;
		} }
	}
	if(i == nblock_) {
		if(orientation()) insert_block(new SubBlock(w(), block_wide_, resizer_), nblock_);
		else insert_block(new SubBlock(block_wide_, h(), resizer_), nblock_);
		block_[nblock_-1]->adjust(dw, orientation());
	}
}

void PlainHolder::end() {
	DockHolder::end();
	DockWindow *dw;
	for(int i = children(); i--; ) {
		dw = dynamic_cast<DockWindow *>(child(i));
		if(dw) {
			dw->orientation(orientation(), true);
			dw->state(align_holder());
			dw->dock_in(this);
			dw->holder(parent_holder());
		}
	}
}

void PlainHolder::add(DockWindow *dw, int l, int p) {
	if(l >= nblock_) {
		l = nblock_;
		insert_block(orientation() ? new SubBlock(w(), dw->h(), resizer_)
			: new SubBlock(dw->w(), h(), resizer_), nblock_);
	}
	block_[l]->insert(dw, p > block_[l]->ndockwin_ ? block_[l]->ndockwin_ : p);
}

void PlainHolder::init_child() {
	delete [] block_;
	block_ = 0; nblock_ = 0;
#if 0
	DockWindow *dw;
	for(int i = children(); i--; ) {
		dw = dynamic_cast<DockWindow *>(child(i));
		if(dw) dw->holder()->status()->read_status(dw);
	}
#endif
	int minx, miny, index = 0, ln = 0;
	if(mark_) delete [] mark_;
	mark_ = new uchar[children()];
	memset(mark_, 1, sizeof(mark_));
	
	if(orientation()) { // HORIZONTAL {{
	while(true) {
		miny = MAX_INT;
		for(int i = 0; i < children(); i++) {
			if(mark_[i]) miny = child(i)->y() < miny ? child(i)->y() : miny;
		}
		if(miny == MAX_INT) break;
		while(true) {
			minx = MAX_INT;
			for(int i = 0; i < children(); i++) {
				if(mark_[i]) if(abs(miny - child(i)->y()) < TOLERANCE) if(child(i)->x() < minx) {
					minx = child(i)->x();
					index = i;
				}
			}
			if(minx != MAX_INT) {
				DockWindow *dw = dynamic_cast<DockWindow *>(child(index));
				if(!dw) continue;
				add(dw, ln, ln < nblock_ ? block_[ln]->ndockwin_ : 0);
				mark_[index] = 0;
			}
			else break;
		}
		ln++;
	}
	}// }} HORIZONTAL
	else { // VERTICAL {{
	while(true) {
		minx = MAX_INT;
		for(int i = 0; i < children(); i++) {
			if(mark_[i]) minx = child(i)->x() < minx ? child(i)->x() : minx;
		}
		if(minx == MAX_INT) break;
		while(true) {
			miny = MAX_INT;
			for(int i = 0; i < children(); i++) {
				if(mark_[i]) if(abs(minx - child(i)->x()) < TOLERANCE) if(child(i)->y() < miny) {
					miny = child(i)->y();
					index = i;
				}
			}
			if(miny != MAX_INT) {
				DockWindow *dw = dynamic_cast<DockWindow *>(child(index));
				if(!dw) continue;
				add(dw, ln, ln < nblock_ ? block_[ln]->ndockwin_ : 0);
				mark_[index] = 0;
			}
			else break;
		}
		ln++;
	}
	}// }} VERTICAL
	layout_damage(LAYOUT_DAMAGE);
	layout();
}
//
// End of $Id: PlainHolder.cxx,v 1.2 2006/06/24 19:59:13 z_hossain Exp $
//
