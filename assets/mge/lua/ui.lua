-- NOTES --
-- Pivotpoint for each image is on the topleft
-- Order from first to last in the array (matters for buttons!)
Menu = {}

-- MenuName = 1, Menu background = 2
Menus = { 
  {"MainMenu",               "temp_bg.png"}, 
  {"Credits",                "temp_bg.png"}, 
  {"Controls",               "temp_bg.png"}, 
  {"PAUSE",                  "temp_bg.png"},
  {"LevelSelect 1",          "temp_bg.png"},
  {"LevelSelect 2",          "temp_bg.png"},
}


-- Name = 1, Path == 2
Fonts = {
  {"FontName", "font.ttf"},
  {"FontName", "font.ttf"},
}

-- Target Levels should be LEVEL 1, LEVEL 2, etc.
-- MenuParent = 1, target = 2, activatedSprite = 3,  deactivatedSprite = 4,  positionX = 5,  postionY = 6
Buttons = {
  { "MainMenu",         "LevelSelect 0",     "mainmenu/play.png",        "mainmenu/play_deactivated.png",              145,    375 },
  { "MainMenu",         "Controls",         "mainmenu/controls.png",    "mainmenu/controls_deactivated.png",          145,    475 },
  { "MainMenu",         "Credits",          "mainmenu/credits.png",     "mainmenu/credits_deactivated.png",           145,    575 },
  { "MainMenu",         "Quit",             "mainmenu/quit.png",        "mainmenu/quit_deactivated.png",              145,    675 },  
  { "Controls",         "MainMenu",         "backbutton.png",           "backbutton_deactivated.png",                 1700,  830 },  
  { "Credits",          "MainMenu",         "backbutton.png",           "backbutton_deactivated.png",                 1700,  830 },
  { "LevelSelect 1",     "LevelSelect 0",     "nextbutton.png",           "nextbutton_deactivated.png",                 125, 412 },
}

-- Parent = 1, Image = 2 PositionX = 3, PositionY = 4
Images = { 
  {"HUD",             "hud/time.png",                     40,       980 }, 
  {"HUD",             "hud/reset.png",                    1610,     980 },
  {"Controls",        "controls/controls.png",            105,      250 },
  {"Credits",         "credits/credits.png",              105,      250 },
  {"LevelSelect0",    "levelselection/panel.png",         105,      250 },
  {"LevelSelect1",    "levelselection/panel.png",         105,      250 },
}

-- Target Menu = 1 Font Name used = 2, Text Name/Key = 3, Drawn Text = 4, text size = 5, textcolor r = 6, textcolor g = 7, textcolor b = 9, boldness = 9, x pos = 10, y pos = 11
Text = {
  {"HUD", "FontName", "TRIES", "Tries: ", 12, 0, 0, 0, true, 100, 100 },
}

-- Insert all resolution screens
function addLevelResolutionScreens()
  for i=1, levelcount do 
    -- Levels menu
    newMenu = {"LEVEL " .. i, "resolutionscreen/background.png"}
    table.insert(Menus, newMenu)
    
    -- Insert Next button
    newNextButton = {"LEVEL " .. i, "LEVEL " .. i + 1, "resolutionscreen/nextlevel.png", "resolutionscreen/nextlevel_deactivated.png", 700,  830 },
    table.insert(Buttons, newNextButton)
    
    -- Insert Back to menu button
    newBackButton = {"LEVEL " .. i, "MainMenu", "backbutton.png", "backbutton_deactivated.png", 1200,  830 },
    table.insert(Buttons, newBackButton)
  end
 end

function addLevelSelection()
-- Add Menus
  
levelcount = 10
LevelSelect = 0

thumbnailX = 150 
thumbnailY = 366
backgroundX = 140 
backgroundY = 355

imageOffset = 325
offsetmodifier = 0
levelSelectionBackground = "levelselection/level_bg.png"

  for i=1, levelcount do
    -- Thumbnails
    thumbnail = { "LevelSelect" .. LevelSelect,"LEVEL " .. i,"levelselection/thumbnails/level_".. i ..".png",
    "levelselection/thumbnails/level_".. i .."_deactivated.png", thumbnailX + imageOffset * offsetmodifier, thumbnailY}  
    table.insert(Buttons, thumbnail)
           
    -- Thumbnail Backgrounds
    thumbnailBackground = { "LevelSelect " .. LevelSelect, levelSelectionBackground, backgroundX + imageOffset * offsetmodifier, backgroundY} 
    table.insert(Images, thumbnailBackground)
    
    offsetmodifier  = offsetmodifier + 1
    -- Reset variables once 5 levels have been displayed
    if(i == 5) then
      LevelSelect = 1
      offsetmodifier = 0
    end 
  end
  
  -- Additional Buttons
  table.insert(Buttons,{ "LevelSelect 0", "LevelSelect1","nextbutton.png", "nextbutton_deactivated.png",  1754, 412 })
  table.insert(Buttons,{ "LevelSelect 1", "MainMenu", "backbutton.png","backbutton_deactivated.png",      1700, 830 })
  table.insert(Buttons,{ "LevelSelect 0", "MainMenu", "backbutton.png","backbutton_deactivated.png",      1700, 830 }) 
  
  
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
  
  -- Add fonts
  -- for i=1,#Fonts do 
  --  CreateFont(Fonts[i][1], Fonts[i][2]);
  --end 
  
  -- Add Text
  --for i=1,#Text do 
  --  CreateText(Text[i][1], Text[i][2], Text[i][3], Text[i][4], Text[i][5], Text[i][6], Text[i][7], Text[i][8], Text[i][9], Text[i][10], Text[i][11]);
  --end 
end