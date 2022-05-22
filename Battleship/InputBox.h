#ifndef INPUTBOX_H
#define INPUTBOX_H
#include <SFML/Graphics/Text.hpp>
#include "Box.h"
#include "ButtonTexture.h"
#include "Multiplayer.h"
#include <thread>

class InputBox : public Box
{
protected:
	sf::RectangleShape textfield_shape_;			// states : 0 - not visible, 1 - visible, 2 - connecting
	sf::RectangleShape text_cursor_;
	std::thread* animate_cursor_thread_;
	sf::Text header_text_;
	sf::Text textfield_text_;
	sf::Font* font_;
	std::string textfield_entry_;
	std::vector<ButtonTexture> buttons_;			// 0 - enter button
	std::vector<sf::Texture*> textures_;
	bool edit_mode_;								// Represents if the user is actively editing the text field
	bool cursor_active_;							// Used for cursor animation invisibility state
	unsigned char_size_;
	Button* held_button_;
	Multiplayer* multiplayer_;
	sf::Sprite* loading_sprite_;
	sf::Color original_textfield_color_;
	std::string original_header_;
	const unsigned MAX_TEXTFIELD_ENTRY;
	bool showing_tip_;
	const char* ORIGINAL_TEXTFIELD_ENTRY_;
public:
	InputBox(sf::Vector2f size, sf::Vector2f pos, std::string& header, unsigned char_size, std::vector<sf::Texture*> textures, sf::Font& font, Multiplayer* multiplayer, unsigned max_textfield_entry);
	InputBox(sf::Vector2f size, sf::Vector2f pos, std::string& header, unsigned char_size, std::vector<sf::Texture*> textures, sf::Font& font, Multiplayer* multiplayer, sf::Sprite* loading_sprite, unsigned max_textfield_entry);
	~InputBox();
	void draw(sf::RenderWindow& win) const;
	// Updates the textfield animations
	virtual void update(sf::RenderWindow& win, sf::Vector2f& mouse_pos);
	// Returns true if the text field is being edited
	bool& edit_mode();
	void set_edit_mode(bool b);
	void process_keyboard(char key);
	// Checks if the textbox contains the mouse_pos
	bool contains(const sf::Vector2f& mouse_pos);
	void animate_text_cursor();
	void delete_end_textfield();
	unsigned get_textfield_size();
	virtual void release_button();
	void process_click(sf::Vector2f& mouse_pos);
	bool holding_button();
	std::string& get_textfield_entry();
	void set_textfield_entry(sf::String string);
	bool get_tip_status();
};

#endif
