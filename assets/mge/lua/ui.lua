-- NOTES --
-- Pivotpoint for each image is on the topleft
-- Order from first to last in the array (matters for buttons!)
Menu = {}

levelcount = 10

-- MenuName = 1, Menu background = 2, 3 = Horizontal movement (true/false)
Menus = { 
  {"MAIN",                   "background.png",                  false}, 
  {"Credits",                "background.png",                  false}, 
  {"Controls",               "background.png",                  false}, 
  {"PAUSE",                  "",                                true},
  {"HUD",                    "",                                false},
  {"HUD_HINT",               "",                                false},
  {"LOADING",                "loading.png",                     false},
  {"LevelSelect 0",          "background.png",                  true},
  {"LevelSelect 1",          "background.png",              true},
  {"LEVEL 10",               "resolutionscreen/background.png", true},
}

-- Name = 1, Path == 2
Fonts = {
  {"Arial", "arial.ttf"}, 
  {"Shark", "sharkformalfunnyness.ttf"},
  {"Burbin", "burbin.ttf"},
  {"Myriad", "MYRIADPRO-BOLD.OTF"},
}

-- Target Levels should be LEVEL 1, LEVEL 2, etc.
-- MenuParent = 1, target = 2, activatedSprite = 3,  deactivatedSprite = 4,  positionX = 5,  postionY = 6
Buttons = {
  { "MAIN",             "LevelSelect 0",    "mainmenu/play.png",          "mainmenu/play_deactivated.png",              835,    460 },
  { "MAIN",             "Controls",         "mainmenu/controls.png",      "mainmenu/controls_deactivated.png",          800,    602 },
  { "MAIN",             "Credits",          "mainmenu/credits.png",       "mainmenu/credits_deactivated.png",           843,    772 },
  { "MAIN",             "EXIT",             "mainmenu/quit.png",          "mainmenu/quit_deactivated.png",              890,    910 },  
  { "Controls",         "MAIN",             "resolutionscreen/backbutton.png", "resolutionscreen/backbutton_deactivated.png",      1620,   840 },
  { "Credits",          "MAIN",             "resolutionscreen/backbutton.png", "resolutionscreen/backbutton_deactivated.png",      1620,   840 },
  { "LevelSelect 1",    "LevelSelect 0",    "levelselection/previousbutton.png", "levelselection/previousbutton_deactivated.png",  75, 550 },
  { "PAUSE",            "RESUME",           "pausemenu/no.png",     "pausemenu/no_deactivated.png",                 675,    545 },
  { "PAUSE",            "MAIN",             "pausemenu/yes.png",         "pausemenu/yes_deactivated.png",             1110,    545 },
  {"LEVEL 10",          "MAIN",             "backbutton.png",              "backbutton_deactivated.png", 950,  830 },
}

-- Parent = 1, Image = 2 PositionX = 3, PositionY = 4
Images = { 
  {"LevelSelect 0", "panel.png",                    65,     200},
  {"LevelSelect 0", "levelselection/levels.png",    150,    235},
  {"LevelSelect 1", "panel.png",                    65,     200},
  {"LevelSelect 1", "levelselection/levels.png",    150,    235},
  {"HUD",           "hud/hint_deactivated.png",     70,    945}, 
  {"HUD",           "hud/reset.png",                1725,   945}, 
  {"HUD",           "hud/tries.png",                800,    945}, 
  {"HUD_HINT",      "hud/hint.png",                 70,    945}, 
  {"HUD_HINT",      "hud/reset.png",                1725,   945}, 
  {"HUD_HINT",      "hud/tries.png",                800,    945}, 
  {"HUD_HINT",      "hud/hint_popup.png",           140,    815}, 
  {"PAUSE",         "hud/hint_deactivated.png",     70,    945}, 
  {"PAUSE",         "hud/reset.png",                1725,   945}, 
  {"PAUSE",         "hud/tries.png",                800,    945}, 
  {"Controls",      "panel.png",                    65,     200},
  {"Credits",       "panel.png",                    65,     200},
  {"Credits",      	"credits/credits.png",        	280,    238},
  {"Controls",      "controls/controls.png",        150,    212},
  {"PAUSE",         "pausemenu/background.png",     555,    210}, 
  {"MAIN",          "mainmenu/background.png",      420,    290},  
  {"MAIN",          "mainmenu/title.png",      630,    0},  
}
LevelRating = { 0, 1, 2, 1, 2, 1, 1, 1, 0, 0 }


-- Target Menu = 1 Font Name used = 2, Drawn Text = 4, text size = 5, textcolor r = 6, textcolor g = 7, textcolor b = 8, boldness = 9, posX = 10, posY = 11
Text = {
  {"HUD", "Myriad", "TRIES: TRIES", 45, 118, 85, 48, true, 858, 980},
  {"HUD_HINT", "Myriad", "TRIES: TRIES", 45, 118, 85, 48, true, 858, 980},
  {"PAUSE", "Myriad", "TRIES: TRIES", 45, 118, 85, 48, true, 858, 980},
  {"LOADING", "Arial", "", 60, 255, 255, 255, false, 1700, 900}, 
}


-- Insert all resolution screens
function addLevelResolutionScreens()
starX = 780
starY = 400
offset = 180
for i=1, levelcount  do 

    -- Levels menu
  newMenu = {"LEVEL " .. i, "", true}
  table.insert(Menus, newMenu)
  
  -- Levels panel
  newPanel =  {"LEVEL " .. i,  "resolutionscreen/background.png",   610, 230 }, 
  table.insert(Images, newPanel)
    
  -- Insert Next button
  newNextButton = {"LEVEL " .. i, "LEVEL " .. i + 1, "resolutionscreen/nextlevel.png", "resolutionscreen/nextlevel_deactivated.png", 725,  870 },
  table.insert(Buttons, newNextButton)
    
  -- Insert Back to menu button
  newBackButton = {"LEVEL " .. i, "MAIN", "resolutionscreen/backbutton.png", "resolutionscreen/backbutton_deactivated.png", 1145,  870},
  table.insert(Buttons, newBackButton)
  end
  -- Stars
  for i=1, levelcount  do 
    for a=1, 3 do
        newStar =  {"LEVEL " .. i,  "resolutionscreen/stars_" .. a ..".png",   800, 525 }, 
        table.insert(Images, newStar)
      end
    end
end

function addLevelSelection()
LevelSelect = 0

thumbnailX = 195
thumbnailY = 455
backgroundX = 150 
backgroundY = 455

imageOffset = 315
offsetmodifier = 0
staroffset = 20

  for i=1, levelcount do
    
    -- Thumbnails
    thumbnail = { "LevelSelect " .. LevelSelect,"LEVEL " .. i,"levelselection/thumbnails/Level_" .. i .. ".png",
    "levelselection/thumbnails/Level_" .. i .. "_deactivated.png", thumbnailX + imageOffset * offsetmodifier, thumbnailY}  
    table.insert(Buttons, thumbnail)
    offsetmodifier  = offsetmodifier + 1
    
    -- Reset variables once 5 levels have been displayed
    if(i == 5) then
      LevelSelect = 1
      offsetmodifier = 0
    end 
  end
  
  -- Additional Buttons
  table.insert(Buttons,{ "LevelSelect 0", "LevelSelect 1","levelselection/nextbutton.png", "levelselection/nextbutton_deactivated.png",  1758, 550 })
  table.insert(Buttons,{ "LevelSelect 1", "MAIN", "resolutionscreen/backbutton.png", "resolutionscreen/backbutton_deactivated.png",      1620,   840 }) 
  table.insert(Buttons,{ "LevelSelect 0", "MAIN", "resolutionscreen/backbutton.png", "resolutionscreen/backbutton_deactivated.png",      1620,   840 }) 
 
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