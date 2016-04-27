move_left = function(ent)
	ent:setImpulse(ent:getImpulseX() - (5000 * ent:getDeltaTime()), ent:getImpulseY())
end

move_right = function(ent)
	ent:setImpulse(ent:getImpulseX() + (500 * ent:getDeltaTime()), ent:getImpulseY())
end

move_stop = function(ent)
	--ent:setImpulse(0, ent:getImpulseY())
	ent:test()
end

move_jump = function(ent)
	ent:setImpulse(ent:getImpulseX(), ent:getImpulseY() - 500)
end