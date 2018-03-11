
Audio = {}

 --1 = eventname, 2 = min pitch, 3 = max pitch, 4 = volume, 5 = loop
Events = {
  -- UI
  {"MAIN_BGM", 1, 1, 1, true},
  {"BUTTON_HOVER", 0.95, 1.05, 1, false},
  {"BUTTON_CLICK", 0.95, 1.05, 1, false},
  {"SELECT_LEVEL", 0.95, 1.05, 1, false},
  {"OPEN_PAUSE",  0.95, 1.05, 1, false},
  {"OPEN_RESOLUTION", 1, 1, 1, false},
  
  -- Gameplay
   {"RESET_LEVEL", 1, 1, 1, true},
  }

Audiofiles = {
  {"MAIN_BGM", "mainbmg.wav"},
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
    CreateEvent(Audiofiles[i][1], Audiofiles[i][2])
  end
end
