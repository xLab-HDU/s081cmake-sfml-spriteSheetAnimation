#include "Animation.hpp"
//仅仅是一个frame容器
Animation::Animation(sf::Texture* texture) : _texture(texture)
{
}

Animation::~Animation()
{
}

void Animation::setTexture(sf::Texture* texture)
{
    _texture = texture;
}

sf::Texture* Animation::getTexture()const
{
    return _texture;
}
    
Animation& Animation::addFrame(const sf::IntRect& rect)
{
    _frames.emplace_back(rect);
    return *this;
}
//在sprite sheet上，按行加载序列
Animation& Animation::addFramesLine(int number_x,int number_y,int line)
{
    const sf::Vector2u size = _texture->getSize();
    const float delta_x = size.x / float(number_x);
    const float delta_y = size.y / float(number_y);

    for(int i = 0;i<number_x;++i)
        addFrame(sf::IntRect({static_cast<int>(i*delta_x),
                                static_cast<int>(line*delta_y)},
                                {static_cast<int>(delta_x),
                                static_cast<int>(delta_y)}));
    return *this;
}
//在sprite sheet上，按列加载序列
Animation& Animation::addFramesColumn(int number_x,int number_y,int column)
{
    const sf::Vector2u size = _texture->getSize();
    const float delta_x = size.x / float(number_x);
    const float delta_y = size.y / float(number_y);

    for(int i = 0;i<number_y;++i)
        addFrame(sf::IntRect({static_cast<int>(column*delta_x),
                                static_cast<int>(i*delta_y)},
                                {static_cast<int>(delta_x),
                                static_cast<int>(delta_y)}));
    return *this;
}

size_t Animation::size()const
{
    return _frames.size();
}

const sf::IntRect& Animation::getRect(size_t index)const
{
    return _frames[index];
}