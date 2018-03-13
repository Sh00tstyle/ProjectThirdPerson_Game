#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

class Menu : public SystemEventListener {
	public:
		Menu(std::string pMenuName);
		~Menu();

		void InitMenu(int imageCount, int buttonCount, int textCount, bool horizontalNav);

		void SetBackground(std::string filename);
		sf::Sprite GetBackgroundSprite();

		int GetImgCount();
		sf::Sprite GetImgAt(int index);

		void AddButton(std::string activeFilename, std::string inactiveFilename, int xPos, int yPos, std::string target);
		void AddImage(std::string filename, int xPos, int yPos);
		void AddText(std::string fontname, std::string text, int textSize, int r, int g, int b, bool isBold, int xPos, int yPos);

		int GetButtonCount();
		sf::Sprite GetButtonAt(int index);

		int GetTextCount();
		sf::Text GetTextAt(int index);
		void SetTextAt(int index, std::string newString);

		int GetActiveButton();
		void SetActiveButton(int index);

		void SetImageActive(int index);
		int GetImageActive();

		std::string GetMenuName();

		bool IsActive();
		void SetActive(bool value);

	private:
		bool _isActive;
		bool _horizontalNavigation;

		int _activeButton;
		int _buttonCount;

		int _imageActive;

		std::string _menuName;

		sf::Texture _backgroundTexture;
		sf::Sprite _backgroundSprite;

		std::vector<sf::Texture> _buttonActiveTextures;
		std::vector<sf::Texture> _buttonInactiveTextures;
		std::vector<sf::Sprite> _buttonSprites;
		std::vector<std::string> _buttonCommands;
		std::vector<sf::Text> _menuTexts;

		std::vector<sf::Texture> _imageTextures;
		std::vector<sf::Sprite> _imageSprites;

		void _useButton(int index);
		void _updateButtons();

		virtual void onNotify(sf::Event pEvent);

};

#endif