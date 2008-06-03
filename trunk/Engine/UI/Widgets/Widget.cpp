#include "Widget.h"

bool Odorless::Engine::UI::Widgets::Widget::IsOver(const float &x, const float &y)
{
	int iXPos, iYPos;

	iXPos = _winParentWindow->GetXPos();
	iYPos = _winParentWindow->GetYPos();

	bool isX = (x > iXPos + _2fPosition[0])
		&& (x < iXPos + _2fPosition[0] + _2fDimensions[0]);
	bool isY = (y > iYPos + _2fPosition[1])
		&& (y < iYPos + _2fPosition[1] + _2fDimensions[1]);

	return isX && isY;
}