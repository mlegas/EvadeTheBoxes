#include "SetOriginToCenter.h"

// These functions come from my assignment from the first year. I decided to re-use it, as it is still beneficial for this assignment.
// They simply set the origin of the sprites to the centre.

void setOriginToCenter(sf::Sprite &_sprite)
{
	sf::FloatRect boundsRect = _sprite.getLocalBounds();
	_sprite.setOrigin(boundsRect.left + boundsRect.width / 2.0f, boundsRect.top + boundsRect.height / 2.0f);
}

void setOriginToCenter(sf::Text &_text)
{
	sf::FloatRect boundsRect = _text.getLocalBounds();
	_text.setOrigin(boundsRect.left + boundsRect.width / 2.0f, boundsRect.top + boundsRect.height / 2.0f);
}