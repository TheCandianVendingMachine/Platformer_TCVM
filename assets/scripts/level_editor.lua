setSize = function(entity, mousePosX, mousePosY)
	entSizeX = entity:getSizeX()
	entSizeY = entity:getSizeY()

	entPosX = entity:getPosX()
	entPosY = entity:getPosY()

	entity:printToConsole(entPosX)
	
	sizeX = mousePosX - entPosX
	sizeY = mousePosX - entPosY

	entity:printToConsole(sizeX)
	entity:printToConsole(sizeY)
	
	entity:setSize(sizeX / entSizeX, sizeY / entSizeY)
end