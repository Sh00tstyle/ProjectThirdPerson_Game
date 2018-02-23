-- NOTES --
-- Pivotpoint for each image is on the topleft
-- Order from first to last in the array (matters for buttons!)
Menu = {}

-- MenuName = 1, Menu background = 2
Menus = { 
  {"MainMenu", "test_img.png"}, 
 
}

-- Levels should be LEVEL 1, LEVEL 2, etc.
-- MenuParent = 1, target = 2, activatedSprite = 3,  deactivatedSprite = 4,  positionX = 5,  postionY = 6
Buttons = {
   { "MainMenu",   "Play",      "mainmenu/play.png",            "mainmenu/play_deactiavted.png",         145,   375 },
   { "MainMenu",   "Controls",  "mainmenu/controls.png",        "mainmenu/controls_deactiavted.png",     145,   475 },
   { "MainMenu",   "Credits",   "mainmenu/credits.png",         "mainmenu/credits_deactiavted.png",      145,   575 },
   { "MainMenu",   "Quit",      "mainmenu/quit.png",            "mainmenu/quit_deactiavted.png",         145,   675 },   
}

-- Parent = 1, Image = 2 PositionX = 3, PositionY = 4
Images = { 
  { "HUD",        "hud/time.png",               40,     980 }, 
  { "HUD",        "hud/reset.png",              1610,   980 }, 
}


-- returns the amount of images or buttons for each menu/parent
function count(class, string)
local count = 0;
  for i=1,#class do 
    if(class[i][1] == string) then
      count = count + 1;
    end
  end
  return count;
end


function Menu.init()

--create Menus
  for i=1,#Menus do 
    CreateMenu(Menus[i][1], count(Images, Menus[i][1]), count(Buttons, Menus[i][1]));
    SetBackground(Menus[1][1], Menus[1][2]);
  end
  
--create Images
  for i=1,#Images do 
    AddImage(Images[i][1], Images[i][2], Images[i][3], Images[i][4]);
  end

-- create Buttons
  for i=1,#Buttons do 
    AddButton(Buttons[i][1], Buttons[i][2], Buttons[i][3], Buttons[i][4], Buttons[i][5], Buttons[i][6]);
  end
end