//
// $Id: Resizer.cxx,v 1.2 2006/06/24 19:59:38 z_hossain Exp $
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

#include <fltk/draw.h>

#define RESIZER_DOT_GAP 8
#define RESIZER_DOT_NO 3

using namespace fltk;

int Resizer::breadth = 5;

Resizer::Resizer(int b) : Rectangle (0, 0, 0, 0) {
	breadth = b;
}

Resizer::~Resizer() {
}

void Resizer::draw(Flags o) {	
	int X = x() + w()/2, mx;
	int Y = y() + h()/2, my;

	setcolor(GRAY33);
	fillrect(Rectangle(X - 1, Y - 1, 2, 2));
	setcolor(WHITE);
	fillrect(Rectangle(X, Y, 2, 2));
	
	int rno = (RESIZER_DOT_NO - 1) / 2;
	for(int i = 1; i <= rno; i++) {
		if(w() < h()) { mx = 0; my = i * RESIZER_DOT_GAP; }
		else { mx = i * RESIZER_DOT_GAP; my = 0; }
		
		setcolor(GRAY33);
		fillrect(Rectangle(X + mx - 1, Y + my - 1, 2, 2));
		setcolor(WHITE);
		fillrect(Rectangle(X + mx, Y + my, 2, 2));
	
		setcolor(GRAY33);
		fillrect(Rectangle(X - mx - 1, Y - my - 1, 2, 2));
		setcolor(WHITE);
		fillrect(Rectangle(X - mx, Y - my, 2, 2));
	}
}

void Resizer::layout(Rectangle &r, int pos, Flags o) {
	if(o) set(r.x(), pos, r.w(), breadth);
	else set(pos, r.y(), breadth, r.h());
}

void Resizer::move_h(int dp, DockWindow *w1, DockWindow *w2) {
	if(w1->h() + dp > w1->min_h() && w2->h() - dp >= w2->min_h()) {
		w1->move_y(-dp);
		w2->move_y(+dp);
		move_y(dp);
	}
}

void Resizer::move_v(int dp, DockWindow *w1, DockWindow *w2) {
	if(w1->w() + dp > w1->min_w() && w2->w() - dp >= w2->min_w()) {
		w1->move_x(-dp);
		w2->move_x(+dp);
		move_x(dp);
	}
}

void Resizer::move_h(int dp, SubBlock *line) {
	for(int i = 0; i < line->ndockwin_; i++) {
		if(line->dockwin_[i]->h() + dp < line->dockwin_[i]->min_h()) return;
	}
	line->move_b(dp);
	move_y(dp);
}

void Resizer::move_v(int dp, SubBlock *line) {
	for(int i = 0; i < line->ndockwin_; i++) {
		if(line->dockwin_[i]->w() + dp < line->dockwin_[i]->min_w()) return;
	}
	line->move_r(dp);
	move_x(dp);
}

//
// End of $Id: Resizer.cxx,v 1.2 2006/06/24 19:59:38 z_hossain Exp $
//
