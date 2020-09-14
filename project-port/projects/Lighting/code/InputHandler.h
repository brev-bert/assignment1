#pragma once

namespace Input {
	struct {
		bool W, A, S, D;

		bool Up, Left, Down, Right;

		bool MouseLeft, MouseRight;
	} Keys;
	
	struct {
		double xpos, ypos;
		double oldx, oldy;
		double dx, dy;
	} Mouse;
}
