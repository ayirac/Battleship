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
	// Creates a ChatBox for two people given size, pos, a string, char_size, textures, font, a pointer to the multiplayer object, and two max variables.
	ChatBox(sf::Vector2f& size, sf::Vector2f& pos, std::string& instructions, unsigned char_size, std::vector<sf::Texture*> textures, sf::Font& font, Multiplayer* multiplayer, 
		unsigned max_textfield_entry, unsigned max_rows);
	~ChatBox();
	// Draws the ChatBox to a RenderWindow
	void draw(sf::RenderWindow& win) const;
	// Updates the ChatBox's animations and states given RenderWindow and mouse coordinates
	void update(sf::RenderWindow& win, sf::Vector2f& mouse_pos);
	// Releases the held button of the ChatBox
	void release_button();
	// Adds an entry to the ChatBox given a ChatMessage; which is made up of a name, message, and color.
	void add_entry(ChatMessage& chat_message);
	// Detects if there is any new messages inbound from the other player
	void detect_messages();
	// Shifts the chat messages downwards
	void shift_down();
	// Shifts the chat messages upwards
	void shift_up();
	// Resets chatbox
	void reset_table();
	// Called when the enter key is pressed, calls the add_entry method
	void enter_key_pressed();
};

#endif
