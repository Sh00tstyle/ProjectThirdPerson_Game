
Audio = {}

 --1 = eventname, 2 = min pitch, 3 = max pitch, 4 = volume, 5 = loop
Events = {
  -- UI
  {"MAIN_BGM", 1, 1, 32, true},
  {"BUTTON_CLICK", 1, 1, 12, false},
  {"SELECT_LEVEL", 1, 1, 16, false},
  {"OPEN_PAUSE", 1, 1, 12, false},
  {"OPEN_RESOLUTION", 1, 1, 12, false},
  {"BUTTON_HOVER", 0.8, 0.85, 8, false},
  {"CHANGE_COLOR", 1, 1, 12, false},
  

  -- Player
  {"MOVE_SNAIL", 0.96, 1.04, 15, false},
  {"BLOCK_SNAIL", 0.96, 1.06, 30, false},
  
  -- Gameplay
  {"RESET_LEVEL", 1, 1, 25, false},
  {"HINT", 1, 1, 45, false},
  {"TILE_MOVE", 1, 1, 25, false},
  --{"START_LEVEL", 1, 1, 1, false},
  {"END_LEVEL", 1, 1, 10, false},
  {"BGM_LEVEL", 1, 1, 40, true},
  {"ACTIVATE_PLATE", 0.95, 1.05, 42, false},
  
  
  }

Audiofiles = {
  -- UI
  {"HINT",              "ui/hint.wav"},
  {"BUTTON_HOVER",      "ui/button_click.wav"},
  {"CHANGE_COLOR",      "gameplay/color_switch.wav"}, 
  {"SELECT_LEVEL",      "ui/select_level.wav"},
  {"BUTTON_CLICK",      "ui/button_click.wav"},
  {"SELECT_LEVEL",      "ui/select_level.wav"},
  {"OPEN_PAUSE",        "ui/select_level.wav"},
  {"OPEN_RESOLUTION",   "ui/select_level.wav"},
  {"MAIN_BGM",          "ui/mainmenu.wav"},
  
  -- Player
  {"BLOCK_SNAIL",       "player/player_block.wav"},
  {"MOVE_SNAIL", 		"player/slime/Snail_Move_Medium_SFX_01.wav"},
  {"MOVE_SNAIL", 		"player/slime/Snail_Move_Medium_SFX_02.wav"},
  {"MOVE_SNAIL", 		"player/slime/Snail_Move_Medium_SFX_03.wav"},
  {"MOVE_SNAIL", 		"player/slime/Snail_Move_Medium_SFX_04.wav"},

  
  -- Gameplay
  {"BGM_LEVEL",         "gameplay/bgm_level.wav"},
  {"END_LEVEL",         "gameplay/level_end.wav"},
  {"RESET_LEVEL",       "gameplay/reset.wav"},
  --{"START_LEVEL",       "gameplay/level_end.wav"},
  {"TILE_MOVE",         "gameplay/pressureplate_move.ogg"},
  {"ACTIVATE_PLATE",    "gameplay/pressureplate_open.wav"},
}
  
function Audio.init() 
--[[
  EVENTS:
  
  UI:
- Main menu music          MAIN_BGM
- Button hover             BUTTON_HOVER
- Button click             BUTTON_CLICK
- Select level click       SELECT_LEVEL
- Open pause menu          OPEN_PAUSE
- Resolution               OPEN_RESOLUTION


Gameplay:
- Reset level               RESET_LEVEL
- Pressure plate activate   ACTIVATE_PLATE
- Activatable tile move     TILE_MOVE
- Color change              CHANGE_COLOR
- Start level               START_LEVEL
- Reach level end           END_LEVEL
- Background music          BGM_LEVEL


Snail:
- Move                      MOVE_SNAIL
- Blocked Path(bearbeitet)  BLOCK_SNAIL
- Main menu music          MAIN_BGM 
- Button hover             BUTTON_HOVER 
- Button click             BUTTON_CLICK 
- Select level click       SELECT_LEVEL 
- Open pause menu          OPEN_PAUSE 
- Resolution               OPEN_RESOLUTION 


Gameplay:
- Reset level               RESET_LEVEL 
- Pressure plate activate   ACTIVATE_PLATE 
- Activatable tile move     TILE_MOVE 
- Color change              CHANGE_COLOR 
- Start level               START_LEVEL 
- Reach level end           END_LEVEL 
- Background music          BGM_LEVEL 


Snail:
- Move                      MOVE_SNAIL 
- Blocked Path              BLOCK_SNAIL 
--]]

  -- create Events
  for i=1,#Events do 
    CreateEvent(Events[i][1], Events[i][2], Events[i][3], Events[i][4], Events[i][5])
  end
    
  -- Add Audiofiles to Events
  for i=1,#Audiofiles do 
    AddSound(Audiofiles[i][1], Audiofiles[i][2])
  end
end
