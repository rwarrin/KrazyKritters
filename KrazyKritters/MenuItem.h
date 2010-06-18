#ifndef MENUITEM_H
#define MENUITEM_H

#include <hge.h>
#include <hgegui.h>
#include <hgefont.h>
#include <hgecolor.h>

class hgeGUIMenuItem : public hgeGUIObject {
public:
	hgeGUIMenuItem(int _id, hgeFont * font, HEFFECT snd, float x, float y, char * title);

	virtual void Render();
	virtual void MouseOver(bool bOver);
	virtual bool MouseLButton(bool bDown);

private:
	hgeFont * font_;
	HEFFECT sound_;
	char * title_;

	hgeColor color_, shadow_;
	float offset_;
};

#endif  // MENUITEM_H
