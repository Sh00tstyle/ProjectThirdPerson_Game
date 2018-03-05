-- NOTES --
-- Pivotpoint for each image is on the topleft
-- Order from first to last in the array (matters for buttons!)
Menu = {}

-- MenuName = 1, Menu background = 2
Menus = { 
  {"MainMenu",            "temp_bg.png"}, 
  {"LevelSelection",      "temp_bg.png"}, 
  {"Controls",            "temp_bg.png"}, 
  {"Credits",             "temp_bg.png"}, 
 
}

-- Levels should be LEVEL 1, LEVEL 2, etc.
-- MenuParent = 1, target = 2, activatedSprite = 3,  deactivatedSprite = 4,  positionX = 5,  postionY = 6
Buttons = {
   { "MainMenu",        "LevelSelection",    "mainmenu/play.png",             "mainmenu/play_deactivated.png",              145,    375 },
   { "MainMenu",        "Controls",          "mainmenu/controls.png",         "mainmenu/controls_deactivated.png",          145,    475 },
   { "MainMenu",        "Credits",           "mainmenu/credits.png",          "mainmenu/credits_deactivated.png",           145,    575 },
   { "MainMenu",        "Quit",              "mainmenu/quit.png",             "mainmenu/quit_deactivated.png",              145,    675 },  
   { "LevelSelection",  "LEVEL 1",           "levelselection/thumbnails/level_1.png",  "levelselection/thumbnails/level_1_deactivated.png",   212,     366 },  
   { "LevelSelection",  "LEVEL 2",           "levelselection/thumbnails/level_2.png",  "levelselection/thumbnails/level_2_deactivated.png",   612,     366 },  
   { "LevelSelection",  "LEVEL 3",           "levelselection/thumbnails/level_3.png",  "levelselection/thumbnails/level_3_deactivated.png",   1012,    366 },  
   { "LevelSelection",  "LEVEL 4",           "levelselection/thumbnails/level_4.png",  "levelselection/thumbnails/level_4_deactivated.png",   1412,    366 },  
 
   { "LevelSelection",   "MainMenu",  "backbutton.png",         "backbutton_deactivated.png",                        1700,  830 },  
   { "Controls",         "MainMenu",  "backbutton.png",         "backbutton_deactivated.png",                        1700,  830 },  
   { "Credits",          "MainMenu",  "backbutton.png",         "backbutton_deactivated.png",                        1700,  830 }, 
   
}

-- Parent = 1, Image = 2 PositionX = 3, PositionY = 4
Images = { 
  {"HUD",               "hud/time.png",                   40,       980 }, 
  {"HUD",               "hud/reset.png",                  1610,     980 },
  {"Controls",          "controls/controls.png",          105,      250 },
  {"Credits",           "credits/credits.png",            105,      250 },
  {"LevelSelection",    "levelselection/panel.png",       105,      250 },
  {"LevelSelection",    "levelselection/level_bg.png",    201,      355 },
  {"LevelSelection",    "levelselection/level_bg.png",    601,      355 },
  {"LevelSelection",    "levelselection/level_bg.png",    1001,     355 },
  {"LevelSelection",    "levelselection/level_bg.png",    1401,     355 },
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

print(count(Buttons, "LevelSelection"))

function Menu.init()

--create Menus
  for i=1,#Menus do 
    CreateMenu(Menus[i][1], count(Images, Menus[i][1]), count(Buttons, Menus[i][1]));
    SetBackground(Menus[i][1], Menus[i][2]);
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