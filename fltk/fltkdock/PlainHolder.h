//
// $Id: PlainHolder.h,v 1.2 2006/06/24 19:56:56 z_hossain Exp $
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

#ifndef fltk_PlainHolder_h
#define fltk_PlainHolder_h

#include <fltk/DockWindow.h>
#include <fltk/DockHolder.h>

#define EDGE_GAP 5

namespace fltk {

class SubBlock;

class Resizer : public Rectangle {
public:	
	static int breadth;
	
	Resizer(int = breadth);
	~Resizer();
	void draw(Flags);
	void layout(Rectangle&, int, Flags);
	void move_h(int, DockWindow *, DockWindow *);
	void move_v(int, DockWindow *, DockWindow *);
	void move_v(int, SubBlock *);
	void move_h(int, SubBlock *);
};

class SubBlock : public Rectangle {
public:
	enum {
		ITEM_RESIZABLE = 0x01,
	};
	uchar flags_;
	int ndockwin_;
	DockWindow **dockwin_;
	Resizer **resizer_;
	Resizer *lresizer_;
	
	SubBlock(int, int, bool);
	~SubBlock();
	
	bool item_resizable() { return ITEM_RESIZABLE & flags_; }
	int proper_w();
	int proper_h();
	void insert(DockWindow *, int);
	void remove(int, Flags);
	int adjust(DockWindow *dw, Flags);
	bool can_accept(int xy[], Flags);
	int find(DockWindow *);
};

class PlainHolder : public DockHolder {
	uchar *mark_;

protected:
	int nblock_;
	SubBlock **block_;
	int block_wide_;
	bool resizer_;
	
	PlainHolder(int, int, int, int, Flags);
	
	void insert_block(SubBlock *, int);
	void remove_block(int);
	void adjust(DockWindow *dw, bool = false);
	void find_block(DockWindow *, int &, int &);

	virtual bool do_layout(uchar) = 0;
	
public:
	~PlainHolder();

	void insert_dock(Widget *, int, int);
	void add(Widget *, int);
	void add(DockWindow *, int, int);
	void add(Widget *o) { Group::add(o); }
	void add(Widget &o) { Group::add(o); }
	void init_child();
	
	void try_dock(DockWindow *);
	void dock(DockWindow *, Flags = DOCKED_XY);
	void undock(DockWindow *);
	void preview_dock(DockWindow * = 0);
	
	void end();
};

}
#endif
//
// End of $Id: PlainHolder.h,v 1.2 2006/06/24 19:56:56 z_hossain Exp $
//

