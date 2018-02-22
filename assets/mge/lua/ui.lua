-- MenuParent = 1, destination = 2, activatedSprite = 3,  deactivatedSprite = 4,  positionX = 5,  postionY = 6
Buttons = {
  { "MainMenu",   "Play",       "activatedSprite.png",     "deactivatedSprite.png", 0,  0}, 
  { "MainMenu",   "Controls",   "activatedSprite.png",    "deactivatedSprite.png",  0,  0},
  { "MainMenu",   "Credits",    "activatedSprite.png",    "deactivatedSprite.png",  0,  0},
  { "MainMenu",   "Quit",       "activatedSprite.png",    "deactivatedSprite.png",  0,  0},
}

-- Parent = 1, Image = 2 PositionX = 3, PositionY = 4
Images = { 
  { "MainMenu", "Image.png", 0, 0 }, 
  { "MainMenu", "Image.png", 0, 0 },
  { "MainMenu", "Image.png", 0, 0 },
  { "MainMenu", "Image.png", 0, 0 },
}

-- MenuName = 1, Menu background = 2
Menus = { 
  {"MainMenu", "background.png"}, 
  {"Controls", "background.png"}, 
  {"Credits", "background.png"}, 
  {"LevelSelect", "background.png"}, 
  {"PauseMenu", "background.png"}, 
  {"HUD", "hudbackground.png"},
}


--function Menu.init()
--create Menus
  for i=1,#Menus do 
    --CreateMenu(Menus[i][1]);
    print("Create menu " .. Menus[i][1] .. " with background " .. Menus[i][2]);
    --SetBackground(Menus[i][2]);x
  end

-- create Images
  for i=1,#Images do 
    --AddImage(Images[i][1], Images[i][2], Images[i][3], Images[i][4]);
    print("On menu " .. Images[i][1] .. " create " .. Images[i][2] .. " on position X/Y " .. Images[i][3] .. Images[i][4]);
  end

  for i=1,#Buttons do 
--AddButton(Buttons[i][1], Buttons[i][2], Buttons[i][3], Buttons[i][4], Buttons[i][5], Buttons[i][6]);
    print("On menu " .. Buttons[i][1] .. " create a button which leads to " .. Buttons[i][2] .. " with the image " .. Buttons[i][3] .. " and deactivated " .. Buttons[i][4] .. " on position X/Y " ..    Buttons[i][5] .. Buttons[i][6]);
  --end
end