-- NOTES --
-- Pivotpoint for each image is on the topleft
-- Order from first to last in the array (matters for buttons!)
Menu = {}

levelcount = 10

-- MenuName = 1, Menu background = 2, 3 = Horizontal movement (true/false)
Menus = { 
  {"MAIN",                   "background.png", false}, 
  {"Credits",                "background.png", false}, 
  {"Controls",               "background.png", false}, 
  {"PAUSE",                  "",            true},
  {"HUD",                    "",            false},
  {"LOADING",                "loading.png", false},
  {"LevelSelect 0",          "background.png", true},
  {"LevelSelect 1",          "background.png", true},
  {"LEVEL 10",          "resolutionscreen/background.png", true}
}

-- Name = 1, Path == 2
Fonts = {
  {"Arial", "arial.ttf"}, 
  {"Shark", "sharkformalfunnyness.ttf"},
  {"Burbin", "burbin.ttf"},
}

-- Target Levels should be LEVEL 1, LEVEL 2, etc.
-- MenuParent = 1, target = 2, activatedSprite = 3,  deactivatedSprite = 4,  positionX = 5,  postionY = 6
Buttons = {
  { "MAIN",             "LevelSelect 0",    "mainmenu/play.png",          "mainmenu/play_deactivated.png",              145,    375 },
  { "MAIN",             "Controls",         "mainmenu/controls.png",      "mainmenu/controls_deactivated.png",          145,    475 },
  { "MAIN",             "Credits",          "mainmenu/credits.png",       "mainmenu/credits_deactivated.png",           145,    575 },
  { "MAIN",             "EXIT",             "mainmenu/quit.png",          "mainmenu/quit_deactivated.png",              145,    675 },  
  { "Controls",         "MAIN",             "backbutton.png",             "backbutton_deactivated.png",                  885,   900 },  
  { "Credits",          "MAIN",             "backbutton.png",             "backbutton_deactivated.png",                 1700,   830 },
  { "LevelSelect 1",    "LevelSelect 0",    "previousbutton.png",         "previousbutton_deactivated.png",                 82,    490 },
  { "PAUSE",            "RESUME",           "pausemenu/no.png",     "pausemenu/no_deactivated.png",                 675,    545 },
  { "PAUSE",            "MAIN",             "pausemenu/yes.png",         "pausemenu/yes_deactivated.png",             1110,    545 },
  {"LEVEL 10",          "MAIN",             "backbutton.png",              "backbutton_deactivated.png", 950,  830 },
}

-- Parent = 1, Image = 2 PositionX = 3, PositionY = 4
Images = { 
  {"LevelSelect 0", "panel.png", 65, 200},
  {"LevelSelect 0", "levelselection/levels.png", 150, 235},
  {"LevelSelect 1", "panel.png",65, 200},
  {"LevelSelect 1", "levelselection/levels.png", 150, 235},
  {"HUD",             "hud/tries_panel.png",                     40,       980 }, 
  {"Controls",        "panel.png",            65, 200},
 
  {"Credits",         "panel.png",            65, 200},
  {"Controls",         "controls/controls.png",            100, 205},
  {"PAUSE",           "pausemenu/background.png",             555,      210 }, 
}
LevelRating = { 0, 1, 2, 1, 2, 1, 1, 1, 0, 0 }


-- Target Menu = 1 Font Name used = 2, Drawn Text = 4, text size = 5, textcolor r = 6, textcolor g = 7, textcolor b = 8, boldness = 9, posX = 10, posY = 11
Text = {
  { "HUD", "Arial", "Tries: TRIES", 25, 0, 0, 0, true, 80, 1000},
  { "LOADING", "Arial", "", 60, 255, 255, 255, false, 1700, 900},
  {"Credits", "Shark", "Daan Kemps", 75, 10, 10 , 10, true, 230, 320},
  {"Credits", "Shark", "Felix de Natris", 75, 10, 10 , 10, true, 230, 400},
  {"Credits", "Shark", "Lucia Dzediti", 75, 10, 10 , 10, true, 230, 480},
  {"Credits", "Shark", "Nico Mazek ", 75, 10, 10 , 10, true, 230, 560},
  {"Credits", "Shark", "Anna Richter", 75, 10, 10 , 10, true, 230, 640},
  {"Credits", "Shark", "Alex Lysenko", 75, 10, 10 , 10, true, 230, 720},
  {"Credits", "Shark", "ROLE", 75, 10, 10 , 10, true, 1050, 320},
  {"Credits", "Shark", "ROLE", 75, 10, 10 , 10, true, 1050, 400},
  {"Credits", "Shark", "SPECIAL ROLE", 75, 10, 10 , 10, true, 1050, 480},
  {"Credits", "Shark", "SPECIMEN", 75, 10, 10 , 10, true, 1050, 560},
  {"Credits", "Shark", "ROLE", 75, 10, 10 , 10, true, 1050, 640},
  {"Credits", "Shark", "ROLE", 75, 10, 10 , 10, true, 1050, 720},
  { "LEVEL 10", "Shark", "You completed Level 10 in TRIES tries", 50, 0, 0, 0, true, 660, 540},
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
    
    
    
    -- Insert Next button
    newNextButton = {"LEVEL " .. i, "LEVEL " .. i + 1, "resolutionscreen/nextlevel.png", "resolutionscreen/nextlevel_deactivated.png", 725,  870 },
    table.insert(Buttons, newNextButton)
    
    -- Insert Back to menu button
    newBackButton = {"LEVEL " .. i, "MAIN", "resolutionscreen/backbutton.png", "resolutionscreen/backbutton_deactivated.png", 1145,  870},
    table.insert(Buttons, newBackButton)
    
    newText =  { "LEVEL " .. i, "Burbin", "You completed Level " .. i .. " in " .. "TRIES" .. " tries", 38, 0, 0, 0, true, 730, 540},
    table.insert(Text, newText)
    

    
        -- Levels panel
    newPanel =  {"LEVEL " .. i,  "resolutionscreen/background.png",   610, 230 }, 
    table.insert(Images, newPanel)
    
    -- Insert star rating
    for a=0, 2 do
     if(a <= LevelRating[i]) then
        activatedStar = { "LEVEL " .. i,   "resolutionscreen/star.png", starX + offset * a, starY }
        table.insert(Images, activatedStar)
      elseif (a > LevelRating[i]) then
        deactivatedStar = { "LEVEL " .. i,   "resolutionscreen/star_deactivated.png", starX + offset * a, starY }
        table.insert(Images, deactivatedStar)
      end
    end
    
  end
 end

function addLevelSelection()
LevelSelect = 0

thumbnailX = 195
thumbnailY = 455
backgroundX = 195 
backgroundY = 455

imageOffset = 315
offsetmodifier = 0
staroffset = 20

  for i=1, levelcount do
    -- Insert star activated and deactivated stars;
    -- for a=0, 2 do
    --  if(a <= LevelRating[i]) then
    --    activatedStar = { "LevelSelect " .. LevelSelect, "levelselection/star_small.png", (thumbnailX + 10 + 80 * a) 
    --      + imageOffset * offsetmodifier, thumbnailY + 275}
    --    table.insert(Images, activatedStar)
    --  elseif (a > LevelRating[i]) then
    --    deactivatedStar = { "LevelSelect " .. LevelSelect, "levelselection/star_small_deactivated.png", (thumbnailX + 10 + 80 * a) 
    --      + imageOffset * offsetmodifier, thumbnailY + 275}
    --    table.insert(Images, deactivatedStar)
    --  end
    -- end
    
    -- Thumbnails
    thumbnail = { "LevelSelect " .. LevelSelect,"LEVEL " .. i,"levelselection/thumbnails/thumbnail_temp.png",
    "levelselection/thumbnails/thumbnail_temp_deactivated.png"  , thumbnailX + imageOffset * offsetmodifier, thumbnailY}  
    table.insert(Buttons, thumbnail)
    
    --Level Text
    --levelText = {"LevelSelect " .. LevelSelect, "Burbin", "Level " .. i, 52, 118, 95, 67, true, 86 + backgroundX + imageOffset * offsetmodifier, backgroundY + 380 }
    --table.insert(Text, levelText)
    
    -- Tries text
    --levelText = {"LevelSelect " .. LevelSelect, "Arial", "TRIES", 18, 0, 0, 0, false, 20 + backgroundX + imageOffset * offsetmodifier, backgroundY + 320 }
    --table.insert(Text, levelText)
    
    offsetmodifier  = offsetmodifier + 1
    
    -- Reset variables once 5 levels have been displayed
    if(i == 5) then
      LevelSelect = 1
      offsetmodifier = 0
    end 
  end
  
  -- Additional Buttons
  table.insert(Buttons,{ "LevelSelect 0", "LevelSelect 1","nextbutton.png", "nextbutton_deactivated.png",  1732, 490 })
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