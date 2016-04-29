move_left = function(ent)
	ent:setImpulse(ent:getImpulseX() - (ent:getAcceleration() * ent:getDeltaTime()), ent:getImpulseY())
end

move_right = function(ent)
	ent:setImpulse(ent:getImpulseX() + (ent:getAcceleration() * ent:getDeltaTime()), ent:getImpulseY())
end

move_stop = function(ent)
	--ent:setImpulse(0, ent:getImpulseY())
	ent:test()
end

move_jump = function(ent)
	ent:setImpulse(ent:getImpulseX(), ent:getImpulseY() - 100 - (ent:getAcceleration() * ent:getDeltaTime()))
end