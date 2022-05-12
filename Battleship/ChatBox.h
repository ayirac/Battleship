#ifndef CHATBOX_H
#define CHATBOX_H
#include "InputBox.h"
#include "Button.h"
#include "ImageBox.h"

class ChatBox : public InputBox
{
private:
	int front_row_;									// buttons: 0 - enter, 1 - scroll up, 2 - scroll down
	int bottom_row_;								// states: 0 - not visible, 1 - visible
	int current_row_;
	unsigned row_spacing_;
	const unsigned MAX_ROWS_;
	const float CHAT_MESSAGES_BASE_Y_;
	std::vector<ChatMessage> chat_messages_;
	std::vector<sf::Text> message_texts_;
	std::vector<sf::Text> name_texts_;
	std::vector<sf::IntRect> chat_message_containers_;
	std::thread* detect_messages_thread_;
	ImageBox* scroll_bar_image_;
public:
	ChatBox(sf::Vector2f& size, sf::Vector2f& pos, std::string& instructions, unsigned char_size, std::vector<sf::Texture*> textures, sf::Font& font, Multiplayer* multiplayer, 
		unsigned max_textfield_entry, unsigned max_rows);
	~ChatBox();
	void draw(sf::RenderWindow& win) const;
	void update(sf::RenderWindow& win, sf::Vector2f& mouse_pos);
	void release_button();
	void add_entry(ChatMessage& chat_message);
	void detect_messages();
	void shift_down();
	void shift_up();
	void reset_table();
	void enter_key_pressed();
};

#endif
