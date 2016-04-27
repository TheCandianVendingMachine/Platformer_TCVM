move_left = function(ent)
	ent:setImpulse(-250, ent:getImpulseY())
	ent:test()
end

move_right = function(ent)
	ent:setImpulse(250, ent:getImpulseY())
	ent:test()
end

move_stop = function(ent)
	ent:setImpulse(0, ent:getImpulseY())
end

move_jump = function(ent)
	ent:setImpulse(ent:getImpulseX(), -500)
	ent:test()
end