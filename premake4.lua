dofile("framework/framework.lua")

SetupSolution("3DMath")

local dirList = os.matchdirs("Chapter*");

for i, theDir in ipairs(dirList) do
    local filename = string.format("%s/chapter.lua", theDir);
	if(os.isfile(filename)) then
		dofile(filename)
	end
end

