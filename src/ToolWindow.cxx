//
// $Id: ToolWindow.cxx,v 1.3 2006/06/23 21:36:56 z_hossain Exp $
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

#include <fltk/ToolWindow.h>
#include <Utility.h>

#include <fltk/draw.h>
#include <fltk/layout.h>
#include <fltk/Symbol.h>
#include <fltk/events.h>
#include <fltk/Font.h>
#include <fltk/Color.h>

// #include <fltk/Window.h>
// #include <fltk/x.h>
// #include <X11/extensions/XInput.h>
// #include <X11/extensions/XI.h>


#define FACTOR 0.6

using namespace fltk;

/*
********************************************************************************
                                     Title
********************************************************************************
*/
Title::Title(const char *l, Widget *p, int h) : Rectangle(0, 0, 0, h) {
	label_ = l;
	p->labelcolor(BLACK);
	p->labelfont(p->labelfont()->bold());
	p->align(ALIGN_LEFT | ALIGN_INSIDE);
	p->highlight_color(color(140, 168, 202));
	p->highlight_textcolor(BLACK);
	color_ = GRAY33;
	flags_ = NO_FLAGS;
}

void Title::draw(Widget *parent) {
	push_clip(*this);
	
	Color color;
	if(flags_ & HIGHLIGHTED) color = parent->highlight_color();
	else color = color_;
	
	setcolor(color);
	fillrect(*this);
	setcolor(darker_color(color));
	strokerect(*this);
	
	Rectangle close(r() - h(), y() + 3, h() - 6, h() - 6);
	setfont(parent->labelfont(), parent->labelsize());
	if(flags_ & HIGHLIGHTED) setcolor(parent->highlight_textcolor());
	else setcolor(parent->labelcolor());
	Rectangle r(*this);
	r.set(8, r.y(), r.w() - h() - 7, r.h());
	push_clip(r);
	drawtext(label_, r, parent->align());
	pop_clip();
	if(flags_ & PUSHED) {
		setcolor(WHITE);
		drawtext("@-31+", close, ALIGN_INSIDE | ALIGN_CENTER);
	}
	else drawtext("@-31+", close, ALIGN_INSIDE | ALIGN_CENTER);
	pop_clip();
}

int Title::handle(int event, DockWindow *dw) {
	Rectangle close(r() - h(), y() + 3, h() - 6, h() - 6);
	if(!close.contains(event_x(), event_y())) return 0;
	
	switch(event) {
		case PUSH:
			set_flag(PUSHED);
			dw->redraw(close);
			return 1;
		case RELEASE:
			clear_flag(PUSHED);
			dw->redraw(close);
			dw->hide();
			return 1;
		case MOVE:
		case DRAG:
			clear_flag(PUSHED);
			dw->redraw(close);
	}
	return 0;
}

Color Title::darker_color(Color c) {
	uchar r, g, b;
	split_color(c, r, g, b);
	return color(max(int(r * FACTOR), 0),
							 max(int(g * FACTOR), 0),
							 max(int(b * FACTOR), 0));
}

/*
********************************************************************************
                                     ToolWindow
********************************************************************************
*/

/*! \class fltk::ToolWindow
	
	This class provide the functionality of a dockable-floatable window.
*/
ToolWindow::ToolWindow(int X, int Y, int W, int H, const char *l)
	:DockWindow(X, Y, W, H) {
	box(FLAT_BOX);
	dockable(DOCKABLE_ALL);
	floatable(true);
	fillable(false);
	orientable(ORIENTABLE_NONE);
	sizes(W, H);
	match_id(MATCH_WINDOWHOLDER | MATCH_REGIONHOLDER);
	type(TOOL_WINDOW);
	title_ = new Title(l, this);
	dragger(title_);
	build_menu();
	if(Global::highlight_title()) {
		set_click_to_focus();
		set_tab_to_focus();
	}
}

ToolWindow::~ ToolWindow() {
}

void ToolWindow::layout() {
	if (!layout_damage()) return;
	if (!(layout_damage() & (LAYOUT_WH|LAYOUT_DAMAGE|LAYOUT_CHILD))) {
		return;
	}
	int damage = layout_damage() & LAYOUT_DAMAGE;
	Widget::layout();
	
	Rectangle r(w(), h());
	box()->inset(r);
	
	title_->set(r.x() + 1, r.y() + 1, r.w() - 2, title_->h());
	r.move_y(title_->b() + 1);
	
	for (int i = 0; i < children(); i++) {
		Widget* widget = child(i);
		if (!widget->visible()) continue;
		widget->resize(r.x(), r.y(), r.w(), r.h());
		widget->layout_damage(widget->layout_damage() | damage);
		widget->layout();
	}
}

void ToolWindow::draw() {
	DockWindow::draw();
	title_->draw(this);
}

int ToolWindow::handle(int event) {
// 	if(event == PUSH) {
// // 		XSetWindowAttributes attr;
// // 		attr.override_redirect = 1;
// // 		int mask = CWOverrideRedirect;
// // 		XChangeWindowAttributes(fltk::xdisplay, fltk::xid(this->window()), mask, &attr);
// 		this->window()->destroy();
// 		this->window()->set_override();
// 		this->window()->show();
// 	}
// 	if(event == RELEASE) {
// // 		XSetWindowAttributes attr;
// // 		attr.override_redirect = 0;
// // 		int mask = CWOverrideRedirect;
// // 		if(fltk::xid(this->window()))
// // 		XChangeWindowAttributes(fltk::xdisplay, fltk::xid(this->window()), mask, &attr);
// 		this->window()->clear_flag(0x20000000);
// 		this->window()->destroy();
// 		this->window()->show();
// // 		this->take_focus();
// 	}
	if(title_->handle(event, this)) return 1;
	if(Global::highlight_title()) {
		switch(event) {
			case PUSH:
				take_focus();
				break;
			case FOCUS:
				title_->set_flag(Title::HIGHLIGHTED);
				redraw(*title_);
				return 2;
			case UNFOCUS:
				title_->clear_flag(Title::HIGHLIGHTED);
				redraw(*title_);
				return 1;
		}
	}
	return DockWindow::handle(event);
}

//
// End of $Id: ToolWindow.cxx,v 1.3 2006/06/23 21:36:56 z_hossain Exp $
//
