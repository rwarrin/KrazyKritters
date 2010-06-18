#include "MenuItem.h"

// This is a GUI Control constructor, we should initialize all the variables here
hgeGUIMenuItem::hgeGUIMenuItem(int _id, hgeFont * font, HEFFECT snd, float x, float y, char * title) {
	id = _id;
	font_ = font;
	title_ = title;
	sound_ = snd;

	color_.SetHWColor(0xFFFFFFF06);
	shadow_.SetHWColor(0x300000000);
	offset_ = 0.0f;

	bStatic = false;
	bVisible = true;
	bEnabled = true;

	float w = font_->GetStringWidth(title);
	rect.Set(x - w/2, y, x + w/2, y + font_->GetHeight());
}

void hgeGUIMenuItem::Render() {
	font_->SetColor(shadow_.GetHWColor());
	font_->Render(rect.x1 + offset_ + 3, rect.y1 + 3, HGETEXT_LEFT, title_);
	font_->SetColor(color_.GetHWColor());
	font_->Render(rect.x1 - offset_, rect.y1 - offset_, HGETEXT_LEFT, title_);
}

void hgeGUIMenuItem::MouseOver(bool bOver) {
	if(bOver)
		hge->Effect_Play(sound_);
}

bool hgeGUIMenuItem::MouseLButton(bool bDown) {
	if(!bDown) {
		offset_ = 4;
		return true;
	}
	return false;
}