
-- Event = 1, Audiofile = 2, Volume = 3
Audio = {
  { "MenuBG",         "audio/menuBG.wav",           0.8 },
  { "Pressureplate",  "audio/pressureplate_01.wav", 0.8 },
  { "Pressureplate",  "audio/pressureplate_02.wav", 0.8 },
  { "Pressureplate",  "audio/pressureplate_03.wav", 0.8 },
  { "Pressureplate",  "audio/pressureplate_04.wav", 0.8 },
  { "Pressureplate",  "audio/pressureplate_05.wav", 0.8 },
  { "Pressureplate",  "audio/pressureplate_06.wav", 0.8 },
  { "Pressureplate",  "audio/pressureplate_07.wav", 0.8 },
  { "Pressureplate",  "audio/pressureplate_08.wav", 0.8 },
  { "Pressureplate",  "audio/pressureplate_09.wav", 0.8 },
  { "Pressureplate",  "audio/pressureplate_10.wav", 0.8 },
  { "Pressureplate",  "audio/pressureplate_11.wav", 0.8 },
}



function randomSound(string)
local AudioIndexes = {}
  for i=1,#Audio do 
    if(Audio[i][1] == string) then
      table.insert(AudioIndexes, i)
    end
  end
  
  math.randomseed(os.time())
  return AudioIndexes[math.random(#AudioIndexes)]
end


function PlaySound(event)
audioindex = Audio[randomSound(event)]
print(audioindex[2]) -- Audiopath
print(audioindex[3]) -- Volume
end

PlaySound("Pressureplate")