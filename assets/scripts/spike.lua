on_collide = function(ent, ent_other, offsetX, offsetY)
	if (ent_other:isType("player")) then
		sendEvent("player_lose_health")
	end
end