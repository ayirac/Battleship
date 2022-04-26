#include "Table.h"

Table::Table() : max_rows_(1)
{

}

Table::Table(sf::Vector2f pos, sf::Vector2f size, unsigned row_spacing, sf::Font& font, int rows) : row_spacing_(row_spacing), font_(font), max_rows_(rows)
{
	this->outline_shape_.setSize(size);
	this->outline_shape_.setPosition(pos);
	this->outline_shape_.setOutlineThickness(3);
	this->outline_shape_.setFillColor(sf::Color(145, 109, 74));
	this->outline_shape_.setOutlineColor(sf::Color(115, 86, 59));
	// Setup Header/horizontal lines
	sf::RectangleShape header(sf::Vector2f(this->outline_shape_.getGlobalBounds().width, 3));
	sf::RectangleShape column1(sf::Vector2f(this->outline_shape_.getGlobalBounds().height, 3));
	sf::RectangleShape column2(sf::Vector2f(this->outline_shape_.getGlobalBounds().height, 3));
	sf::RectangleShape column3(sf::Vector2f(this->outline_shape_.getGlobalBounds().height, 3));
	sf::RectangleShape column4(sf::Vector2f(this->outline_shape_.getGlobalBounds().height, 3));
	header.setFillColor(sf::Color((245, 163, 53)));
	column1.setFillColor(sf::Color((245, 163, 53)));
	column2.setFillColor(sf::Color((245, 163, 53)));
	column3.setFillColor(sf::Color((245, 163, 53)));
	column4.setFillColor(sf::Color((245, 163, 53)));
	header.setOutlineColor(sf::Color::Black);
	column1.setOutlineColor(sf::Color::Black);
	column2.setOutlineColor(sf::Color::Black);
	column3.setOutlineColor(sf::Color::Black);
	column4.setOutlineColor(sf::Color::Black);

	// 24 - 19 - 19 - 19 - 19 : proportions
	float column1_x = this->outline_shape_.getGlobalBounds().left + this->outline_shape_.getGlobalBounds().width * 0.24,
		column2_x = column1_x + this->outline_shape_.getGlobalBounds().width * 0.19,
		column3_x = column2_x + this->outline_shape_.getGlobalBounds().width * 0.19,
		column4_x = column3_x + this->outline_shape_.getGlobalBounds().width * 0.19;

	header.setPosition(sf::Vector2f(this->outline_shape_.getGlobalBounds().left, this->outline_shape_.getGlobalBounds().top + row_spacing));
	column1.setPosition(sf::Vector2f(column1_x, this->outline_shape_.getGlobalBounds().top));
	column2.setPosition(sf::Vector2f(column2_x, this->outline_shape_.getGlobalBounds().top));
	column3.setPosition(sf::Vector2f(column3_x, this->outline_shape_.getGlobalBounds().top));
	column4.setPosition(sf::Vector2f(column4_x, this->outline_shape_.getGlobalBounds().top));
	column1.rotate(90);
	column2.rotate(90);
	column3.rotate(90);
	column4.rotate(90);

	this->lines_.push_back(header);
	this->lines_.push_back(column1);
	this->lines_.push_back(column2);
	this->lines_.push_back(column3);
	this->lines_.push_back(column4);
	for (unsigned i = 1; i < this->max_rows_; i++)
	{
		header.setPosition(this->outline_shape_.getGlobalBounds().left, this->outline_shape_.getGlobalBounds().top + this->row_spacing_ * i + this->row_spacing_);
		this->lines_.push_back(header);
	}

	// Setup header text
	sf::Text hits("Hits", font, 22);
	sf::Text misses("Misses", font, 22);
	sf::Text total("Total", font, 22);
	sf::Text hit_rate("Hit%", font, 22);
	sf::Text player("Player", font, 23);
	sf::Text enemy("Enemy", font, 23);
	hits.setFillColor(sf::Color(245, 163, 53));
	misses.setFillColor(sf::Color(245, 163, 53));
	total.setFillColor(sf::Color(245, 163, 53));
	hit_rate.setFillColor(sf::Color(245, 163, 53));
	player.setFillColor(sf::Color(245, 163, 53));
	enemy.setFillColor(sf::Color(245, 163, 53));
	hits.setOutlineThickness(1);
	misses.setOutlineThickness(1);
	total.setOutlineThickness(1);
	hit_rate.setOutlineThickness(1);
	player.setOutlineThickness(1);
	enemy.setOutlineThickness(1);
	hits.setOutlineColor(sf::Color::Black);
	misses.setOutlineColor(sf::Color::Black);
	total.setOutlineColor(sf::Color::Black);
	hit_rate.setOutlineColor(sf::Color::Black);
	player.setOutlineColor(sf::Color::Black);
	enemy.setOutlineColor(sf::Color::Black);
	hits.setOrigin(hits.getLocalBounds().width / 2, hits.getLocalBounds().height / 2);
	misses.setOrigin(misses.getLocalBounds().width / 2, misses.getLocalBounds().height / 2);
	total.setOrigin(total.getLocalBounds().width / 2, total.getLocalBounds().height / 2);
	hit_rate.setOrigin(hit_rate.getLocalBounds().width / 2, hit_rate.getLocalBounds().height / 2);
	player.setOrigin(player.getLocalBounds().width / 2, player.getLocalBounds().height / 2);
	enemy.setOrigin(enemy.getLocalBounds().width / 2, enemy.getLocalBounds().height / 2);

	hits.setPosition((column2_x - column1_x)/2 + column1_x - this->lines_[1].getGlobalBounds().width / 2,
		this->outline_shape_.getGlobalBounds().top + row_spacing / 2 - this->lines_[0].getGlobalBounds().height);

	misses.setPosition((column3_x - column2_x) / 2 + column2_x,
		this->outline_shape_.getGlobalBounds().top + row_spacing / 2 - this->lines_[0].getGlobalBounds().height);

	total.setPosition((column4_x - column3_x) / 2 + column3_x - this->lines_[1].getGlobalBounds().width / 2,
		this->outline_shape_.getGlobalBounds().top + row_spacing / 2 - this->lines_[0].getGlobalBounds().height);

	hit_rate.setPosition(((this->outline_shape_.getGlobalBounds().left + this->outline_shape_.getGlobalBounds().width) - column4_x) / 2 + column4_x - this->lines_[1].getGlobalBounds().width / 2,
		this->outline_shape_.getGlobalBounds().top + row_spacing / 2 - this->lines_[0].getGlobalBounds().height);

	player.setPosition((this->outline_shape_.getGlobalBounds().left + column1_x)/2 - this->lines_[1].getGlobalBounds().width / 2,
		this->outline_shape_.getGlobalBounds().top + 1*row_spacing + row_spacing / 2 - this->lines_[0].getGlobalBounds().height);

	enemy.setPosition((this->outline_shape_.getGlobalBounds().left + column1_x) / 2 - this->lines_[1].getGlobalBounds().width / 2,
		this->outline_shape_.getGlobalBounds().top + 2*row_spacing + row_spacing / 2 - this->lines_[0].getGlobalBounds().height);
	this->header_text_.push_back(hits);
	this->header_text_.push_back(misses);
	this->header_text_.push_back(total);
	this->header_text_.push_back(hit_rate);
	this->header_text_.push_back(player);
	this->header_text_.push_back(enemy);
	std::vector<sf::Text> text_vec;
	this->texts_.push_back(text_vec);
	this->texts_.push_back(text_vec);
	this->texts_.push_back(text_vec);
	this->texts_.push_back(text_vec);
}

Table::Table(sf::Vector2f pos, sf::Vector2f size, unsigned row_spacing, sf::Font& font,
	std::vector<sf::Texture*> textures, int rows, bool lazy) : row_spacing_(row_spacing), font_(font), textures_(textures), max_rows_(rows)
{
	this->outline_shape_.setSize(size);
	this->outline_shape_.setPosition(pos);
	this->outline_shape_.setOutlineThickness(3);
	this->outline_shape_.setFillColor(sf::Color(145, 109, 74));
	this->outline_shape_.setOutlineColor(sf::Color(115, 86, 59));
}

void Table::draw(sf::RenderWindow& win)
{
	win.draw(this->outline_shape_); // draw shape
	for (int i = 0; i < this->lines_.size(); i++) // draw lines
		win.draw(this->lines_[i]);
	for (int i = 0; i < this->header_text_.size(); i++) // draw header
		win.draw(this->header_text_[i]);
	for (int e = 0; e < this->texts_.size(); e++)
	{
		for (int i = 0; i < this->texts_[0].size(); i++) // draw text
			win.draw(this->texts_[e][i]);
	}
}

void Table::add_entry(HMT_stats& player_stats, HMT_stats& enemy_stats)
{
	float column1_x = this->outline_shape_.getGlobalBounds().left + this->outline_shape_.getGlobalBounds().width * 0.24,
		column2_x = column1_x + this->outline_shape_.getGlobalBounds().width * 0.19,
		column3_x = column2_x + this->outline_shape_.getGlobalBounds().width * 0.19,
		column4_x = column3_x + this->outline_shape_.getGlobalBounds().width * 0.19;
	sf::Text hits(std::to_string(player_stats.hits), this->font_, 22);
	sf::Text misses(std::to_string(player_stats.misses), this->font_, 22);
	sf::Text total(std::to_string(player_stats.total), this->font_, 22);
	std::string hit_rate_no_trailing = std::to_string(player_stats.hit_rate);
	hit_rate_no_trailing.erase(hit_rate_no_trailing.find("."));
	sf::Text hit_rate(hit_rate_no_trailing, this->font_, 22);
	hits.setFillColor(sf::Color(245, 163, 53));
	misses.setFillColor(sf::Color(245, 163, 53));
	total.setFillColor(sf::Color(245, 163, 53));
	hit_rate.setFillColor(sf::Color(245, 163, 53));
	hits.setOutlineThickness(1);
	misses.setOutlineThickness(1);
	total.setOutlineThickness(1);
	hit_rate.setOutlineThickness(1);
	hits.setOutlineColor(sf::Color::Black);
	misses.setOutlineColor(sf::Color::Black);
	total.setOutlineColor(sf::Color::Black);
	hit_rate.setOutlineColor(sf::Color::Black);
	hits.setOrigin(hits.getLocalBounds().width / 2, hits.getLocalBounds().height / 2);
	misses.setOrigin(misses.getLocalBounds().width / 2, misses.getLocalBounds().height / 2);
	total.setOrigin(total.getLocalBounds().width / 2, total.getLocalBounds().height / 2);
	hit_rate.setOrigin(hit_rate.getLocalBounds().width / 2, hit_rate.getLocalBounds().height / 2);

	// player
	hits.setPosition((column2_x - column1_x) / 2 + column1_x - this->lines_[1].getGlobalBounds().width / 2,
		this->outline_shape_.getGlobalBounds().top + 1 * this->row_spacing_ + this->row_spacing_ / 2 - this->lines_[0].getGlobalBounds().height);

	misses.setPosition((column3_x - column2_x) / 2 + column2_x,
		this->outline_shape_.getGlobalBounds().top + 1 * this->row_spacing_ + this->row_spacing_ / 2 - this->lines_[0].getGlobalBounds().height);

	total.setPosition((column4_x - column3_x) / 2 + column3_x - this->lines_[1].getGlobalBounds().width / 2,
		this->outline_shape_.getGlobalBounds().top + 1 * this->row_spacing_ + this->row_spacing_ / 2 - this->lines_[0].getGlobalBounds().height);

	hit_rate.setPosition(((this->outline_shape_.getGlobalBounds().left + this->outline_shape_.getGlobalBounds().width) - column4_x) / 2 + column4_x - this->lines_[1].getGlobalBounds().width / 2,
		this->outline_shape_.getGlobalBounds().top + 1 * this->row_spacing_ + this->row_spacing_ / 2 - this->lines_[0].getGlobalBounds().height);
	this->texts_[0].push_back(hits);
	this->texts_[1].push_back(misses);
	this->texts_[2].push_back(total);
	this->texts_[3].push_back(hit_rate);
	// enemy
	hits.setString(std::to_string(enemy_stats.hits));
	misses.setString(std::to_string(enemy_stats.misses));
	total.setString(std::to_string(enemy_stats.total));
	hit_rate_no_trailing = std::to_string(enemy_stats.hit_rate);
	hit_rate_no_trailing.erase(hit_rate_no_trailing.find("."));
	hit_rate.setString(hit_rate_no_trailing);
	hits.setPosition((column2_x - column1_x) / 2 + column1_x - this->lines_[1].getGlobalBounds().width / 2,
		this->outline_shape_.getGlobalBounds().top + 2 * this->row_spacing_ + this->row_spacing_ / 2 - this->lines_[0].getGlobalBounds().height);

	misses.setPosition((column3_x - column2_x) / 2 + column2_x,
		this->outline_shape_.getGlobalBounds().top + 2 * this->row_spacing_ + this->row_spacing_ / 2 - this->lines_[0].getGlobalBounds().height);

	total.setPosition((column4_x - column3_x) / 2 + column3_x - this->lines_[1].getGlobalBounds().width / 2,
		this->outline_shape_.getGlobalBounds().top + 2 * this->row_spacing_ + this->row_spacing_ / 2 - this->lines_[0].getGlobalBounds().height);

	hit_rate.setPosition(((this->outline_shape_.getGlobalBounds().left + this->outline_shape_.getGlobalBounds().width) - column4_x) / 2 + column4_x - this->lines_[1].getGlobalBounds().width / 2,
		this->outline_shape_.getGlobalBounds().top + 2 * this->row_spacing_ + this->row_spacing_ / 2 - this->lines_[0].getGlobalBounds().height);
	this->texts_[0].push_back(hits);
	this->texts_[1].push_back(misses);
	this->texts_[2].push_back(total);
	this->texts_[3].push_back(hit_rate);
}
