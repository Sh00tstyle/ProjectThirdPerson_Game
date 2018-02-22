#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

class Menu {
	public:
		Menu(std::string pMenuName);
		~Menu();

		void InitMenu(int imageCount, int buttonCount);

		void SetBackground(std::string filename);
		sf::Sprite GetBackgroundSprite();

		int GetImgCount();
		sf::Sprite GetImgAt(int index);

		void AddButton(std::string filename);
		void AddImage(std::string filename, int xPos, int yPos);

		std::string GetMenuName();

	private:
		int _activeButton;
		int _buttonCount;

		std::string _menuName;

		sf::Texture _backgroundTexture;
		sf::Sprite _backgroundSprite;

		std::vector<sf::Texture> _buttonTextures;
		std::vector<sf::Sprite> _buttonSprites;

		std::vector<sf::Texture> _imageTextures;
		std::vector<sf::Sprite> _imageSprites;
};

#endif