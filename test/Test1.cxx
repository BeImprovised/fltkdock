//
// $Id: Test1.cxx,v 1.4 2006/06/24 20:02:35 z_hossain Exp $
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

#include <fltk/Window.h>
#include <fltk/run.h>
#include <stdlib.h>

#include <fltk/MenuBar.h>
#include <fltk/ItemGroup.h>
#include <fltk/Item.h>
#include <fltk/HighlightButton.h>
#include <fltk/Clock.h>
#include <fltk/InputBrowser.h>
#include <fltk/WordwrapInput.h>
#include <fltk/xpmImage.h>

#include <fltk/RegionHolder.h>
#include <fltk/DockFlags.h>
#include <fltk/BarHolder.h>
#include <fltk/GripperBar.h>
#include <fltk/ToolBar.h>
#include <fltk/ToolDivider.h>

static char *computer_xpm[] = {
"31 27 5 1",
" 	c None",
"&	c #000100",
"+	c #B1B2B1",
"*	c #E5E7E4",
"#	c #798DA5",
"                               ",
"       &&&&&&&&&&&&&&&&&       ",
"      &+***************+&      ",
"      &*****************&      ",
"      &**#&&&&&&&&&&&#**&      ",
"      &**&***********&**&      ",
"      &**&######***##&**&      ",
"      &**&######***##&**&      ",
"      &**&######***##&**&      ",
"      &**&######***##&**&      ",
"      &**&######***##&**&      ",
"      &**&###########&**&      ",
"      &**&###########&**&      ",
"      &**#&&&&&&&&&&&#**&      ",
"      &*****************&      ",
"      &+***************+&      ",
"       &+++++++++++++++&       ",
"      &&&&&&&&&&&&&&&&&&&      ",
"      &*****************&      ",
"      &*****************&      ",
"      &+++++++++++++++++&      ",
"    &&&&&&&&&&&&&&&&&&&&&&&    ",
"   &+++++++++++++++++++++++&   ",
"  &+++++++++++++++++++++++++&  ",
" &+++++++++++++++++++++++++++& ",
" &&&&&&&&&&&&&&&&&&&&&&&&&&&&& ",
"                               "};


using namespace fltk;

xpmImage *pixmap = new xpmImage(computer_xpm);

void close_cb(Widget*, void*) {exit(0);}
void create_toolbar(RegionHolder *dg, int X, int Y, int br) {
	Item *o;
	ToolBar *tb = new ToolBar(X, Y, 350, 40);
	tb->begin();
	(new HighlightButton(0, 0, 40, 40))->image(pixmap);
	(new HighlightButton(0, 0, 40, 40))->image(pixmap);
	new ToolDivider();
	new HighlightButton(0, 0, 40, 40, "three");
	if(br & 0x01) (new Clock(0,0,40,40))->box(THIN_DOWN_BOX);
	if(br & 0x02) {
		fltk::InputBrowser* ib = new fltk::InputBrowser(0, 0, 80, 23);
		ib->begin();
		o = new fltk::Item("Red"); o->set_vertical();
		o = new fltk::Item("Green"); o->set_vertical();
		ib->end();
		ib->value(0);
		tb->child_info(ToolBar::FIXED_HEIGHT | ToolBar::FIXED_WIDTH, 10, 10);
		tb->min_sizes(10, 40, 100, 10);
	}
	tb->end();
}

int main(int argc, char **argv) {
	Global::save_status(false); //disable the saving of user setting
	
	Window *win = new Window(700, 550, "Test");
	win->begin();
	
	RegionHolder *dg = new RegionHolder(0, 0, win->w(), win->h());
	win->resizable(dg); //put this here for fltk...
	dg->begin();
	
	DockHolder *dh = new DockHolder(200, 500, HALIGN_CENTER);
	dh->begin();
	WordwrapInput *input = new WordwrapInput(0, 0, 100, 100);
	input->static_text("hold down the CTRL key prevent from dock while dragging.");
	dh->end();
	BarHolder *bh = new BarHolder(700, 100, HALIGN_TOP);
	bh->begin();
	GripperBar *gb = new GripperBar(0, 0, 500, 27);
	gb->type(GRIP_MENUBAR);
	gb->sizes(100, 30, 50, 200, true);
	gb->begin();
	MenuBar *menubar = new MenuBar(0, 0, 500, 25);
	menubar->begin();
	ItemGroup *file = new ItemGroup("&File");
	file->end();
	ItemGroup *edit = new ItemGroup("&Edit");
	edit->end();
	menubar->end();
	gb->end();
	create_toolbar(dg, 0, 27, 0x0);
	create_toolbar(dg, 520, 27, 0x1);
	create_toolbar(dg, 0, 67, 0x2);
	bh->end();
	new BarHolder(700, 100, HALIGN_BOTTOM); //enable user to dock at bottom
	new BarHolder(100, 550, HALIGN_LEFT);		//enable user to dock at left
	new BarHolder(100, 550, HALIGN_RIGHT);	//enable user to dock at right
	//skip above two lines if you donot want to dock vertical (LEFT/RIGHT)
	dg->end();
	win->end();
	win->show(argc, argv);
	win->callback((fltk::Callback *)close_cb, win);
	dg->initiate();
	return run();
}
//
// End of $Id: Test1.cxx,v 1.4 2006/06/24 20:02:35 z_hossain Exp $
//
