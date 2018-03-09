
Audio = {}

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

--]]

--filepath: mge/assets/audio
  
  
  --1 = eventname, 2 = min pitch, 3 = max pitch, 4 = volume, 5 = loop
  CreateEvent("MAIN_BGM", 1, 1, 20, true)
  CreateEvent("MOVE_SNAIL", 0.8, 1.2, 20, false)
  
  -- 1 = eventname, 2 = filename
  AddSound("MAIN_BGM", "Gwe_Gwe_Gwe.wav")
  AddSound("MOVE_SNAIL", "player_move.wav")
end