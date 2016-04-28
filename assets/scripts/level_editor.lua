setSize = function(entity, mousePosX, mousePosY)
	entSizeX = entity:getSizeX()
	entSizeY = entity:getSizeY()

	entPosX = entity:getPosX()
	entPosY = entity:getPosY()

	sizeX = mousePosX - entPosX
	sizeY = mousePosX - entPosY

	entity:setSize(sizeX / entSizeX, sizeY = entSizeY)
end