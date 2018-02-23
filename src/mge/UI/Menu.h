#ifndef MENU_H
#define MENU_H

#include <string>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

class UiManager;

class Menu {
	public:
		Menu(std::string pMenuName);
		~Menu();

		void InitMenu(int imageCount, int buttonCount);

		void SetBackground(std::string filename);
		sf::Sprite GetBackgroundSprite();

		int GetImgCount();
		sf::Sprite GetImgAt(int index);

		void AddButton(std::string activeFilename, std::string inactiveFilename, int xPos, int yPos, std::string target);
		void AddImage(std::string filename, int xPos, int yPos);

		int GetButtonCount();
		sf::Sprite GetButtonAt(int index);

		int GetActiveButton();
		void SetActiveButton(int index);

		std::string GetMenuName();

		bool IsActive();
		void SetActive(bool value);

		void ProcessInput(sf::Event event);

		void SetUiManager(UiManager* manager);

	private:
		bool _isActive;

		int _activeButton;
		int _buttonCount;

		std::string _menuName;

		sf::Texture _backgroundTexture;
		sf::Sprite _backgroundSprite;

		std::vector<sf::Texture> _buttonActiveTextures;
		std::vector<sf::Texture> _buttonInactiveTextures;
		std::vector<sf::Sprite> _buttonSprites;
		std::vector<std::string> _buttonCommands;

		std::vector<sf::Texture> _imageTextures;
		std::vector<sf::Sprite> _imageSprites;

		void _useButton(int index);
		void _updateButtons();

		UiManager* _manager;
};

#endif