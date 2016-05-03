move_left = function(ent)
	ent:setImpulse(ent:getImpulseX() - (ent:getAcceleration() * ent:getDeltaTime()), ent:getImpulseY())
end

move_right = function(ent)
	ent:setImpulse(ent:getImpulseX() + (ent:getAcceleration() * ent:getDeltaTime()), ent:getImpulseY())
end

move_jump = function(ent)
	ent:setImpulse(ent:getImpulseX(), ent:getImpulseY() - 150 - (ent:getAcceleration() * ent:getDeltaTime()))
end

on_collide = function(ent, ent_other, offsetX, offsetY)
	if (math.abs(offsetX) > math.abs(offsetY)) then
		-- if Y is shallow axis, the entity will move to the bottom/top of the object
		ent:setPos(ent:getPosX(), ent:getPosY() + offsetY)
	elseif (math.abs(offsetX) < math.abs(offsetY)) then
		-- if X is shallow axis, the entity will move to the side of the object
		ent:setPos(ent:getPosX() + offsetX, ent:getPosY())
	else
		-- equal on both sides. Entity will move in diagonal direction
	   ent:setPos(ent:getPosX() + offsetX, ent:getPosY() + offsetY)
   end
end