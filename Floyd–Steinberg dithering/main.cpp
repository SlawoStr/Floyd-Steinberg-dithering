#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Simulation", sf::Style::Fullscreen);
	window.setFramerateLimit(144);
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
	std::string filename = "img1.jpg";
	image.loadFromFile(filename);
	texture.loadFromImage(image);
	sprite.setTexture(texture);

	float factor = 0.0f;
	bool updated = true;

	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::KeyPressed)
			{
				if (e.key.code == sf::Keyboard::Escape)
				{
					window.close();
				}
			}
			if (e.type == sf::Event::MouseButtonPressed)
			{
				if (e.key.code == sf::Mouse::Left)
				{
					factor += 1.0f;
					// New image should be rendered
					updated = true;
					for (unsigned i = 0; i < image.getSize().y-1; i++)
						for (unsigned j = 1; j < image.getSize().x-1; j++)
						{
							sf::Color oldpixel = image.getPixel(j, i);
							sf::Color newPixel;

							newPixel.r = static_cast<int>(round(factor * oldpixel.r / 255) * (255 / factor));
							newPixel.g = static_cast<int>(round(factor * oldpixel.g / 255) * (255 / factor));
							newPixel.b = static_cast<int>(round(factor * oldpixel.b / 255) * (255 / factor));
							newPixel.a = 255;
							image.setPixel(j, i, newPixel);

							// Error Diffusion
							float errR = static_cast<float>(oldpixel.r - newPixel.r);
							float errG = static_cast<float>(oldpixel.g - newPixel.g);
							float errB = static_cast<float>(oldpixel.b - newPixel.b);


							sf::Color currentPixel = image.getPixel(j + 1, i);
							currentPixel.a = 255;
							currentPixel.r += static_cast<sf::Uint8>(errR * 7 / 16.0f);
							currentPixel.g += static_cast<sf::Uint8>(errG * 7 / 16.0f);
							currentPixel.b += static_cast<sf::Uint8>(errB * 7 / 16.0f);
							image.setPixel(j + 1, i, currentPixel);

							currentPixel = image.getPixel(j - 1, i + 1);
							currentPixel.r += static_cast<sf::Uint8>(errR * 3 / 16.0f);
							currentPixel.g += static_cast<sf::Uint8>(errG * 3 / 16.0f);
							currentPixel.b += static_cast<sf::Uint8>(errB * 3 / 16.0f);
							image.setPixel(j - 1, i + 1, currentPixel);

							currentPixel = image.getPixel(j, i + 1);
							currentPixel.r += static_cast<sf::Uint8>(errR * 5 / 16.0f);
							currentPixel.g += static_cast<sf::Uint8>(errG * 5 / 16.0f);
							currentPixel.b += static_cast<sf::Uint8>(errB * 5 / 16.0f);
							image.setPixel(j, i + 1, currentPixel);

							currentPixel = image.getPixel(j + 1, i + 1);
							currentPixel.r += static_cast<sf::Uint8>(errR * 1 / 16.0f);
							currentPixel.g += static_cast<sf::Uint8>(errG * 1 / 16.0f);
							currentPixel.b += static_cast<sf::Uint8>(errB * 1 / 16.0f);
							image.setPixel(j + 1, i + 1, currentPixel);
						}
					// Set new texture
					texture.loadFromImage(image);
					sprite.setTexture(texture);
					// Reload image
					image.loadFromFile(filename);
				}
			}
		}
		if (updated)
		{
			window.clear();
			window.draw(sprite);
			window.display();
		}
	}
	return 0;
}