#include "TableScrollable.h"

TableScrollable::TableScrollable() : Table(), current_round_(1), current_row_(1), front_row_(-1), bottom_row_(-1), held_button_(nullptr) {}

TableScrollable::TableScrollable(sf::Vector2f pos, sf::Vector2f size, unsigned row_spacing, sf::Font& font,
	std::vector<sf::Texture*> textures, int rows) : Table(pos, size, row_spacing, font, textures, rows, true), current_round_(1), current_row_(1), front_row_(-1), bottom_row_(-1), held_button_(nullptr)
{
	// Setup Header/horizontal lines
	sf::RectangleShape header(sf::Vector2f(this->outline_shape_.getGlobalBounds().width, 3));
	sf::RectangleShape column1(sf::Vector2f(this->outline_shape_.getGlobalBounds().width, 3));
	sf::RectangleShape column2(sf::Vector2f(this->outline_shape_.getGlobalBounds().width, 3));
	header.setFillColor(sf::Color((245, 163, 53)));
	column1.setFillColor(sf::Color((245, 163, 53)));
	column2.setFillColor(sf::Color((245, 163, 53)));
	header.setOutlineColor(sf::Color::Black);
	column1.setOutlineColor(sf::Color::Black);
	column2.setOutlineColor(sf::Color::Black);

	// hardcorded 20 - 40 - 40 : proportions
	float column1_x = this->outline_shape_.getGlobalBounds().left + this->outline_shape_.getGlobalBounds().width * 0.2,
		column2_x = this->outline_shape_.getGlobalBounds().left + this->outline_shape_.getGlobalBounds().width * 0.6 + column1.getGlobalBounds().height / 2;

	header.setPosition(sf::Vector2f(this->outline_shape_.getGlobalBounds().left, this->outline_shape_.getGlobalBounds().top + row_spacing));
	column1.setPosition(sf::Vector2f(column1_x, this->outline_shape_.getGlobalBounds().top));
	column2.setPosition(sf::Vector2f(column2_x, this->outline_shape_.getGlobalBounds().top));
	column1.rotate(90);
	column2.rotate(90);

	this->lines_.push_back(header);
	this->lines_.push_back(column1);
	this->lines_.push_back(column2);
	for (unsigned i = 0; i < this->max_rows_; i++)
	{
		header.setPosition(this->outline_shape_.getGlobalBounds().left, this->outline_shape_.getGlobalBounds().top + this->row_spacing_ * i + this->row_spacing_);
		this->lines_.push_back(header);
	}


	// Setup header text & columns
	std::vector<sf::Text> text_vec;
	std::string header_titles[3] = { "Round", "Player Hits", "Enemy Hits" };
	float header_y_pos = this->outline_shape_.getGlobalBounds().top + row_spacing / 2 - this->lines_[0].getGlobalBounds().height;
	float header_x_pos[3] = { (column1_x - this->outline_shape_.getGlobalBounds().left) / 2 + this->outline_shape_.getGlobalBounds().left - this->lines_[1].getGlobalBounds().width / 2,
		(column2_x - column1_x) / 2 + column1_x - this->lines_[1].getGlobalBounds().width / 2,
		(this->outline_shape_.getGlobalBounds().left + this->outline_shape_.getGlobalBounds().width - column2_x) / 2 + column2_x - this->lines_[2].getGlobalBounds().width / 2 };
	for (int i = 0; i < 3; i++)
	{
		sf::Text header_text(header_titles[i], font, 22);
		header_text.setOrigin(header_text.getGlobalBounds().width / 2, header_text.getGlobalBounds().height / 2);
		header_text.setFillColor(sf::Color(245, 163, 53));
		header_text.setOutlineThickness(1);
		header_text.setOutlineColor(sf::Color::Black);
		header_text.setPosition(header_x_pos[i], header_y_pos);
		this->header_text_.push_back(header_text);
		this->texts_.push_back(text_vec);
	}

	// Setup buttons
	sf::Vector2f btn_size(50, 50);
	ButtonTexture button_scroll_up(sf::Vector2f((this->outline_shape_.getGlobalBounds().left + this->outline_shape_.getGlobalBounds().width ),
		this->outline_shape_.getGlobalBounds().top), btn_size, "Up", this->textures_[0], this->textures_[1], 0);
	ButtonTexture button_scroll_down(sf::Vector2f((this->outline_shape_.getGlobalBounds().left + this->outline_shape_.getGlobalBounds().width ),
		this->outline_shape_.getGlobalBounds().top + this->outline_shape_.getGlobalBounds().height - btn_size.y), btn_size, "Down", this->textures_[2], this->textures_[3], 0);
	this->buttons_.push_back(button_scroll_up);
	this->buttons_.push_back(button_scroll_down);

	// setup images
	ImageBox scroll_bar(this->textures_[4], sf::Vector2f(50, 400), sf::Vector2f(this->outline_shape_.getGlobalBounds().left + this->outline_shape_.getGlobalBounds().width, this->outline_shape_.getGlobalBounds().top));
	this->image_boxes_.push_back(scroll_bar);
}

void TableScrollable::draw(sf::RenderWindow& win, sf::Vector2i& mouse_pos)
{
	win.draw(this->outline_shape_); // draw shape
	for (int i = 0; i < this->lines_.size(); i++) // draw lines
		win.draw(this->lines_[i]);
	for (int i = 0; i < this->header_text_.size(); i++) // draw header
		win.draw(this->header_text_[i]);
	for (int e = 0; e < this->texts_.size(); e++)
	{
		for (unsigned i = this->front_row_; i < this->texts_[0].size() && i < this->max_rows_ + this->front_row_; i++) // draw text
			win.draw(this->texts_[e][i]);
	}
	for (int i = 0; i < this->image_boxes_.size(); i++) // draw images
		this->image_boxes_[i].draw(win);
	for (int i = 0; i < this->buttons_.size(); i++) // draw buttons
	{
		this->buttons_[i].draw(win);
		this->buttons_[i].update(mouse_pos, win);
	}
}

void TableScrollable::add_entry(sf::Vector2i player_hit, sf::Vector2i enemy_hit, bool& successful_player_attack, bool& successful_enemy_attack)
{
	Hit player = { player_hit, successful_player_attack }, enemy{ enemy_hit, successful_enemy_attack };
	this->player_hits_.push_back(player);
	this->enemy_hits_.push_back(enemy);

	char char_equivalent = 65 + player_hit.y;
	std::string temp;
	temp += char_equivalent;
	std::string player_coord = "(" + temp + ", " + std::to_string(player_hit.x + 1) + ")";
	std::string enemy_coord = "(" + temp + ", " + std::to_string(enemy_hit.x + 1) + ")";
	float column1_x = this->outline_shape_.getGlobalBounds().left + this->outline_shape_.getGlobalBounds().width * 0.2,
		column2_x = this->outline_shape_.getGlobalBounds().left + this->outline_shape_.getGlobalBounds().width * 0.6 + this->lines_[1].getGlobalBounds().width / 2;

	// add text
	sf::Text round(std::to_string(this->current_round_), this->font_, 24);
	sf::Text p_hits(player_coord, this->font_, 25);
	sf::Text e_hits(enemy_coord, this->font_, 25);
	round.setOrigin(round.getGlobalBounds().width / 2, round.getGlobalBounds().height / 2);
	p_hits.setOrigin(p_hits.getGlobalBounds().width / 2, p_hits.getGlobalBounds().height / 2);
	e_hits.setOrigin(e_hits.getGlobalBounds().width / 2, e_hits.getGlobalBounds().height / 2);
	round.setFillColor(sf::Color(245, 163, 53));
	if (successful_player_attack)
		p_hits.setFillColor(sf::Color::Red);
	else
		p_hits.setFillColor(sf::Color(209, 192, 175));
	if (successful_enemy_attack)
		e_hits.setFillColor(sf::Color::Red);
	else
		e_hits.setFillColor(sf::Color(209, 192, 175));

	if (this->texts_[0].empty()) {
		this->bottom_row_++;
		this->front_row_++;
	}

	if (1 + this->bottom_row_ <= this->texts_[0].size()) {  // doesn't allow movement if at bottom of table
		this->bottom_row_++;
	}
	while (this->bottom_row_ != this->texts_[0].size()) // move to bottom of table
	{
		shift_down();
		this->bottom_row_++;
	}

	// If the table is filled past its original capacity then shift the table downwards
  	if (this->texts_[0].size() >= this->max_rows_)
	{
		this->shift_down();
		round.setPosition((column1_x - this->outline_shape_.getGlobalBounds().left) / 2 + this->outline_shape_.getGlobalBounds().left - this->lines_[1].getGlobalBounds().width / 2,
			this->outline_shape_.getGlobalBounds().top + this->max_rows_ * this->row_spacing_ + this->row_spacing_ / 2 - this->lines_[0].getGlobalBounds().height);
		p_hits.setPosition((column2_x - column1_x) / 2 + column1_x - this->lines_[1].getGlobalBounds().width / 2,
			this->outline_shape_.getGlobalBounds().top + this->max_rows_ * this->row_spacing_ + this->row_spacing_ / 2 - this->lines_[0].getGlobalBounds().height);
		e_hits.setPosition(((this->outline_shape_.getGlobalBounds().left + this->outline_shape_.getGlobalBounds().width) - column2_x) / 2 + column2_x - this->lines_[2].getGlobalBounds().width / 2,
			this->outline_shape_.getGlobalBounds().top + this->max_rows_ * this->row_spacing_ + this->row_spacing_ / 2 - this->lines_[0].getGlobalBounds().height);
	}
	else // adds to the next open row
	{
		round.setPosition((column1_x - this->outline_shape_.getGlobalBounds().left) / 2 + this->outline_shape_.getGlobalBounds().left - this->lines_[1].getGlobalBounds().width / 2,
			this->outline_shape_.getGlobalBounds().top + this->current_row_ * this->row_spacing_ + this->row_spacing_ / 2 - this->lines_[0].getGlobalBounds().height);
		p_hits.setPosition((column2_x - column1_x) / 2 + column1_x - this->lines_[1].getGlobalBounds().width / 2,
			this->outline_shape_.getGlobalBounds().top + this->current_row_ * this->row_spacing_ + this->row_spacing_ / 2 - this->lines_[0].getGlobalBounds().height);
		e_hits.setPosition(((this->outline_shape_.getGlobalBounds().left + this->outline_shape_.getGlobalBounds().width) - column2_x) / 2 + column2_x - this->lines_[2].getGlobalBounds().width / 2,
			this->outline_shape_.getGlobalBounds().top + this->current_row_ * this->row_spacing_ + this->row_spacing_ / 2 - this->lines_[0].getGlobalBounds().height);
	}

	this->texts_[0].push_back(round);
	this->texts_[1].push_back(p_hits);
	this->texts_[2].push_back(e_hits);
	this->current_row_ = (this->current_round_ + 1) % (this->max_rows_ + 1);
	if (this->current_row_ == 0)
		this->current_row_++;
	this->current_round_++;
}

void TableScrollable::release_button()
{
	const std::string btn_text = this->held_button_->get_text();
	if (btn_text == "Up")
	{
		if (this->front_row_ > 0)
			shift_up();
	}
	else if (btn_text == "Down" && (this->enemy_hits_.size() > 9 && 1 + this->bottom_row_ != this->texts_[0].size()))
	{
		shift_down();
		this->bottom_row_++;
	}

	this->held_button_->reset();
	if (this->held_button_->get_state() != 3)
		this->held_button_->set_state(0);
	this->held_button_ = nullptr;
}

std::string TableScrollable::get_current_round()
{
	return std::to_string(this->current_round_);
}

void TableScrollable::process_click(sf::Vector2f& mouse_pos)
{

	// check if a tex_button was clocked on
	for (int i = 0; i < this->buttons_.size(); i++)
	{
		if (this->buttons_[i].contains(mouse_pos) && this->buttons_[i].get_state() != 3)
		{
			this->buttons_[i].set_state(2);
			this->held_button_ = &this->buttons_[i];
			break;
		}
	}
}

bool TableScrollable::holding_button()
{
	if (this->held_button_ == nullptr)
		return false;
	return true;
}

void TableScrollable::shift_down()
{
	for (unsigned i = this->front_row_ + this->max_rows_ - 1; i > 0; i--)
	{
		this->texts_[0][i].setPosition(this->texts_[0][i - 1].getPosition());
		this->texts_[1][i].setPosition(this->texts_[1][i - 1].getPosition());
		this->texts_[2][i].setPosition(this->texts_[2][i - 1].getPosition());
	}
	this->front_row_++;
}

void TableScrollable::shift_up()
{
	for (unsigned i = this->front_row_; i < this->max_rows_ + this->front_row_ - 1; i++)
	{
		this->texts_[0][i].setPosition(this->texts_[0][i + 1].getPosition());
		this->texts_[1][i].setPosition(this->texts_[1][i + 1].getPosition());
		this->texts_[2][i].setPosition(this->texts_[2][i + 1].getPosition());

	}
	this->front_row_--;
	this->bottom_row_--;
}

void TableScrollable::reset_table()
{
	this->player_hits_.clear();
	this->enemy_hits_.clear();
	for (int i = 0; i < this->texts_.size(); i++)
		this->texts_[i].clear();
	this->current_round_ = 1;
	this->current_row_ = 1;
	this->front_row_ = -1;
	this->bottom_row_ = -1;
}
