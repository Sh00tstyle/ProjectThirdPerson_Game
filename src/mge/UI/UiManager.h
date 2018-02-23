#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "mge/eventSystem/SystemEventDispatcher.hpp"

class lua_State;
class Menu;

class UiManager : public SystemEventListener {
	public:
		UiManager(sf::RenderWindow* pWindow);
		~UiManager();

		void draw();
		
		void CloseApp();
		void SelectMenu(std::string target);

	private:
		sf::RenderWindow* _window;
		Menu* _activeMenu;

		static std::vector<Menu*> _menus;

		static int _createMenu(lua_State* state);
		static int _setMenuBackground(lua_State* state);
		static int _addButton(lua_State* state);
		static int _addImage(lua_State* state);

		void _drawAll();
		void _initMenus();

		virtual void onNotify(sf::Event pEvent);
};

#endif