#ifndef  INPUTMANAGER_H
#define INPUTMANAGER_H

class InputManager {
	public:
	
		static bool GetMenuInput();
		static bool GetGameInput();

		static void SetMenuInput(bool value);
		static void SetGameInput(bool value);
	private:

		static bool _menuInput;
		static bool _gameInput;
};

#endif