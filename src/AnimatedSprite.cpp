#include "AnimatedSprite.hpp"
#include "Animation.hpp"
#include <SFML/Graphics.hpp>
#include <cassert>
//动画播放类
AnimatedSprite::FuncType AnimatedSprite::defaultFunc = []()->void{};
//构造函数仅将所有不同的属性初始化为它们的正确值
AnimatedSprite::AnimatedSprite(Animation* animation,Status status,
	const sf::Time& deltaTime,bool loop,int repeat)
	: on_finished(defaultFunc),_delta(deltaTime),_loop(loop),
	_repeat(repeat),_status(status)
{
    setAnimation(animation); 
}
//这个函数只在当前的纹理不同的情况下才会改变新的纹理，
//并将帧重设为新动画的第一个帧。
//注意，至少有一个帧必须存储在作为参数接收的新动画中。
void AnimatedSprite::setAnimation(Animation* animation)
{
    if(_animation != animation)
    {
        _animation = animation;
        _elapsed = sf::Time::Zero;
        _currentFrame = 0;
        setFrame(0,true);
    }
}

Animation* AnimatedSprite::getAnimation()const
{
    return _animation;
}

void AnimatedSprite::setFrameTime(sf::Time deltaTime)
{
    _delta = deltaTime;
}

sf::Time AnimatedSprite::getFrameTime()const
{
    return _delta;
}

void AnimatedSprite::setLoop(bool loop)
{
    _loop = loop;
}

bool AnimatedSprite::getLoop()const
{
    return _loop;
}
void AnimatedSprite::setRepeate(int nb)
{
    _repeat = nb;
}

int AnimatedSprite::getRepeate()const
{
    return _repeat;
}

void AnimatedSprite::play()
{
    _status = Playing;
}
void AnimatedSprite::pause()
{
    _status = Paused;
}

void AnimatedSprite::stop()
{
    _status = Stopped;
    _currentFrame = 0;
    setFrame(0,true);
}

AnimatedSprite::Status AnimatedSprite::getStatus()const
{
    return _status;
}
//此函数将当前帧更改为从内部动画类获取的新帧。
void AnimatedSprite::setFrame(size_t index)
{
    assert(_animation);
    _currentFrame = index % _animation->size();
    setFrame(_currentFrame,true);
}
//此函数改变所显示图像的颜色掩码。
//为此，我们将每个内部顶点的颜色设置为作为参数接收的新颜色:
void AnimatedSprite::setColor(const sf::Color& color)
{
    _vertices[0].color = color;
    _vertices[1].color = color;
    _vertices[2].color = color;
    _vertices[3].color = color;
}
///////////////////////////////////////////////////////////////
//它的工作是在达到时间限制时从当前帧切换到下一帧。
//当到达动画的最后一帧时，可以有以下3种选择：
//(1)根据_loop值，判定是否从第一帧开始重新播放动画
//(2)如果_repeat值有授权，则从第一帧开始重新播放动画
//(3)在所有其他情况下，通过调用内部回调来触发“on finish”事件
void AnimatedSprite::update(const sf::Time& deltaTime)
{
    if(_status == Playing && _animation)
    {
        _elapsed += deltaTime;
        if(_elapsed > _delta)
        {//帧切换
            _elapsed -= _delta;
            if(_currentFrame + 1 < _animation->size())
                ++_currentFrame;
            else
            {//已经是最后一帧
                _currentFrame = 0;
                if(!_loop)
                {//是否开始下次循环?
                    --_repeat;
                    if(_repeat<=0)
                    {//动画播放结束
                        _status = Stopped;
                        on_finished();
                    }
                }
            }
        }
		//更新当前帧
        setFrame(_currentFrame,false);
    }
}
//该函数的目的是将内部Animation类对象_animation的
//属性（位置和纹理坐标）赋值给动画帧的4个顶点
void AnimatedSprite::setFrame(size_t index,bool resetTime)
{
    if(_animation)
    {
        sf::IntRect rect = _animation->getRect(index);

        _vertices[0].position = sf::Vector2f(0.f, 0.f);
        _vertices[1].position = sf::Vector2f(0.f, static_cast<float>(rect.size.y));
        _vertices[2].position = sf::Vector2f(static_cast<float>(rect.size.x), 
			static_cast<float>(rect.size.y));
        _vertices[3].position = sf::Vector2f(static_cast<float>(rect.size.x), 0.f);

        float left = static_cast<float>(rect.position.x);
        float right = left + static_cast<float>(rect.size.x);
        float top = static_cast<float>(rect.position.y);
        float bottom = top + static_cast<float>(rect.size.y);

        _vertices[0].texCoords = sf::Vector2f(left, top);
        _vertices[1].texCoords = sf::Vector2f(left, bottom);
        _vertices[2].texCoords = sf::Vector2f(right, bottom);
        _vertices[3].texCoords = sf::Vector2f(right, top);
    }

    if(resetTime)
        _elapsed = sf::Time::Zero;
}
//因为AnimatedSprite类继承自sf::Transformable，所以可以进行
//相应的几何变换。具体函数实现是设置要使用的纹理，
//并通过内部顶点数组绘制实现动画帧的绘制
void AnimatedSprite::draw(sf::RenderTarget& target,
	sf::RenderStates states) const
{
    if (_animation && _animation->_texture)
    {
        states.transform *= getTransform();
        states.texture = _animation->_texture;
        // target.draw(_vertices, 4, sf::Quads, states);
        // 用两个三角形拼成矩形，需 6 个顶点（原来 4 个顶点不变）
        sf::Vertex triangles[6] = {
            _vertices[0],
            _vertices[1],
            _vertices[2],
            _vertices[2],
            _vertices[3],
            _vertices[0]
        };

        target.draw(triangles, 6, sf::PrimitiveType::Triangles, states);
    }
}