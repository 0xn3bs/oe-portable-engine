#include "Widget.h"

bool OE::UI::Widgets::Widget::IsOver(const float x, const float y)
{
	int iXPos, iYPos;

	iXPos = _winParentWindow->GetXPos();
	iYPos = _winParentWindow->GetYPos();

	bool isX = (x > iXPos + _v2fPosition.x)
		&& (x < iXPos + _v2fPosition.x + _v2fDimensions.x);
	bool isY = (y > iYPos + _v2fPosition.y)
		&& (y < iYPos + _v2fPosition.y + _v2fDimensions.y);

	return isX && isY;
}

void OE::UI::Widgets::Widget::GetLocalPosition(int *x, int *y)
{
	double mX, mY;
	mX = mY = 0;

	OE::Input::InputManager::GetMousePos(&mX, &mY);
	*x = mX - _v2fPosition.x - _winParentWindow->GetXPos();
	*y = mY - _v2fPosition.y - _winParentWindow->GetYPos();
}