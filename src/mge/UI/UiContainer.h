#ifndef UICONTAINER_H
#define UICONTAINER_H

#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include "mge/eventSystem/SystemEventDispatcher.hpp"

class lua_State;
class Menu;

class UiContainer : public SystemEventListener {
	public:
		UiContainer(sf::RenderWindow* pWindow);
		~UiContainer();

		void draw();
		
		static void CloseApp();
		static void SelectMenu(std::string target);

		static sf::Font* GetFontByName(std::string fontname);

		static int GetIgnoCount();
		static bool GetHintActive();
		static void SetHintActive(bool value);
		static int GetHintsTaken();

		static void ResetHints();

		void drawLoading(int percentage);

	private:
		static sf::RenderWindow* _window;
		static Menu* _activeMenu;

		static bool _hintActive;
		static int _hintIgnoCount;
		static int _hintsTaken;

		static std::vector<Menu*> _menus;
		static std::map<std::string, sf::Font*> _fonts;

		static int _createMenu(lua_State* state);
		static int _createFont(lua_State* state);
		static int _setBackground(lua_State* state);
		static int _addButton(lua_State* state);
		static int _addImage(lua_State* state);
		static int _addText(lua_State* state);

		void _drawAll();
		void _initMenus();

		virtual void onNotify(sf::Event pEvent);
};

#endif