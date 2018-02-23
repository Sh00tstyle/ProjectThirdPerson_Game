Menu = {}

function Menu.init()  
  --creating a new menu (menu name/key, image count, button count)
  CreateMenu("Main", 0, 3)
  CreateMenu("Options", 0, 0)
  
  --set background of the given menu (parent menu key, image filename)
  SetBackground("Main", "test_bg.png");
  
  --add new image (parent menu key, image filename, x position, y position)
  --AddImage("Main", "test_img.png", 0, 0)
  
  --add new button (parent menu key, target menu key, image filename (active), image filename (inactive), x position, y position)
  --Special keys: Exit, Level_1, Level_2, etc.
  AddButton("Main", "LEVEL 1", "test_img.png", "test_img2.png", 100, 200)
  AddButton("Main", "Options", "test_img.png", "test_img2.png", 100, 500)
  AddButton("Main", "EXIT", "test_img.png", "test_img2.png", 100, 800)
  
  --W and A for up/left, S and D for down/right, Enter and Space for entering
end