//
// $Id: SubBlock.cxx,v 1.2 2006/06/23 12:03:24 z_hossain Exp $
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

#include <string.h>

using namespace fltk;

SubBlock::SubBlock(int W, int H, bool ir) : Rectangle(0, 0, W, H) {
	ndockwin_ = 0;
	dockwin_ = 0;
	resizer_ = 0;
	lresizer_ = 0;
	if(ir) {
		flags_ = flags_ | ITEM_RESIZABLE;
		lresizer_ = new Resizer();
	}
	else flags_ = flags_ & ~ITEM_RESIZABLE;
}

SubBlock::~SubBlock() {
	delete [] dockwin_;
	if(resizer_) delete [] resizer_;
	if(lresizer_) delete lresizer_;
}

int SubBlock::proper_w() {
	if(!ndockwin_) return 0;
	int maxw = w();
	for(int i = 0; i < ndockwin_; i++) {
		if(dockwin_[i]->min_w() > maxw) maxw = dockwin_[i]->min_w();
	}
	return maxw;
}

int SubBlock::proper_h() {
	if(!ndockwin_) return 0;
	int maxh = h();
	for(int i = 0; i < ndockwin_; i++) {
		if(dockwin_[i]->min_h() > maxh) maxh = dockwin_[i]->min_h();
	}
	return maxh;
}

void SubBlock::insert(DockWindow *dw, int index) {
	if (!ndockwin_) dockwin_ = new DockWindow*[1];
	else {
		if (!(ndockwin_ & (ndockwin_ - 1))) {
			DockWindow** newdockwin_ = new DockWindow*[ndockwin_ << 1];
			memcpy(newdockwin_, dockwin_, ndockwin_*sizeof(DockWindow*));
			delete [] dockwin_;
			dockwin_ = newdockwin_;
		}
		for (int j = ndockwin_; j > index; j--) dockwin_[j] = dockwin_[j-1];
	}
	dockwin_[index] = dw;
	if(!item_resizable()) { ++ndockwin_; return; }
	
	if (!ndockwin_) resizer_ = new Resizer*[1];
	else {
		Resizer** newresizer_ = new Resizer*[ndockwin_ << 1];
		memcpy(newresizer_, resizer_, ndockwin_*sizeof(Resizer*));
		delete [] resizer_;
		resizer_ = newresizer_;
	}
	if(ndockwin_ > 0) resizer_[ndockwin_ - 1] = new Resizer();
	++ndockwin_;
}

void SubBlock::remove(int index, Flags orientation) {
	if (index >= ndockwin_) return;
	ndockwin_--;
	for (int i = index; i < ndockwin_; i++) dockwin_[i] = dockwin_[i+1];
	
	if(!item_resizable()) return;
	if(ndockwin_ > 0) delete resizer_[ndockwin_ - 1];
	else return; // NOT WINDOW HOLDER
		
	if(orientation) {
		double rf = 1 + (dockwin_[ndockwin_]->w() + Resizer::breadth) / (double)w();
		for(int i = 0; i < ndockwin_; i++) dockwin_[i]->w(int(dockwin_[i]->w() * rf));
	}
	else {
		double rf = 1 + (dockwin_[ndockwin_]->h() + Resizer::breadth) / (double)h();
		for(int i = 0; i < ndockwin_; i++) dockwin_[i]->h(int(dockwin_[i]->h() * rf));
	}
	//DON'T DELETE dockwin_[ndockwin_];
}

int SubBlock::adjust(DockWindow *dw, Flags orientation) {
	int i;
	if(orientation) { // HORIZONTAL {{
	for(i = 0; i < ndockwin_; i++) {
		if(dockwin_[i]->center_x() > dw->x()) { insert(dw, i); break; }
		else if(dockwin_[i]->r() > dw->x()) { insert(dw, i + 1); break; }
	}
	if(i == ndockwin_) insert(dw, i);
	
	if(item_resizable()) { // WINDOW HOLDER {{
		int W = w() - max(0, Resizer::breadth * (ndockwin_ - 1));
		double rf = 1 - 1 / (double)(ndockwin_) - Resizer::breadth / (double)W;
		dw->w(W / (ndockwin_));
		
		for(int i = 0, X = 0; i < ndockwin_; i++) {
			if(dockwin_[i] != dw) {
				dockwin_[i]->w(max(dockwin_[i]->min_w(), int(dockwin_[i]->w() * rf)));
			}
			if(X + dockwin_[i]->w() > w() && i > 0) {
				dockwin_[i - 1]->move_r(r() - dockwin_[i - 1]->r());
				return i;
			}
			X += dockwin_[i]->w() + Resizer::breadth;
		}
	}	// }} WINDOW HOLDER
	else { // BAR HOLDER {{
		for(int i = 0, X = 0; i < ndockwin_; i++) {
			X += dockwin_[i]->min_w();
			if(X > w() && i > 0) return i;
		}
	} // }} BAR HOLDER
	} // }} HORIZONTAL
	else { // VERTICAL {{
	for(i = 0; i < ndockwin_; i++) {
		if(dockwin_[i]->center_y() > dw->y()) { insert(dw, i); break; }
		else if(dockwin_[i]->b() > dw->y()) { insert(dw, i + 1); break; }
	}
	if(i == ndockwin_) insert(dw, i);
	
	if(item_resizable()) { // WINDOW HOLDER {{
		int H = h() - max(0, Resizer::breadth * (ndockwin_ - 1));
		double rf = 1 - 1 / (double)(ndockwin_) - Resizer::breadth / (double)H;
		dw->h(H / (ndockwin_));
		
		for(int i = 0, Y = 0; i < ndockwin_; i++) {
			if(dockwin_[i] != dw) {
				dockwin_[i]->h(max(dockwin_[i]->min_h(), int(dockwin_[i]->h() * rf)));
			}
			if(Y + dockwin_[i]->h() > h() && i > 0) {
				dockwin_[i - 1]->move_b(b() - dockwin_[i - 1]->b());
				return i;
			}
			Y += dockwin_[i]->h() + Resizer::breadth;
		}
	} // }} WINDOW HOLDER
	else { // BAR HOLDER {{
		for(int i = 0, Y = 0; i < ndockwin_; i++) {
			Y += dockwin_[i]->min_h();
			if(Y > h() && i > 0) return i;
		}
	} // }} BAR HOLDER
	} // }} VERTICAL
	return ndockwin_;
}

int SubBlock::find(DockWindow *dw) {
	for(int i = ndockwin_; i--;) {
		if(dockwin_[i] == dw) return i;
	}
	return ndockwin_;
}

bool SubBlock::can_accept(int xy[], Flags orientation) {
	if(ndockwin_ >= 0) if(dockwin_[0]->fillable()) return false;

	if(orientation) return xy[1] >= y() + EDGE_GAP
				&& xy[1] < min(b() - h() / 2 + EDGE_GAP, b() - EDGE_GAP);
	else return xy[0] >= x() + EDGE_GAP
				&& xy[0] < min(r() - w() / 2 + EDGE_GAP, r() - EDGE_GAP);
}
//
// End of $Id: SubBlock.cxx,v 1.2 2006/06/23 12:03:24 z_hossain Exp $
//
