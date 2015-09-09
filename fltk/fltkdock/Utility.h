//
// $Id: Utility.h,v 1.3 2006/06/23 21:33:16 z_hossain Exp $
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

#ifndef fltk_Utility_h
#define fltk_Utility_h

#define MAX_INT 2147483647
#define MIN_INT -2147483648

namespace fltk {
	inline int max(int a, int b) { return a > b ? a : b; }
	inline int min(int a, int b) { return a < b ? a : b; }
	inline int abs(int a) { return a < 0 ? -a : a; }
	inline double abs(double a) { return a < 0 ? -a : a; }
	inline int round(double a) { return int(a + 0.5); } //not needed exact round
	inline int square(int a) { return a * a; }
	inline int nonnegative(int a) { return a < 0 ? 0 : a; }
	inline int nonpositive(int a) { return a > 0 ? 0 : a; }
	inline void swap(int *a, int *b) { int t = *a; *a = *b; *b = t; }
}
#endif
//
// End of $Id: Utility.h,v 1.3 2006/06/23 21:33:16 z_hossain Exp $
//

