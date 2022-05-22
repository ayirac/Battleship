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
	// Creates an Animation given a sprite to target to apply the Animation textures to
	Animation(sf::Sprite& target);
	virtual ~Animation();
	// Adds a frame to the animation
	void addFrame(Frame&& frame);
	// Updates the animation given an elapsed time from a Clock
	void update(double elapsed);
	// Returns the length of the animation
	const double getLength() const { return totalLength; }
};

#endif