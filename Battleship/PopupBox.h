#ifndef POPUPBOX_H
#define POPUPBOX_H
#include "Box.h"

class PopupBox : public Box
{
public:
	void draw(sf::RenderWindow& win) const;
	void set_text(std::string text);
};

#endif


/*
 *
 * idea
 * constructor could be PopupBox(some_rect_size, some_char_size, "My first row!!!", "Guess what, my second row", "Lastly my third row..");
 * could be good lazy way to setup text with more control from dev, removing a param/adding one will change the size of the box..
 *
 *
 * otherwise, need to take a string and create a logical algorithm that adds the string to a rectangle and does the resizing/accounting for text cutoff automatically... sounds kinda messy (?)
 */