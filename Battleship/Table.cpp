#include "Table.h"

Table::Table() : max_rows_(1), row_spacing_(0)
{

}

Table::Table(sf::Vector2f pos, sf::Vector2f size, unsigned row_spacing, sf::Font& font, int rows) : row_spacing_(row_spacing), font_(font), max_rows_(rows), texts_(4)
{
	this->outline_shape_.setSize(size);
	this->outline_shape_.setPosition(pos);
	this->outline_shape_.setOutlineThickness(3);
	this->outline_shape_.setFillColor(sf::Color(145, 109, 74));
	this->outline_shape_.setOutlineColor(sf::Color(115, 86, 59));
	// Setup Header/horizontal lines
	sf::RectangleShape header(sf::Vector2f(this->outline_shape_.getGlobalBounds().width, 3));
	sf::RectangleShape column(sf::Vector2f(this->outline_shape_.getGlobalBounds().height, 3));
	header.setFillColor(sf::Color((245, 163, 53)));
	column.setFillColor(sf::Color((245, 163, 53)));
	header.setOutlineColor(sf::Color::Black);
	column.setOutlineColor(sf::Color::Black);
	column.rotate(90);
	// 24 - 19 - 19 - 19 - 19 : proportions
	float column1_x = this->outline_shape_.getGlobalBounds().left + this->outline_shape_.getGlobalBounds().width * 0.24,
		column2_x = column1_x + this->outline_shape_.getGlobalBounds().width * 0.19,
		column3_x = column2_x + this->outline_shape_.getGlobalBounds().width * 0.19,
		column4_x = column3_x + this->outline_shape_.getGlobalBounds().width * 0.19;

	header.setPosition(sf::Vector2f(this->outline_shape_.getGlobalBounds().left, this->outline_shape_.getGlobalBounds().top + row_spacing));
	this->lines_.push_back(header);
	column.setPosition(sf::Vector2f(column1_x, this->outline_shape_.getGlobalBounds().top));
	this->lines_.push_back(column);
	column.setPosition(sf::Vector2f(column2_x, this->outline_shape_.getGlobalBounds().top));
	this->lines_.push_back(column);
	column.setPosition(sf::Vector2f(column3_x, this->outline_shape_.getGlobalBounds().top));
	this->lines_.push_back(column);
	column.setPosition(sf::Vector2f(column4_x, this->outline_shape_.getGlobalBounds().top));
	this->lines_.push_back(column);
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
	this->header_text_.push_back(hits);
	this->header_text_.push_back(misses);
	this->header_text_.push_back(total);
	this->header_text_.push_back(hit_rate);
	this->header_text_.push_back(player);
	this->header_text_.push_back(enemy);

	for (int i = 0; i < this->header_text_.size(); i++)
	{
		this->header_text_[i].setFillColor(sf::Color(245, 163, 53));
		this->header_text_[i].setOutlineThickness(1);
		this->header_text_[i].setOutlineColor(sf::Color::Black);
		this->header_text_[i].setOrigin(this->header_text_[i].getLocalBounds().width / 2, this->header_text_[i].getLocalBounds().height / 2);
	}

	this->header_text_[0].setPosition((column2_x - column1_x) / 2 + column1_x - this->lines_[1].getGlobalBounds().width / 2,
		this->outline_shape_.getGlobalBounds().top + row_spacing / 2 - this->lines_[0].getGlobalBounds().height);
	this->header_text_[1].setPosition(( column3_x - column2_x) / 2 + column2_x,
		this->outline_shape_.getGlobalBounds().top + row_spacing / 2 - this->lines_[0].getGlobalBounds().height);
	this->header_text_[2].setPosition((column4_x - column3_x) / 2 + column3_x - this->lines_[1].getGlobalBounds().width / 2,
		this->outline_shape_.getGlobalBounds().top + row_spacing / 2 - this->lines_[0].getGlobalBounds().height);
	this->header_text_[3].setPosition(((this->outline_shape_.getGlobalBounds().left + this->outline_shape_.getGlobalBounds().width) - column4_x) / 2 + column4_x - this->lines_[1].getGlobalBounds().width / 2,
		this->outline_shape_.getGlobalBounds().top + row_spacing / 2 - this->lines_[0].getGlobalBounds().height);
	this->header_text_[4].setPosition((this->outline_shape_.getGlobalBounds().left + column1_x) / 2 - this->lines_[1].getGlobalBounds().width / 2,
		this->outline_shape_.getGlobalBounds().top + 1 * row_spacing + row_spacing / 2 - this->lines_[0].getGlobalBounds().height);
	this->header_text_[5].setPosition((this->outline_shape_.getGlobalBounds().left + column1_x) / 2 - this->lines_[1].getGlobalBounds().width / 2,
		this->outline_shape_.getGlobalBounds().top + 2 * row_spacing + row_spacing / 2 - this->lines_[0].getGlobalBounds().height);
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

void Table::reset_table()
{
	for (int i = 0; i < this->texts_.size(); i++)
		this->texts_[i].clear();
}
