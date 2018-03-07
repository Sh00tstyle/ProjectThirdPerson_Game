Menu = {} --needs to be there so that C++ can call the Menu.init() function

function Menu.init() 
  --Menu Navigation: Enter and Space for entering buttons, Esc for the Pause menu (when in level/hud), Esc to resume the level (when in Pause menu)
  --                 W/S for button navigation (without horizontal navigation), A/D for button navigation (without horizontal navigation)
  --Image filepath: assets/mge/ui
  --Font filepath: assets/mge/fonts
  
  --creating a new menu (menu name/key, image amount, button amount, text amount, horizontal navigation)
  --special keys: MAIN, PAUSE, HUD, LOADING, LEVEL 1, LEVEL 2, etc.
  CreateMenu("MAIN", 0, 3, 1, false) --key for the main menu
  CreateMenu("Options", 0, 0, 0, false)
  CreateMenu("PAUSE", 0, 1, 0, true) --key for the Pause menu, also has horizantal navigation
  CreateMenu("HUD", 0, 0, 1, false) --key for the HUD, it gets loaded whenever a level is selected by a button
  CreateMenu("LEVEL 1", 0, 1, 0, false) --key for the resolution screen of level 1
  CreateMenu("LEVEL 2", 0, 0, 0, false)
  CreateMenu("LOADING", 0, 0, 0, false) --key for the loading screen when starting up the game
  
  --creating a new font to use for text (font name/key, font filename), only call this once for every font
  CreateFont("MyFont", "arial.ttf")
  
  --set background of the given menu (parent menu key, image filename)
  SetBackground("MAIN", "test_bg.png")
  SetBackground("Options", "test_bg.png")
  SetBackground("PAUSE", "test_bg.png")
  SetBackground("LEVEL 1", "test_bg.png")
  SetBackground("LOADING", "loading.png")
  
  --add new image (parent menu key, image filename, x position, y position)
  AddImage("MAIN", "test_img.png", 0, 0)
  AddImage("PAUSE", "test_img.png", 0, 0) --also works for special key menus to customize them
  
  --add new text  (parent menu key, used font name, text to draw/special key, text size, text color r, text color g, text color b, boldness (true/false), x position, y position)
  --RGB values must be whole numbers from 0-255
  --special keys: TRIES (will replace the TRIES keyword with the actual number)
  AddText("MAIN", "MyFont", "Hello World", 12, 255, 255, 255, false, 50, 50) -- non bold, white text with size 12 saying "Hello World"
  AddText("HUD", "MyFont", "Tries: TRIES", 15, 0, 0, 0, true, 0, 0) -- bold, black text with size 15 displaying "Tries: X" (x is the amount of tries filled in by the engine)
  
  --add new button (parent menu key, target (menu) key, image filename (active), image filename (inactive), x position, y position)
  --special target keys: EXIT, RESUME, RESET, LEVEL 1, LEVEL 2, etc.
  AddButton("MAIN", "LEVEL 1", "test_img.png", "test_img2.png", 100, 200) --loads level 1
  AddButton("MAIN", "Options", "test_img.png", "test_img2.png", 100, 500)
  AddButton("MAIN", "EXIT", "test_img.png", "test_img2.png", 100, 800) --closes the application
  AddButton("PAUSE", "RESUME", "test_img.png", "test_img2.png", 400, 600) --resumes the paused level
  AddButton("LEVEL 1", "LEVEL 2", "test_img.png", "test_img2.png", 400, 600) --load level 2 from the resolution screen of level 1
end