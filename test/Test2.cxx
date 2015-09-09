//
// $Id: Test2.cxx,v 1.6 2006/06/24 19:36:51 z_hossain Exp $
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

#include <fltk/Window.h>
#include <fltk/run.h>
#include <stdlib.h>
#include <string.h>

#include <fltk/MenuBar.h>
#include <fltk/ItemGroup.h>
#include <fltk/Item.h>
#include <fltk/HighlightButton.h>
#include <fltk/Clock.h>
#include <fltk/InputBrowser.h>
#include <fltk/WordwrapInput.h>
#include <fltk/ask.h>

#include <fltk/RegionHolder.h>
#include <fltk/DockFlags.h>
#include <fltk/ToolWindow.h>
#include <fltk/WindowHolder.h>
#include <fltk/BarHolder.h>
#include <fltk/GripperBar.h>
#include <fltk/ToolBar.h>
#include <fltk/ToolDivider.h>


using namespace fltk;

Widget *center = 0;
RegionHolder *dg;
char c[100];

void action_cb(Widget* w, void* p) {
	DockHolder *dh = (DockHolder *)p;
	DockWindow *dw = dh->recent();
	strcpy(c, "Window \"");
	if(dw->type() == TOOL_WINDOW) strcat(c, ((ToolWindow *)dw)->label());
	strcat(c, "\" has recently ");
	strcat(c, (dw->state() < DOCKED_NONE ? "Docked" : "Undocked"));
	center->label(c);
	center->redraw();
}
void close_cb(Widget*, void* v) {
	if(ask("Do you want to save Status/Perspective?", "Save"))
		dg->write_status();//if you only want to store settings
	exit(0);
}

void win_cb(Widget*w, void* v) {
	DockWindow *dw = (DockWindow *)v;
	dw->child(0)->label(dw->state() < DOCKED_NONE ? "Four\nDocked" : "Four\nUndocked");
}

void menu_cb(Widget*w, void* v) {
	dg->delete_status();
	message("To get the default Status/Perspective don't save on exit");
}

void different(ToolWindow *tw) {
	tw->titlecolor(BLUE);
	tw->labelcolor(YELLOW);
	tw->highlight_color(RED);
	tw->highlight_textcolor(WHITE);	
}

void create_toolbar(RegionHolder *dg, int X, int Y, int br) {
	Item *o;
	ToolBar *tb = new ToolBar(X, Y, 350, 40);
	tb->begin();
	new HighlightButton(0, 0, 40, 40, "one");
	new HighlightButton(0, 0, 40, 40, "two");
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
		tb->border(true, "ToolBar");
	}
	tb->end();
}

int main(int argc, char **argv) {
	
// 	Global::highlight_title(false);		//default true; //highlight window title
// 	Global::save_status(false);				//default true; //Save user setting
// 	Global::preview_delay(1.0f);			//default 0.3f;
// 	Global::fill_vertical(true);			//default false; //see documentation
	
	Global::app_name("test2.fltkdock.sourceforge.net");
	//above line is required when more than one application in a system use this framework.
	//and also save_status(true)
	
	Window *win = new Window(700, 550, "Test");
	win->begin();
	
	dg = new RegionHolder(0, 0, win->w(), win->h());
	//when() options are WHEN_DOCK, WHEN_UNDOCK
	dg->callback(action_cb, dg);
	dg->begin();
	
	WindowHolder *wh4 =  new WindowHolder(700, 135, HALIGN_BOTTOM);
	wh4->begin();
	ToolWindow *tlw4 = new ToolWindow(0, 0, 133, 135, "Test4");
	//when() options are WHEN_DOCK, WHEN_UNDOCK
	tlw4->callback(win_cb, tlw4);
	tlw4->min_sizes(50, 50);
	tlw4->begin();
	new Widget(0, 0, 100, 100, "Four");
	tlw4->end();
	ToolWindow *tlw5 = new ToolWindow(135, 0, 550, 135, "Test5");
	tlw5->sizes(200, 200);
	tlw5->min_sizes(0, 0);
	tlw5->begin();
	new Widget(0, 0, 100, 100, "Five");
	tlw5->end();
	wh4->end();
	DockHolder *dh = new DockHolder(200, 500, HALIGN_CENTER);
	dh->begin();
	center = new Widget(0, 0, 100, 100, "No recent action detected");
	center->align(ALIGN_WRAP);
	dh->end();
	WindowHolder *wh1 =  new WindowHolder(135, 550, HALIGN_LEFT);
	wh1->begin();
	ToolWindow *tlw = new ToolWindow(0, 0, 135, 100, "Test1");
	tlw->min_sizes(50, 50);
	tlw->begin();
	new WordwrapInput(0, 0, 100, 100);
	tlw->end();
	ToolWindow *tlw2 = new ToolWindow(0, 150, 135, 400, "Test2");
	different(tlw2); //it create a different look
	tlw2->sizes(200, 200);
	tlw2->min_sizes(50, 50);
	tlw2->begin();
	new Widget(0, 0, 100, 200, "Two");
	tlw2->end();
	ToolWindow *tlw3 = new ToolWindow(150, 0, 135, 150, "Test3");
	tlw3->min_sizes(50, 50);
	tlw3->begin();
	new Widget(0, 0, 100, 200, "Three");
	tlw3->end();
	wh1->end();
	new WindowHolder(150, 550, HALIGN_RIGHT);
	new WindowHolder(700, 100, HALIGN_TOP);
	BarHolder *bh = new BarHolder(700, 100, HALIGN_TOP);
	bh->begin();
	GripperBar *gb = new GripperBar(0, 0, 500, 27);
	gb->type(GRIP_MENUBAR);
	gb->sizes(200, 30, 50, 200, true);
	gb->begin();
	MenuBar *menubar = new MenuBar(0, 0, 500, 25);
	menubar->begin();
	ItemGroup *file = new ItemGroup("&File");
	file->begin();
	new Item("&Delete Status/Perspective", 0, menu_cb);
	file->end();
	ItemGroup *edit = new ItemGroup("&Edit");
	edit->end();
	menubar->end();
	gb->end();
	create_toolbar(dg, 0, 27, 0x0);
	create_toolbar(dg, 295, 27, 0x1);
	create_toolbar(dg, 600, 27, 0x2);
	bh->end();
	new BarHolder(700, 100, HALIGN_BOTTOM);
	new BarHolder(100, 550, HALIGN_LEFT);
	new BarHolder(100, 550, HALIGN_RIGHT);
	dg->end();
	win->resizable(dg);
	win->end();
	win->show(argc, argv);
	
	ToolWindow *tlw10 = new ToolWindow(200, 200, 200, 200, "Test10");
// tlw10->window()->transient(false);   //for top-level window
	tlw10->min_sizes(100, 100);
	tlw10->resizer(true); //For adding a resizer corner
	tlw10->begin();
	new Widget(0, 0, 100, 200, "Ten");
	tlw10->end();
	tlw10->holder(dg);
	
	win->callback((fltk::Callback *)close_cb, win);
		
	dg->initiate();
	return run();
}

//
// End of $Id: Test2.cxx,v 1.6 2006/06/24 19:36:51 z_hossain Exp $.
//
