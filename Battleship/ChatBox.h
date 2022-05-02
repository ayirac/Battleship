#ifndef CHATBOX_H
#define CHATBOX_H
#include "InputBox.h"

class ChatBox : public InputBox
{
public:
	void draw(sf::RenderWindow& win) const;
	void set_text(std::string text);
};

#endif
