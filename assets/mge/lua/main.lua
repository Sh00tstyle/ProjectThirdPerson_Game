Menu = {}
Sound = {}

function Menu.init()  
  --creating a new menu (menu name/key)
  CreateMenu("Main", 1, 0)
  --CreateMenu("Options")
  
  --set background of the given menu (parent menu key, image filename)
  SetBackground("Main", "test_bg.png");
  
  --add new image (parent menu key, image filename, x position, y position)
  AddImage("Main", "test_img.png", 0, 0)
  
  --add new button (parent menu key, target menu key, image filename (active), image filename (inactive), x position, y position)
  --Special keys: Exit, Level_1, Level_2, etc.
  --AddButton("Main", "Options", "button_active.png", "button_inactive.png", 0, 0)
  
  --W and A for up/left, S and D for down/right, Enter and Space for entering
end

--placeholder in case we are doing sound management in lua as well
function Sound.init()
  
end