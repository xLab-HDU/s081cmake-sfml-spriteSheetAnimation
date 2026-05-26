/*
 * 项目名称：s081cmake-sfml-spriteSheetAnimation
 * 开发者：李仕
 * 所属组织：xLab-HDU
 * 代码仓库：https://github.com/xLab-HDU
 * 
 * 本代码为开源项目，欢迎提交 Issue 和 Pull Request。
 */
#include <SFML/Graphics.hpp>
#include "Animation.hpp"
#include "AnimatedSprite.hpp"

int main(int argc, char* argv[])
{
	//创建窗口
	sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Example animation");

	//加载纹理图像
	sf::Texture textures("./data/images/sprite.png");

	//创建两个动画序列容器
	Animation walkLeft(&textures);
	walkLeft.addFramesLine(10, 2, 0);
	Animation walkRight(&textures);
	walkRight.addFramesLine(10, 2, 1);

	//创建精灵动画对象
	AnimatedSprite sprite(&walkLeft, AnimatedSprite::Playing, sf::seconds(0.1));

	//游戏循环
	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Time delta = clock.restart();

		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
		}

		float speed = 50; // 游戏对象移动速度
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left)) //向左移动
		{
			sprite.setAnimation(&walkLeft);
			sprite.play();
			sprite.move({ -speed * delta.asSeconds(), 0.0 });
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right))//向右移动 
		{
			sprite.setAnimation(&walkRight);
			sprite.play();
			sprite.move({ speed * delta.asSeconds(), 0.0 });
		}
		window.clear();
		sprite.update(delta); //根据帧频完成精灵动画的帧更新
		window.draw(sprite); //绘制精灵动画
		window.display();
	}
	return 0;
}

