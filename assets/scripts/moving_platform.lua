on_collide = function(ent, ent_other, offsetX, offsetY)
   if (ent_other:isType("player")) then
	ent:logToConsole("test")
	ent:setImpulse(ent:getImpulseX(), ent_other:getImpulseY())
   end
end