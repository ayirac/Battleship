#include <iostream>
#include "Game.h"

#include "Map.h"

// player_map_(10, 50, sf::Vector2f(static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y) * 1.5f))  enemy_map_(10, 50, sf::Vector2f(static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y) / 2)),
Game::Game(sf::RenderWindow* window) : window_(window), held_figurine_(nullptr), held_button_(nullptr), state_(0),
                                       ai_ship_found_(false), ai_reset_first_hit_(true), ai_orientation_reset_(false),
                                       ai_ship_found_orientation(0), side_checked_{false, false, false, false}
{ 
	// Setup pegs
	std::vector<ImageBox*> pegs;
	sf::Vector2f peg_size(30, 30);
	ImageBox* p_peg = new ImageBox(this->texturemanager_.get_texture(11), peg_size);
	pegs.push_back(p_peg);
	p_peg = new ImageBox(this->texturemanager_.get_texture(12), peg_size);
	pegs.push_back(p_peg);

	// Make font
	sf::Font itc_kabel; 
	if (!itc_kabel.loadFromFile(".\\Resources\\Fonts\\ITCKabelStd-Bold.otf"))
		std::cout << "Failed to load ITC_Kabel font\n";
	this->fonts_.push_back(itc_kabel);
	sf::Text victory_text("", this->fonts_[0], 60);
	victory_text.setOrigin(victory_text.getGlobalBounds().width / 2, victory_text.getGlobalBounds().height / 2);
	victory_text.setOutlineThickness(2);
	this->victory_text_ = victory_text;

	// Setup maps
	this->player_map_ = Map(10, 50, sf::Vector2f(static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y) * 1.5f), this->texturemanager_.get_texture(10), "Player", pegs, this->fonts_.at(0));
	this->enemy_map_ = Map(10, 50, sf::Vector2f(static_cast<float>(window->getSize().x), static_cast<float>(window->getSize().y) / 2), this->texturemanager_.get_texture(10), "Enemy", pegs, this->fonts_.at(0));

	// Setup statistics
	std::vector<sf::Texture*> statistic_textures;
	statistic_textures.push_back(this->texturemanager_.get_texture(15)); // up arrow
	statistic_textures.push_back(this->texturemanager_.get_texture(16)); // up arrow pressed
	statistic_textures.push_back(this->texturemanager_.get_texture(17)); // down arrow
	statistic_textures.push_back(this->texturemanager_.get_texture(18)); // down arrow pressed
	statistic_textures.push_back(this->texturemanager_.get_texture(19)); // scrollbar
	this->statistics_ = TableScrollable(sf::Vector2f(this->window_->getSize().x / 1.46, this->window_->getSize().y / 7.7), sf::Vector2f(400, 400), 40, this->fonts_[0], statistic_textures, 9);
	this->HMT_stats_ = Table(sf::Vector2f(this->window_->getSize().x / 38, this->window_->getSize().y / 7.7), sf::Vector2f(400, 120), 120 / 3, this->fonts_[0], 2);

	// Make background/title
	float window_x = static_cast<float>(this->window_->getSize().x), window_y = static_cast<float>(this->window_->getSize().y);
	ImageBox* p_imagebox;
	p_imagebox = new ImageBox(this->texturemanager_.get_texture(0), sf::Vector2f(window_x, window_y));
	this->image_boxes_.push_back(p_imagebox);
	p_imagebox = new ImageBox(this->texturemanager_.get_texture(1), sf::Vector2f(250, 50), sf::Vector2f((window_x - 250) / 2, (window_y - 50) / 20));
	this->image_boxes_.push_back(p_imagebox);
	p_imagebox = new ImageBox(this->texturemanager_.get_texture(2), sf::Vector2f(400, 100), sf::Vector2f((window_x - 400) / 2, (window_y - 100)));
	this->image_boxes_.push_back(p_imagebox);

	// Create Round, PLayer, Enemy ImageTextBox
	p_imagebox = new ImageTextBox(this->texturemanager_.get_texture(14), sf::Vector2f(300, 100), sf::Vector2f((window_x - 300) / 22, (window_y - 100) / 30), "Round " + this->get_statistics().get_current_round(), this->fonts_[0], 55, sf::Color(255, 255, 255), sf::Color::Black, sf::Vector2f(0, 0));
	this->image_boxes_.push_back(p_imagebox);
	p_imagebox = new ImageTextBox(this->texturemanager_.get_texture(20), sf::Vector2f(155, 65), sf::Vector2f((window_x - 155) / 1.345, (window_y - 65) / 19.5), "Enemy", this->fonts_[0], 33, sf::Color(255, 0, 0), sf::Color::Black, sf::Vector2f(0, 0));
	this->image_boxes_.push_back(p_imagebox);
	p_imagebox = new ImageTextBox(this->texturemanager_.get_texture(20), sf::Vector2f(155, 65), sf::Vector2f((window_x - 155) / 1.345, (window_y - 65) / 1.73), "Player", this->fonts_[0], 33, sf::Color(54, 148, 244), sf::Color::Black, sf::Vector2f(0, 0));
	this->image_boxes_.push_back(p_imagebox);

	
	// Make main menu buttons
	Button singleplayer_button(sf::Vector2f(window_x / 2, window_y / 1.4f), 27, "Singleplayer", this->fonts_.at(0), sf::Color(48, 42, 39), sf::Color(38, 35, 33), sf::Color(31, 29, 28), 0);
	Button multiplayer_button(sf::Vector2f(window_x / 2, window_y / 1.3f), 27, "Multiplayer", this->fonts_.at(0), sf::Color(48, 42, 39), sf::Color(38, 35, 33), sf::Color(31, 29, 28), 0);
	Button credits_button(sf::Vector2f(window_x / 2, window_y / 1.22f), 27, "Credits", this->fonts_.at(0), sf::Color(48, 42, 39), sf::Color(38, 35, 33), sf::Color(31, 29, 28), 0);
	this->buttons_.push_back(singleplayer_button);
	this->buttons_.push_back(multiplayer_button);
	this->buttons_.push_back(credits_button);

	// Make ship menu buttons
	sf::Vector2f btn_size(150, 50);
	ButtonTexture randomize_button(sf::Vector2f((window_x - btn_size.x) / 1.3f, (window_y - btn_size.y) / 1.05f), btn_size, 20, "Randomize", this->fonts_.at(0),
		this->texturemanager_.get_texture(3), this->texturemanager_.get_texture(4), sf::Color(245, 163, 53), sf::Color(251, 188, 38), sf::Color(213, 144, 19), 3);
	ButtonTexture exit_button(sf::Vector2f((window_x - btn_size.x) / 1.1f, (window_y - btn_size.y) / 1.05f), btn_size, 20, "Exit to Menu", this->fonts_.at(0),
		this->texturemanager_.get_texture(3), this->texturemanager_.get_texture(4), sf::Color(245, 163, 53), sf::Color(251, 188, 38), sf::Color(213, 144, 19), 3);
	ButtonTexture reset_button(sf::Vector2f((window_x - btn_size.x) / 1.3f, (window_y - btn_size.y) / 1.15f), btn_size, 20, "Reset Ships", this->fonts_.at(0),
		this->texturemanager_.get_texture(3), this->texturemanager_.get_texture(4), sf::Color(245, 163, 53), sf::Color(251, 188, 38), sf::Color(213, 144, 19), 3);
	ButtonTexture start_button(sf::Vector2f((window_x - btn_size.x) / 1.1f, (window_y - btn_size.y) / 1.15f), btn_size, 20, "Start Game", this->fonts_.at(0),
		this->texturemanager_.get_texture(3), this->texturemanager_.get_texture(4), sf::Color(245, 163, 53), sf::Color(251, 188, 38), sf::Color(213, 144, 19), 3);
	this->tex_buttons_.push_back(randomize_button);
	this->tex_buttons_.push_back(exit_button);
	this->tex_buttons_.push_back(reset_button);
	this->tex_buttons_.push_back(start_button);

	// Setup singleplayer battle buttons
	ButtonTexture attack(sf::Vector2f((window_x - btn_size.x) / 1.3f, (window_y - btn_size.y) / 1.05f), btn_size, 20, "Attack", this->fonts_.at(0),
		this->texturemanager_.get_texture(3), this->texturemanager_.get_texture(4), sf::Color(245, 163, 53), sf::Color(251, 188, 38), sf::Color(213, 144, 19), 3);
	ButtonTexture surrender(sf::Vector2f((window_x - btn_size.x) / 1.3f, (window_y - btn_size.y) / 1.5f), btn_size, 20, "Surrender", this->fonts_.at(0),
		this->texturemanager_.get_texture(3), this->texturemanager_.get_texture(4), sf::Color(245, 163, 53), sf::Color(251, 188, 38), sf::Color(213, 144, 19), 3);
	this->tex_buttons_.push_back(attack);
	this->tex_buttons_.push_back(surrender);

	///// Make multiplayer menu buttons /////
	ButtonTexture host_button(sf::Vector2f((window_x - btn_size.x) / 1.3f, (window_y - btn_size.y) / 1.15f), btn_size, 20, "Host Game", this->fonts_.at(0),
		this->texturemanager_.get_texture(3), this->texturemanager_.get_texture(4), sf::Color(245, 163, 53), sf::Color(251, 188, 38), sf::Color(213, 144, 19), 3);
	ButtonTexture join_button(sf::Vector2f((window_x - btn_size.x) / 1.1f, (window_y - btn_size.y) / 1.15f), btn_size, 20, "Join Game", this->fonts_.at(0),
		this->texturemanager_.get_texture(3), this->texturemanager_.get_texture(4), sf::Color(245, 163, 53), sf::Color(251, 188, 38), sf::Color(213, 144, 19), 3);
	this->tex_buttons_.push_back(host_button);
	this->tex_buttons_.push_back(join_button);

	// Inputbox
	sf::Vector2f input_size(this->window_->getSize().x/2, this->window_->getSize().y/8), input_pos(this->window_->getSize().x / 2 - input_size.x/2, this->window_->getSize().y / 5);
	std::string input_insturction("Enter an IP Address");
	std::vector<sf::Texture*> input_textures;
	input_textures.push_back(this->texturemanager_.get_texture(21));
	input_textures.push_back(this->texturemanager_.get_texture(22));
	this->IP_input_box_ = new InputBox(input_size, input_pos, input_insturction, 35, input_textures, this->fonts_[0], &this->multiplayer_);

	// Setup figurines
	sf::Vector2f figurine_pnts[5] = { sf::Vector2f(125, 1100), sf::Vector2f(350, 1100), sf::Vector2f(90, 1170), sf::Vector2f(250, 1170), sf::Vector2f(385, 1170) };
	sf::Vector2f figurine_scale[5] = { sf::Vector2f(1, 1.1f), sf::Vector2f(1, .9f), sf::Vector2f(1.1f, 1.1f), sf::Vector2f(1, 1), sf::Vector2f(1, .95f) };
	std::string figurine_names[5] = { "Carrier", "Battleship", "Cruiser", "Submarine", "Destroyer" };
	for (int i = 0; i < 5; i++)
	{
		std::vector<sf::Texture*> g = texturemanager_.get_ship_texture(i);
		Figurine ship_figurine(this->texturemanager_.get_texture(5+i), figurine_pnts[i], figurine_scale[i], false, figurine_names[i], texturemanager_.get_ship_texture(i));
		this->figurines_.push_back(ship_figurine);
	}
	this->figurine_peg_ = new Figurine(this->texturemanager_.get_texture(12), sf::Vector2f(1, 1), "Red Peg");
}

void Game::release_button()
{
	const std::string btn_text = this->held_button_->get_text();
	std::cout << btn_text << std::endl;
	if (btn_text == "Singleplayer")							// Main menu buttons
	{
		this->state_ = 1;
	}
	else if (btn_text == "Multiplayer")
	{
		this->state_ = 4;
	}
	else if (btn_text == "Credits")
	{
		//this->state_ = 1;
	}
	else if (btn_text == "Randomize")						// Ship menu buttons
	{
		this->get_player_map().remove_ships();
		this->randomize_ships(true);

		size_t const vec_size = this->figurines_.size();
		for (int i = 0; i < vec_size; i++)
			this->remove_figurine(this->figurines_[0].get_type());
	}
	else if (btn_text == "Exit to Menu")
	{
		std::cout << "Exiting to Menu.\n";
		this->reset_game();
		this->state_ = 0;
	}
	else if (btn_text == "Reset Ships")
	{
		this->get_player_map().remove_ships();
		this->add_discarded_figurines();
	}
	else if (btn_text == "Start Game")
	{
		// start game & randomize enemy ships if all players ships are added
		if (this->figurines_.empty())
		{
			this->state_ = 2;
			this->randomize_ships(false);
		}
		else {
			// add error msg, maybe popup box with custom class
		}
	}
	else if (btn_text == "Attack")							// Singleplayer battle buttons
	{
		this->ready_attack();
	}
	else if (btn_text == "Surrender")					
	{
		this->state_ = 3;
		this->HMT_stats_.add_entry(this->player_stats_, this->enemy_stats_);
		this->victory_text_.setString("Defeat");
		this->victory_text_.setPosition(this->get_player_map().get_left().x + this->get_player_map().get_size() * this->get_player_map().get_cell_size()/3.9,
			this->get_player_map().get_left().y -  2.8 * this->get_player_map().get_cell_size());
		this->victory_text_.setFillColor(sf::Color(244, 163, 53));
		this->victory_text_.setOutlineColor(sf::Color(55, 19, 19));
	}
	else if (btn_text == "Host Game")							// Multiplayer setup buttons
	{
		if (!this->multiplayer_.hosting())
			this->multiplayer_.host();
		else
			std::cout << "You're already hosting a server!\n";
	}
	else if (btn_text == "Join Game")							
	{
		this->IP_input_box_->set_state(1);
	}
	this->held_button_->reset();
	this->disable_buttons();
	this->held_button_ = nullptr;
}

void Game::main_menu()
{
	sf::Vector2f world_pos = this->window_->mapPixelToCoords(sf::Mouse::getPosition(*this->window_));
	// Draw elements
	this->image_boxes_.at(0)->draw(*this->window_);
	this->image_boxes_.at(1)->draw(*this->window_);
	this->image_boxes_.at(2)->draw(*this->window_);
	this->buttons_.at(0).draw(*this->window_);
	this->buttons_.at(1).draw(*this->window_);
	this->buttons_.at(2).draw(*this->window_);
	// Update buttons
	this->buttons_.at(0).update(world_pos, *this->window_);
	this->buttons_.at(1).update(world_pos, *this->window_);
	this->buttons_.at(2).update(world_pos, *this->window_);
}


void Game::ship_menu()
{
	// Get current mouse_pos to the window & convert to coordinates
	sf::Vector2f world_pos = this->window_->mapPixelToCoords(sf::Mouse::getPosition(*this->window_));
	std::string figurine_type;

	// Draw background and the maps
	this->image_boxes_.at(0)->draw(*this->window_);
	this->player_map_.draw(*this->window_);
	this->enemy_map_.draw(*this->window_);

	// Draw & update the buttons
	for (int i = 0 ; i < 4; i++)
	{
		this->tex_buttons_.at(i).draw(*this->window_);
		this->tex_buttons_.at(i).update(world_pos, *this->window_);
	}

	// Draw grids
	this->get_player_map().draw_grid_marks(*this->window_);
	this->get_enemy_map().draw_grid_marks(*this->window_);

	// Draw ImageTextBoxes
	this->image_boxes_.at(4)->draw(*this->window_);
	this->image_boxes_.at(5)->draw(*this->window_);

	// Draw figurines
	for (int i = 0; i < this->figurines_.size(); i++)
	{
		this->figurines_[i].draw(*this->window_);
		// Checks if a figurine is being hovered over
		this->figurines_[i].update(world_pos);
		if (this->holding_figurine())
			this->held_figurine_->drag(world_pos);
	}

}

void Game::singleplayer_game_start()
{
	// Get current mouse_pos to the window & convert to coordinates
	sf::Vector2f world_pos = this->window_->mapPixelToCoords(sf::Mouse::getPosition(*this->window_));

	// Draw background and the maps
	this->image_boxes_.at(0)->draw(*this->window_);
	this->player_map_.draw(*this->window_);
	this->enemy_map_.draw(*this->window_);

	// Draw ImageTextBox
	this->image_boxes_.at(3)->draw(*this->window_);
	this->image_boxes_.at(4)->draw(*this->window_);
	this->image_boxes_.at(5)->draw(*this->window_);

	// Draw buttons
	this->tex_buttons_.at(4).draw(*this->window_);
	this->tex_buttons_.at(4).update(world_pos, *this->window_);
	this->tex_buttons_.at(5).draw(*this->window_);
	this->tex_buttons_.at(5).update(world_pos, *this->window_);

	// Draw grids
	this->get_player_map().draw_grid_marks(*this->window_);
	this->get_enemy_map().draw_grid_marks(*this->window_);

	// Draw statistics
	this->statistics_.draw(*this->window_, world_pos);

	// Check if player is readying to attack
	if (this->holding_figurine())
	{
		this->held_figurine_->drag(world_pos);
		this->held_figurine_->draw(*this->window_);
	}

	// Check ship status
	unsigned i = 0;
	while (i < this->get_enemy_map().get_ships().size())
	{
		if (this->get_enemy_map().get_ships()[i]->get_hp() == 0)
		{
			if (!this->get_enemy_map().get_ships()[i]->get_ship_cells()[0]->visible())
			{
				for (int j = 0; j < this->get_enemy_map().get_ships()[i]->get_ship_cells().size(); j++) // Reveal the ship on the map after it has been sunk
					this->get_enemy_map().get_ships()[i]->get_ship_cells()[j]->set_texture(this->get_enemy_map().get_ships()[i]->get_ship_cells()[j]->get_texture(), this->get_enemy_map().get_ships()[i]->get_type(), true);
			}

			delete this->get_enemy_map().get_ships()[i];
			this->get_enemy_map().get_ships().erase(this->get_enemy_map().get_ships().begin() + i);
		}
		else
			i++;
	}
	i = 0;
	while (i < this->get_player_map().get_ships().size())
	{
		if (this->get_player_map().get_ships()[i]->get_hp() == 0) // ship has been sunk
		{
			// delete the ship's coordinates from ai_hits
			for (int j = 0; j < this->ai_hits.size(); j++)
			{
				if (this->player_map_.get_cells()[this->ai_hits[j].x][this->ai_hits[j].y].get_type() == this->get_player_map().get_ships()[i]->get_type())
				{
					this->ai_hits.erase(this->ai_hits.begin() + j);
					j--;
				}
			}
			
			this->ai_ship_found_ = false;
			if (!this->ai_hits.empty())
				this->ai_first_hit_ = this->ai_hits[0];
			this->ai_ship_found_orientation = 0;
			this->ai_orientation_reset_ = false;
			delete this->get_player_map().get_ships()[i];
			this->get_player_map().get_ships().erase(this->get_player_map().get_ships().begin() + i);
		}
		else
			i++;
	}

	// Check victory status
	if (this->get_enemy_map().get_ships().empty())
	{
		this->state_ = 3;
		this->HMT_stats_.add_entry(this->player_stats_, this->enemy_stats_);
		this->victory_text_.setString("Victory");
		this->victory_text_.setFillColor(sf::Color(244, 163, 53));
		this->victory_text_.setPosition(this->get_player_map().get_left().x + this->get_player_map().get_size() * this->get_player_map().get_cell_size() / 3.9,
			this->get_player_map().get_left().y - 2.8 * this->get_player_map().get_cell_size());
		this->victory_text_.setFillColor(sf::Color(255, 157, 25));
		this->victory_text_.setOutlineColor(sf::Color::Black);
	}
	else if (this->get_player_map().get_ships().empty())
	{
		this->state_ = 3;
		this->HMT_stats_.add_entry(this->player_stats_, this->enemy_stats_);
		this->victory_text_.setString("Defeat");
		this->victory_text_.setPosition(this->get_player_map().get_left().x + this->get_player_map().get_size() * this->get_player_map().get_cell_size() / 3.9,
			this->get_player_map().get_left().y - 2.8 * this->get_player_map().get_cell_size());
		this->victory_text_.setFillColor(sf::Color(244, 163, 53));
		this->victory_text_.setOutlineColor(sf::Color(55, 19, 19));
	}
		
}

void Game::post_game()
{
	// Get current mouse_pos to the window & convert to coordinates
	sf::Vector2f world_pos = this->window_->mapPixelToCoords(sf::Mouse::getPosition(*this->window_));

	// Draw background and the maps
	this->image_boxes_.at(0)->draw(*this->window_);
	this->player_map_.draw(*this->window_);
	this->enemy_map_.draw(*this->window_);

	// Draw ImageTextBox
	this->image_boxes_.at(3)->draw(*this->window_);
	this->image_boxes_.at(4)->draw(*this->window_);
	this->image_boxes_.at(5)->draw(*this->window_);

	// Draw statistics
	this->statistics_.draw(*this->window_, world_pos);
	this->HMT_stats_.draw(*this->window_);
	this->window_->draw(this->victory_text_);

	// Draw grids
	this->get_player_map().draw_grid_marks(*this->window_);
	this->get_enemy_map().draw_grid_marks(*this->window_);

	// set enemy ships to be visible
	for (int i = 0; i < this->get_enemy_map().get_ships().size(); i++)
	{
		if (!this->get_enemy_map().get_ships()[i]->get_ship_cells()[0]->visible())
		{
			for (int j = 0; j < this->get_enemy_map().get_ships()[i]->get_ship_cells().size(); j++) // Reveal the ship on the map after it has been sunk
				this->get_enemy_map().get_ships()[i]->get_ship_cells()[j]->set_texture(this->get_enemy_map().get_ships()[i]->get_ship_cells()[j]->get_texture(), this->get_enemy_map().get_ships()[i]->get_type(), true);
		}
	}

	// Draw buttons
	this->tex_buttons_[1].draw(*this->window_);
	this->tex_buttons_[1].update(world_pos, *this->window_);
}

void Game::multiplayer_setup()
{
	// Get current mouse_pos to the window & convert to coordinates
	sf::Vector2f world_pos = this->window_->mapPixelToCoords(sf::Mouse::getPosition(*this->window_));

	// Draw background and the maps
	this->image_boxes_.at(0)->draw(*this->window_);

	// Draw buttons
	this->tex_buttons_.at(6).draw(*this->window_);
	this->tex_buttons_.at(6).update(world_pos, *this->window_);
	this->tex_buttons_.at(7).draw(*this->window_);
	this->tex_buttons_.at(7).update(world_pos, *this->window_);

	if (this->IP_input_box_->get_state() == 1)
	{
		this->IP_input_box_->update();
		this->IP_input_box_->draw(*this->window_, world_pos);
	}
		
}

void Game::multiplayer_game_start()
{
}

Map& Game::get_player_map()
{
	return this->player_map_;
}

Map& Game::get_enemy_map()
{
	return this->enemy_map_;
}

const unsigned& Game::get_state() const
{
	return this->state_;
}

void Game::randomize_ships(bool players_ships)
{
	std::string ship_names[5] = { "Carrier", "Battleship", "Cruiser", "Submarine", "Destroyer" };
	const int num_ships = 5;
	for (int i = 0; i < num_ships; i++)
	{
		// Get orientation/position
		unsigned x, y, rotation, mod = 5;
		const unsigned rand_rot = rand() % 4;
		bool ship_found = false;
		// Generates new random values for x/y/rotation until an acceptable spot has been detected for the ship
		do {
			if (ship_names[i] == "Carrier")
				mod = 6;
			else if (ship_names[i] == "Battleship")
				mod = 7;
			else if (ship_names[i] == "Cruiser" || ship_names[i] == "Submarine")
				mod = 8;
			else if (ship_names[i] == "Destroyer")
				mod = 9;

			if (rand_rot == 0)	// horizontal
			{
				x = rand() % mod;
				y = rand() % 10;
				rotation = 90;
			}
			else if (rand_rot == 1) // horizontal
			{
				x = rand() % mod;
				y = rand() % 10;
				rotation = 270;
			}
			else if (rand_rot == 2) // vertical
			{
				x = rand() % 10;
				y = rand() % mod;
				rotation = 0;
			}
			else if (rand_rot == 3) // vertical
			{
				x = rand() % 10;
				y = rand() % mod;
				rotation = 180;
			}

			if (players_ships)
				ship_found = this->get_player_map().check_overlap(x, y, rotation, ship_names[i]);
			else
				ship_found = this->get_enemy_map().check_overlap(x, y, rotation, ship_names[i]);
		} while (ship_found == true);

		// Fix for overflow/proper placement
		if (rotation == 180)
			y--;
		else if (rotation == 90)
			x--;

		if (players_ships)
			this->player_map_.add_ship(x, y, rotation, ship_names[i], this->texturemanager_.get_ship_texture(ship_names[i]), players_ships);
		else
		{
			this->enemy_map_.add_ship(x, y, rotation, ship_names[i], this->texturemanager_.get_ship_texture(ship_names[i]), false); // debug, set to true to see enemy ships
		}

	}
}

void Game::remove_figurine(std::string& type)
{
	for (std::vector<Figurine>::iterator itr = this->figurines_.begin(); itr != this->figurines_.end(); itr++)
	{
		if (itr->get_type() == type)
		{
			itr->reset_position();
			this->discarded_figurines_.push_back(*itr);
			this->figurines_.erase(itr);
			break;
		}
			
	}
}

void Game::process_click(const sf::Vector2f& mouse_pos)
{
	// check if figurine was clicked on
	for (int i = 0; i < this->figurines_.size(); i++)
	{
		if (this->figurines_[i].contains(mouse_pos))
		{
			this->held_figurine_ = &this->figurines_[i];
			break;
		}
	}

	// check if a button was clicked on
	for (int i = 0; i < this->buttons_.size(); i++)
	{
		if (this->buttons_[i].contains(mouse_pos) && this->buttons_[i].get_state() != 3)
		{
			this->buttons_[i].set_state(2);
			this->held_button_ = &this->buttons_[i];
			break;
		}
	}

	// check if a tex_button was clicked on
	for (int i = 0; i < this->tex_buttons_.size(); i++)
	{
		if (this->tex_buttons_[i].contains(mouse_pos) && this->tex_buttons_[i].get_state() != 3) // excludes buttons in state 3 (disabled)
		{
			this->tex_buttons_[i].set_state(2);
			this->held_button_ = &this->tex_buttons_[i];
			break;
		}
	}

	// Check if an input textbox was clicked on
	if (this->IP_input_box_->contains(mouse_pos) && this->IP_input_box_->get_state() == 1)
		this->IP_input_box_->set_edit_mode(true);


}

void Game::release_figurine(const sf::Vector2f& mouse_pos)
{
	int const rotation = static_cast<int>(this->held_figurine_->get_abs_angle());
	if (this->held_figurine_->release(this->get_player_map(), mouse_pos, rotation))
	{
		this->remove_figurine(this->held_figurine_->get_type());
	}
	this->held_figurine_ = nullptr;
}

void Game::release_figurine()
{
	this->held_figurine_ = nullptr;
}

void Game::reset_game()
{
	this->get_player_map().remove_ships();
	this->get_enemy_map().remove_ships();
	this->add_discarded_figurines();
	this->get_statistics().reset_table();
	this->get_hmt().reset_table();
	this->get_player_map().reset_map();
	this->get_enemy_map().reset_map();
	this->reset_hmt_stats();
}


void Game::rotate_held_figurine(const sf::Keyboard::Key& key) const
{
	if (this->held_figurine_ != nullptr)
	{
		if (key == sf::Keyboard::A)
			held_figurine_->rotate(-90);
		else if (key == sf::Keyboard::D)
			held_figurine_->rotate(90);
	}
}

bool Game::holding_figurine() const
{
	if (this->held_figurine_ == nullptr)
		return false;
	if (this->held_figurine_->get_type() == "Empty")
		return false;
	return true;
}

bool Game::holding_button() const
{
	if (this->held_button_ == nullptr)
		return false;
	return true;
}

void Game::add_discarded_figurines()
{
	for (int i = static_cast<int>(this->discarded_figurines_.size()) - 1; i >= 0; i--)
	{
		this->figurines_.push_back(this->discarded_figurines_[i]);
		this->discarded_figurines_.pop_back();
	}
}

void Game::ready_attack()
{
	this->held_figurine_ = this->figurine_peg_;
}

bool Game::process_hit(sf::Vector2f& mouse_pos, bool player, sf::Vector2i& attack_pos, bool& hit)
{
 	std::string target_hit;
	if (player)
		target_hit = this->get_player_map().check_hit(mouse_pos, attack_pos);
	else // not used, maybe with IP multiplayer
		target_hit = this->get_enemy_map().check_hit(mouse_pos, attack_pos);
	std::cout << "Player has hit " << target_hit << std::endl;
	if (target_hit == "Outside" || target_hit == "Filled")
		return false;
	if (target_hit != "Empty")
	{
		hit = true;
		this->player_stats_.hits++;
	}
	else if (target_hit == "Empty")
		this->player_stats_.misses++;
	this->player_stats_.total++;
 	this->player_stats_.hit_rate = (static_cast<double>(this->player_stats_.hits) / this->player_stats_.total) * 100;
	this->image_boxes_.at(3)->set_text("Round " + this->get_statistics().get_current_round());
	//std::cout << this->enemy_map_ << std::endl; debug
	return true;
}

void Game::process_ai_hit(sf::Vector2i& attack_pos, bool& hit)
{
	if (!this->ai_hits.empty() && !this->ai_ship_found_) // difficulty easy
	{
		this->ai_last_hit_ = this->ai_hits.at(0);
		this->ai_hits.erase(this->ai_hits.begin());
		this->ai_ship_found_ = true;
		ai_sink_ship(attack_pos, hit);
	}
	else if (this->ai_ship_found_)
	{
		ai_sink_ship(attack_pos, hit);
	}
	else
		random_ai_attack(attack_pos, hit);
}

void Game::random_ai_attack(sf::Vector2i& attack_pos, bool& hit)
{
	std::string struck_cell;
	sf::Vector2i rand_pos;
	unsigned rand_x, rand_y;
	// Look for a cell to attack that isn't filled
	do
	{
		rand_x = rand() % 10, rand_y = rand() % 10;
		rand_pos = sf::Vector2i(rand_x, rand_y);
		struck_cell = this->get_player_map().check_hit(rand_pos);
	}
	while (struck_cell == "Filled");

	if (struck_cell != "Empty")
	{
		this->ai_ship_found_ = true;
		this->ai_last_hit_ = rand_pos;
		this->ai_first_hit_ = rand_pos;
		hit = true;
		this->enemy_stats_.hits++;
	}
	else if (struck_cell == "Empty")
		this->enemy_stats_.misses++;
	this->enemy_stats_.total++;
	this->enemy_stats_.hit_rate = (static_cast<double>(this->enemy_stats_.hits) / this->enemy_stats_.total) * 100;
	attack_pos.x = rand_x, attack_pos.y = rand_y;
	std::cout << "AI has randomly attacked (" << rand_x << ',' << rand_y << ") which scored a hit against " << struck_cell << std::endl;
}

void Game::ai_sink_ship(sf::Vector2i& attack_pos, bool& hit) // works (?)
{
	int next_cell;
	bool valid_cell;
	std::string struck_cell;
	sf::Vector2i pos_temp;
	// Randomly strikes one of the valid cells near the given pos.
	do
	{
		pos_temp = this->ai_last_hit_;
		valid_cell = false;
		if (this->ai_ship_found_orientation == 1)
			next_cell = rand() % 2;
		else if (this->ai_ship_found_orientation == 2)
			next_cell = rand() % 2 + 2;
		else
			next_cell = rand() % 4;

		// Invalidates the sides that would result in out of bound errors
		if (pos_temp.x-1 < 0)
			this->side_checked_[0] = true;
		if (pos_temp.x + 1 > 9)
			this->side_checked_[1] = true;
		if (pos_temp.y-1 < 0)
			this->side_checked_[2] = true;
		if (pos_temp.y+1 > 9)
			this->side_checked_[3] = true;

		// TODO - add logic for limiting based on the ships currently alive.. dont need to go left if only 2 spaces are left and only 3+ tile ships are left
		if (next_cell == 0 && this->side_checked_[0] == false) // left
		{
			pos_temp.x--;
			valid_cell = true;
			this->side_checked_[0] = true;
		}
		else if (next_cell == 1 && this->side_checked_[1] == false) // right
		{
			pos_temp.x++;
			valid_cell = true;
			this->side_checked_[1] = true;
		}
		else if (next_cell == 2 && this->side_checked_[2] == false) // top
		{
			pos_temp.y--;
			valid_cell = true;
			this->side_checked_[2] = true;
		}
		else if (next_cell == 3 && this->side_checked_[3] == false) //bottom
		{
			pos_temp.y++;
			valid_cell = true;
			this->side_checked_[3] = true;
		}
		if (valid_cell)
			struck_cell = this->get_player_map().check_hit(pos_temp);
		else
		{
			// Set the last_hit to the first_hit if all the sides have been checked
			if (this->ai_ship_found_orientation == 0)
			{
				if (this->side_checked_[0] && this->side_checked_[1] && this->side_checked_[2] && this->side_checked_[3])
				{
					this->ai_last_hit_ = this->ai_first_hit_;
					this->side_checked_[0] = false; this->side_checked_[1] = false; this->side_checked_[2] = false; this->side_checked_[3] = false;
					if (this->ai_reset_first_hit_)
					{
						this->ai_ship_found_ = false;
						this->ai_reset_first_hit_ = false;
					}
					else
						this->ai_reset_first_hit_ = true;
				}

			}
			else if (this->ai_ship_found_orientation == 1) // cut these two else if's down into a function maybe, only think that changes is first condition rlly
			{
				if (this->side_checked_[0] && this->side_checked_[1])
				{
					this->side_checked_[0] = false; this->side_checked_[1] = false;
					this->ai_last_hit_ = this->ai_first_hit_;


					if (ai_orientation_reset_ == 2 && !this->ai_hits.empty())
					{
						this->ai_last_hit_ = this->ai_hits[0];
						this->ai_hits.erase(this->ai_hits.begin());
					}
					else if (ai_orientation_reset_ == 2 && this->ai_hits.empty())
					{
						this->ai_last_hit_ = this->player_map_.get_first_open_cell();
					}
					else if (this->ai_reset_first_hit_)
					{
						if (this->ai_orientation_reset_ == 0)
							this->ai_orientation_reset_ = 1;
						else if (this->ai_orientation_reset_ == 1)
							this->ai_orientation_reset_ = 2;

						this->ai_ship_found_orientation = 2;
						this->ai_reset_first_hit_ = false;
					}
					else
						this->ai_reset_first_hit_ = true;
				}
				// ai_orientation_reset_ - 0 no reset, 1 reset once, 2 reset twice
			}
			else if (this->ai_ship_found_orientation == 2)
			{
				if (this->side_checked_[2] && this->side_checked_[3])
				{
					this->side_checked_[2] = false; this->side_checked_[3] = false;
					this->ai_last_hit_ = this->ai_first_hit_;

					if (ai_orientation_reset_ == 2 && !this->ai_hits.empty()) // 3a grab a hit cell from the hits vector
					{
						this->ai_last_hit_ = this->ai_hits[0];
						this->ai_hits.erase(this->ai_hits.begin());
					}
					else if (ai_orientation_reset_ == 2 && this->ai_hits.empty()) // 3b randomly get a new cell
					{
						this->ai_last_hit_ = this->player_map_.get_first_open_cell();
					}
					else if (this->ai_reset_first_hit_) // 2a try different orientation
					{
						if (this->ai_orientation_reset_ == 0)
							this->ai_orientation_reset_ = 1;
						else if (this->ai_orientation_reset_ == 1)
							this->ai_orientation_reset_ = 2;

						this->ai_ship_found_orientation = 1;
						this->ai_reset_first_hit_ = false;
					}
					else // 1a reset to original pos
						this->ai_reset_first_hit_ = true;
				}
			}
		}
	}
 while (!valid_cell || struck_cell == "Filled");

	if (struck_cell != "Empty")
	{
		this->ai_hits.push_back(pos_temp);
		this->ai_last_hit_ = pos_temp;
		this->ai_ship_found_ = true;
		this->side_checked_[0] = false; this->side_checked_[1] = false; this->side_checked_[2] = false; this->side_checked_[3] = false;

		// sets the orientation if a second cell is hit that is the same axis as the previous
		if (this->ai_ship_found_orientation == 0)
		{
			if (next_cell == 0 || next_cell == 1) // horizontal
				this->ai_ship_found_orientation = 1;
			else if (next_cell == 2 || next_cell == 3) // vertical
				this->ai_ship_found_orientation = 2;
		}
		this->ai_reset_first_hit_ = false;

		hit = true;
		this->enemy_stats_.hits++;
	}
	else if (struck_cell == "Empty")
		this->enemy_stats_.misses++;
	this->enemy_stats_.total++;
	this->player_stats_.hit_rate = (static_cast<double>(this->player_stats_.hits) / this->player_stats_.total) * 100;
	attack_pos = pos_temp;
	std::cout << "AI has attacked (" << pos_temp.x << ',' << pos_temp.y << ") which scored a hit against " << struck_cell << std::endl;
	std::cout << "ai_ship_found_orientation " << this->ai_ship_found_orientation << std::endl;
}

TableScrollable& Game::get_statistics()
{
	return this->statistics_;
}

Table& Game::get_hmt()
{
	return this->HMT_stats_;
}

void Game::reset_hmt_stats()
{
	this->player_stats_.hits = 0;
	this->player_stats_.misses = 0;
	this->player_stats_.total = 0;
	this->player_stats_.hit_rate = 0.0;
	this->enemy_stats_.hits = 0;
	this->enemy_stats_.misses = 0;
	this->enemy_stats_.total = 0;
	this->enemy_stats_.hit_rate = 0.0;
}

void Game::disable_buttons()
{
	if (this->state_ == 0)
	{
		for (int i = 0; i < this->tex_buttons_.size(); i++)				// Disable game buttons excluding menu
			this->tex_buttons_[i].set_state(3);
	}
	else if (this->state_ == 1)
	{
		for (int i = 0; i < 3; i++) {									// Disable menu buttons
			this->buttons_[i].set_state(3);
		}
		for (int i = 0; i < 4; i++)										// Enable ship select menu buttons 4
			this->tex_buttons_[i].set_state(0);
	}
	else if (this->state_ == 2)
	{
		for (int i = 0; i < 3; i++)										// Disable menu buttons
			this->buttons_[i].set_state(3);
		for (int i = 0; i < 4; i++)										// Disable ship select menu buttons 4
			this->tex_buttons_[i].set_state(3);
		for (int i = 4; i < 6; i++)										// Enable SP buttons 2
			this->tex_buttons_[i].set_state(0);
	}
	else if (this->state_ == 3)
	{
		for (int i = 4; i < 6; i++)										// disable SP buttons 2
			this->tex_buttons_[i].set_state(3);
		this->tex_buttons_[4].set_state(0);								// Enable post-game buttons 1
			
	}
	else if (this->state_ == 4)
	{
		for (int i = 0; i < 3; i++) {									// Disable menu buttons
			this->buttons_[i].set_state(3);
		}
		for (int i = 6; i < 8; i++)										// Enable multiplayer setup buttons
			this->tex_buttons_[i].set_state(0);
	}
	for (int i = 0; i < this->buttons_.size(); i++)
		this->buttons_[i].set_state(3);
}

InputBox* Game::get_inputbox()
{
	return this->IP_input_box_;
}

Multiplayer& Game::get_multiplayer()
{
	return this->multiplayer_;
}

