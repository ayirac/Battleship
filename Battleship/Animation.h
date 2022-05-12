#ifndef ANIMATION_H
#define ANIMATION_H
#include <vector>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>

struct Frame {
	sf::IntRect rect;
	double duration; // in seconds
};

class Animation {  
	std::vector<Frame> frames;
	double totalLength;
	double progress;
	sf::Sprite& target;

public:
	Animation(sf::Sprite& target);
	virtual ~Animation();
	void addFrame(Frame&& frame);
	void update(double elapsed);
	const double getLength() const { return totalLength; }
};

#endif