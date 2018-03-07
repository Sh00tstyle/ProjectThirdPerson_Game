-- NOTES --
-- Pivotpoint for each image is on the topleft
-- Order from first to last in the array (matters for buttons!)
Menu = {}

levelcount = 10

-- MenuName = 1, Menu background = 2, 3 = Horizontal movement (true/false)
Menus = { 
  {"MAIN",                   "temp_bg.png", false}, 
  {"Credits",                "temp_bg.png", false}, 
  {"Controls",               "temp_bg.png", false}, 
  {"PAUSE",                  "",            false},
  {"HUD",                    "",            false},
  {"LOADING",                "loading.png", false},
  {"LevelSelect 0",          "temp_bg.png", true},
  {"LevelSelect 1",          "temp_bg.png", true},
}

-- Name = 1, Path == 2
Fonts = {
  {"Arial", "arial.ttf"},
}

-- Target Levels should be LEVEL 1, LEVEL 2, etc.
-- MenuParent = 1, target = 2, activatedSprite = 3,  deactivatedSprite = 4,  positionX = 5,  postionY = 6
Buttons = {
  { "MAIN",             "LevelSelect 0",    "mainmenu/play.png",          "mainmenu/play_deactivated.png",              145,    375 },
  { "MAIN",             "Controls",         "mainmenu/controls.png",      "mainmenu/controls_deactivated.png",          145,    475 },
  { "MAIN",             "Credits",          "mainmenu/credits.png",       "mainmenu/credits_deactivated.png",           145,    575 },
  { "MAIN",             "EXIT",             "mainmenu/quit.png",          "mainmenu/quit_deactivated.png",              145,    675 },  
  { "Controls",         "MAIN",             "backbutton.png",             "backbutton_deactivated.png",                  1700,   830 },  
  { "Credits",          "MAIN",             "backbutton.png",             "backbutton_deactivated.png",                 1700,   830 },
  { "LevelSelect 1",    "LevelSelect 0",    "nextbutton.png",             "nextbutton_deactivated.png",                 125,    412 },
  { "PAUSE",            "RESUME",           "pausemenu/continue.png",     "pausemenu/continue_deactivated.png",         800,    400 },
  { "PAUSE",            "RESET",            "pausemenu/reset.png",        "pausemenu/reset_deactivated.png",         800,    600 },
  { "PAUSE",            "MAIN",             "pausemenu/menu.png",         "pausemenu/menu_deactivated.png",         800,    800 },
}

-- Parent = 1, Image = 2 PositionX = 3, PositionY = 4
Images = { 
  {"HUD",             "hud/tries_panel.png",                     40,       980 }, 
  {"Controls",        "controls/controls.png",            105,      250 },
  {"Credits",         "credits/credits.png",              105,      250 },
  {"PAUSE",           "pausemenu/panel.png",             730,      250 }, 
  {"LevelSelect 0",   "levelselection/panel.png",         105,      250 },
  {"LevelSelect 1",   "levelselection/panel.png",         105,      250 },
}

-- Target Menu = 1 Font Name used = 2, Drawn Text = 4, text size = 5, textcolor r = 6, textcolor g = 7, textcolor b = 8, boldness = 9, posX = 10, posY = 11
Text = {
  { "LevelSelect 0", "Arial", "Level 1", 25, 0, 0, 0, true, 150, 700},
  { "HUD", "Arial", "Tries: TRIES", 25, 0, 0, 0, true, 80, 1000},
}


-- Insert all resolution screens
function addLevelResolutionScreens()
  starX = 780
  starY = 400
  offset = 150
  for i=1, levelcount do 
    -- Levels menu
    newMenu = {"LEVEL " .. i, "resolutionscreen/background.png", true}
    table.insert(Menus, newMenu)
    
    -- Insert Next button
    newNextButton = {"LEVEL " .. i, "LEVEL " .. i + 1, "resolutionscreen/nextlevel.png", "resolutionscreen/nextlevel_deactivated.png", 700,  830 },
    table.insert(Buttons, newNextButton)
    
    -- Insert Back to menu button
    newBackButton = {"LEVEL " .. i, "MAIN", "backbutton.png", "backbutton_deactivated.png", 1200,  830 },
    table.insert(Buttons, newBackButton)
    
    newText =  { "LEVEL " .. i, "Arial", "You completed Level " .. i .. " in " .. "TRIES" .. " tries", 45, 0, 0, 0, true, 650, 540},
    table.insert(Text, newText)
    
    -- Insert star rating
    for a=0, 2 do
      newStar = { "LEVEL " .. i,   "resolutionscreen/star.png", starX + offset * a, starY }
      table.insert(Images, newStar)
    end
  end
 end

function addLevelSelection()
-- Add Menus

LevelSelect = 0

thumbnailX = 160 
thumbnailY = 366
backgroundX = 150 
backgroundY = 355

imageOffset = 325
offsetmodifier = 0
levelSelectionBackground = "levelselection/level_bg.png"

  for i=1, levelcount do
    -- Thumbnails
    thumbnail = { "LevelSelect " .. LevelSelect,"LEVEL " .. i,"levelselection/thumbnails/level_".. i ..".png",
    "levelselection/thumbnails/level_".. i .."_deactivated.png", thumbnailX + imageOffset * offsetmodifier, thumbnailY}  
    table.insert(Buttons, thumbnail)
           
    -- Thumbnail Backgrounds
    thumbnailBackground = { "LevelSelect " .. LevelSelect, levelSelectionBackground, backgroundX + imageOffset * offsetmodifier, backgroundY} 
    table.insert(Images, thumbnailBackground)
    
    
    --Level Text
    --newText = {"LevelSelect 0", "Arial", "Level " .. i, 12, 0, 0, 0, true, backgroundX + imageOffset * offsetmodifier, backgroundY + 125 }
    --table.insert(Text, newText)
    --print(newText)
    
    offsetmodifier  = offsetmodifier + 1
    -- Reset variables once 5 levels have been displayed
    if(i == 5) then
      LevelSelect = 1
      offsetmodifier = 0
    end 
  end
  
  -- Additional Buttons
  table.insert(Buttons,{ "LevelSelect 0", "LevelSelect 1","nextbutton.png", "nextbutton_deactivated.png",  1754, 412 })
  table.insert(Buttons,{ "LevelSelect 1", "MAIN", "backbutton.png","backbutton_deactivated.png",      1700, 830 })
  table.insert(Buttons,{ "LevelSelect 0", "MAIN", "backbutton.png","backbutton_deactivated.png",      1700, 830 }) 
  
  
 end

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
-- Add all thumbnails, images and buttons for the levelselection screens to the array
addLevelSelection()

-- Add a resolution screen for each level to the array
addLevelResolutionScreens()

--create Menus
  for i=1,#Menus do 
    CreateMenu(Menus[i][1], count(Images, Menus[i][1]), count(Buttons, Menus[i][1]), count(Text, Menus[i][1]), Menus[i][3]);
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
  
  --Add fonts
  for i=1,#Fonts do 
   CreateFont(Fonts[i][1], Fonts[i][2]);
  end 
  
  --Add Text
  for i=1,#Text do 
    AddText(Text[i][1], Text[i][2], Text[i][3], Text[i][4], Text[i][5], Text[i][6], Text[i][7], Text[i][8], Text[i][9], Text[i][10]);
  end 
end