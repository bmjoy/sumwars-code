
-- helperfunctions for the siege of dwarfenwall and dwarfenwall itself
function sendOutHealer(unitToHeal)
	local healer = createObject("aisen_healer", "locHealerIn")
	local targetLoc = get(unitToHeal,"position");

	if(dwarfenwall.people[unitToHeal]~=nil)then
		--print(dwarfenwall.people[unitToHeal])
		local i = dwarfenwall.people[unitToHeal]
		--command chain for the hit location
		local chain = dwarfenwall.city[i]["healer"];
		for key,loc in pairs(chain) do
			if(loc == "heal")then
				--print("healing for "..unitToHeal);
				addUnitCommand(healer,"heal", unitToHeal);
			else
				--print("walking to "..loc);
				addUnitCommand(healer,"walk",loc,2.5);
			end;
		end;
	else
		-- no information available
		addUnitCommand(healer,"walk",targetLoc,2.5);
		addUnitCommand(healer,"heal",unitToHeal);
		addUnitCommand(healer,"walk","locHealerOut");
	end;
end;

function catapult(impactPos)
	createProjectile("catapult_shot",impactPos);
	setDamageValue("phys_dmg", {60,140});
	setDamageValue("power",3000);
	setDamageValue("attack",3000);
end;

function dwarfenwallPeasant1()
--peasant 1
	dwarfenwall.city[13] = {};
	dwarfenwall.city[13]["location"] = "locPeasant1";
	dwarfenwall.city[13]["important"] = false;
	dwarfenwall.city[13]["moving"] = true;
	dwarfenwall.city[13]["refname"] = "Peasant 1";
	dwarfenwall.city[13]["phase"] = 1;
	dwarfenwall.city[13]["phaseCount"] = 4;
	-- commands for phase 1
	dwarfenwall.city[13][1] = {};
	dwarfenwall.city[13][1] = {
		{'addUnitCommand(dwarfenwall.city[13]["unit"],"walk","locPeasant3",2)',0},
		{'addUnitCommand(dwarfenwall.city[13]["unit"],"walk","locPeasant4",2.5)',0}
	}
	-- commands for phase 2
	dwarfenwall.city[13][2] = {
		{'lookAtEachOther(dwarfenwall.city[11]["unit"],dwarfenwall.city[13]["unit"])',0},
		{'setObjectAnimation(dwarfenwall.city[13]["unit"],"talk",1500,true)',0},
		{'clearUnitCommands(dwarfenwall.city[13]["unit"])',1500},
		{'setObjectAnimation(dwarfenwall.city[13]["unit"],"think",1000)',1500};
		{'setObjectAnimation(dwarfenwall.city[11]["unit"],"talk",1500,true)',1500};
		{'startTimer("peasantNextPhase",5000);addTriggerVariable("id",13)',0}
	}
	-- commands for phase 3
	dwarfenwall.city[13][3]={
		{'clearUnitCommands(dwarfenwall.city[11]["unit"])',0},
		{'addUnitCommand(dwarfenwall.city[13]["unit"],"walk","locPeasant8",0.3)',0}
	}
	-- commands for phase 4
	dwarfenwall.city[13][4]={
		{'deleteAndRespawn(dwarfenwall.city[13]["unit"])',100},
		{'startTimer("peasantNextPhase",350);addTriggerVariable("id",13)',150}
	}
end;

function dwarfenwallPeasant2()
--peasant 2	
	dwarfenwall.city[14] = {};
	dwarfenwall.city[14]["location"] = "locPeasant5";
	dwarfenwall.city[14]["important"] = false;
	dwarfenwall.city[14]["moving"] = true;
	dwarfenwall.city[14]["refname"] = "Peasant 2";
	dwarfenwall.city[14]["phase"] = 1;
	dwarfenwall.city[14]["phaseCount"] = 6;
	-- commands for phase 1
	dwarfenwall.city[14][1] = {};
	dwarfenwall.city[14][1] = {
		{'addUnitCommand(dwarfenwall.city[14]["unit"],"walk","locPeasant6",2.5);',0}
	}
	-- commands for phase 2
	dwarfenwall.city[14][2] = {};
	dwarfenwall.city[14][2] = {
		{'lookAtEachOther(dwarfenwall.city[12]["unit"], dwarfenwall.city[14]["unit"])',0},
		{'setObjectAnimation(dwarfenwall.city[14]["unit"],"talk",800,true)',0},
		{'clearUnitCommands(dwarfenwall.city[14]["unit"])',1500},
		{'setObjectAnimation(dwarfenwall.city[14]["unit"],"angry",1000)',1500},
		{'setObjectAnimation(dwarfenwall.city[12]["unit"],"talk",800,true)',1500},
		{'startTimer("peasantNextPhase",4000);addTriggerVariable("id",14)',0}
	}
	-- commands for phase 3
	dwarfenwall.city[14][3] = {};
	dwarfenwall.city[14][3] = {
		{'setObjectAnimation(dwarfenwall.city[12]["unit"],"",800)',0},
		{'addUnitCommand(dwarfenwall.city[14]["unit"],"walk","locPeasant5",2)',0},
		{'addUnitCommand(dwarfenwall.city[14]["unit"],"walk","locPeasant3",1)',0},
		{'addUnitCommand(dwarfenwall.city[14]["unit"],"walk","locPeasant2",2.5)',0}
	}
	-- commands for phase 4
	dwarfenwall.city[14][4] = {};
	dwarfenwall.city[14][4] = {
		{'lookAtEachOther(dwarfenwall.city[10]["unit"], dwarfenwall.city[14]["unit"])',0},
		{'setObjectAnimation(dwarfenwall.city[14]["unit"],"talk",800,true)',0},
		{'clearUnitCommands(dwarfenwall.city[14]["unit"])',1500},
		{'setObjectAnimation(dwarfenwall.city[14]["unit"],"laugh",1000)',1500},
		{'setObjectAnimation(dwarfenwall.city[10]["unit"],"talk",800,true)',1500},
		{'clearUnitCommands(dwarfenwall.city[10]["unit"])',2800},
		{'setObjectAnimation(dwarfenwall.city[10]["unit"],"laugh",1000)',2800},
		{'setObjectAnimation(dwarfenwall.city[14]["unit"],"talk",800,true)',2800},
		{'startTimer("peasantNextPhase",6000);addTriggerVariable("id",14)',0}
	}
	-- commands for phase 5
	dwarfenwall.city[14][5] = {};
	dwarfenwall.city[14][5] = {
		{'clearUnitCommands(dwarfenwall.city[10]["unit"])',0},
		{'addUnitCommand(dwarfenwall.city[14]["unit"],"walk","locPeasant8")',0},
	}
	-- commands for phase 6
	dwarfenwall.city[14][6] = {};
	dwarfenwall.city[14][6] = {
		{'deleteAndRespawn(dwarfenwall.city[14]["unit"])',100},
		{'startTimer("peasantNextPhase",350);addTriggerVariable("id",14)',150}
	}
end;

function dwarfenwallPeasant3()
--peasant 3	
	dwarfenwall.city[15] = {};
	dwarfenwall.city[15]["location"] = "locPeasant7";
	dwarfenwall.city[15]["important"] = false;
	dwarfenwall.city[15]["moving"] = true;
	dwarfenwall.city[15]["refname"] = "Peasant 3";
	dwarfenwall.city[15]["phase"] = 1;
	dwarfenwall.city[15]["phaseCount"] = 6;
	-- commands for phase 1
	dwarfenwall.city[15][1] = {};
	dwarfenwall.city[15][1] = {
		{'addUnitCommand(dwarfenwall.city[15]["unit"],"walk","locPeasant4",2.5)',0}
	}
	-- commands for phase 2
	dwarfenwall.city[15][2] = {};
	dwarfenwall.city[15][2] = {
		{'lookAtEachOther(dwarfenwall.city[11]["unit"], dwarfenwall.city[15]["unit"])',0},
		{'setObjectAnimation(dwarfenwall.city[15]["unit"],"talk",800,true)',0},
		{'startTimer("peasantNextPhase",3000);addTriggerVariable("id",15)',0}
	}
	-- commands for phase 3
	dwarfenwall.city[15][3] = {};
	dwarfenwall.city[15][3] = {
		{'addUnitCommand(dwarfenwall.city[15]["unit"],"walk","locPeasant3",1)',0},
		{'addUnitCommand(dwarfenwall.city[15]["unit"],"walk","locPeasant5",1)',0},
		{'addUnitCommand(dwarfenwall.city[15]["unit"],"walk","locSergeant",2.5)',0}
	}
	-- commands for phase 4
	dwarfenwall.city[15][4] = {};
	dwarfenwall.city[15][4] = {
		{'lookAtEachOther(dwarfenwall.city[1]["unit"], dwarfenwall.city[15]["unit"])',0},
		{'setObjectAnimation(dwarfenwall.city[15]["unit"],"talk",800,true)',0},
		{'setObjectAnimation(dwarfenwall.city[15]["unit"],"think",1000)',2000},
		{'setObjectAnimation(dwarfenwall.city[1]["unit"],"talk",800,true)',2000},
		{'setObjectAnimation(dwarfenwall.city[1]["unit"],"",800)',5800},
		{'setObjectAnimation(dwarfenwall.city[15]["unit"],"angry",1000)',3500},
		{'setObjectAnimation(dwarfenwall.city[15]["unit"],"talk",800,true)',5800},

		{'startTimer("peasantNextPhase",8000);addTriggerVariable("id",15)',0}
	}
	-- commands for phase 5
	dwarfenwall.city[15][5] = {};
	dwarfenwall.city[15][5] = {
		{'setObjectAnimation(dwarfenwall.city[1]["unit"],"",800)',0},
		{'addUnitCommand(dwarfenwall.city[15]["unit"],"walk","locPeasant9",1)',0},
		{'addUnitCommand(dwarfenwall.city[15]["unit"],"walk","locPeasant7",2.5)',0}
	}
	-- commands for phase 6
	dwarfenwall.city[15][6] = {};
	dwarfenwall.city[15][6] = {
		{'startTimer("peasantNextPhase",3000);addTriggerVariable("id",15)',0}
	}
end;

function deleteAndRespawn(unitId)
	--print("unit: "..unitId);
	local id = dwarfenwall.people[unitId]
	local unit = dwarfenwall.city[id]["unit"];
	--print("unit: still "..unit);
	local newUnit = createObject("peasant",dwarfenwall.city[id]["location"],-90);
	--print("new unit: "..newUnit);
	dwarfenwall.city[id]["unit"]= newUnit;
	dwarfenwall.people[newUnit]=dwarfenwall.people[unit];
	dwarfenwall.people[unit]=nil;
	
	setRefName(newUnit, dwarfenwall.city[id]["refname"]);
	
	timedExecute('deleteObject('..unit..')',500);
end;

--rampart
--creates a guard at a specified position(defend_dwall_tmp.siege[index]) with a given rotation(rot)
function setGuard(index,rot)
	local guard = createObject(defend_dwall_tmp.siege[index]["subtype"], defend_dwall_tmp.siege[index]["location"],rot);
	addUnitCommand(guard,"guard",defend_dwall_tmp.siege[index]["location"],10,"secondary");
	
	defend_dwall_tmp.units[guard] = index;
	return guard;
end;

--set up the data for a unit in defend_dwall_tmp.siege
--index[int]: the siege index for that post
--location[string]: the location, were that post is
--guard_range[int]: the radius of the area, in wich the unit will move
--subtype[string]: the subtype of the unit on that post
--penalty[int]: the number of penalty-points the players will get if this unit dies
--respawn[int]: the respawntime for that unit in milliseconds
--respawnlocation[string]: the location, were the unit will respawn
--respawntrigger[string]: the trigger that will be thrown after this unit died. the default is newUnit
function createUnitIndex(index, location, guard_range, subtype, penalty, respawn, respawnlocation, respawntrigger)
	defend_dwall_tmp.siege[index] = {};
	defend_dwall_tmp.siege[index]["location"] = location;
	defend_dwall_tmp.siege[index]["guard_range"] = guard_range;
	defend_dwall_tmp.siege[index]["subtype"] = subtype;
	defend_dwall_tmp.siege[index]["penalty"] = penalty;
	
	defend_dwall_tmp.siege[index]["respawn"] = respawn;
	defend_dwall_tmp.siege[index]["respawnlocation"] = respawnlocation;
	if(respawntrigger~=nil)then
		defend_dwall_tmp.siege[index]["respawnTrigger"] = respawntrigger;
	else
		defend_dwall_tmp.siege[index]["respawnTrigger"] = "newUnit";
	end;
end;

--set up data for a spawnlocation in defend_dwall_tmp.siege
--index[int]: the siege index of that spawngroup
--location[string]: the location, were it will be spawned
--route[liste {string,string...}]: the route a spawngroup will take. It will be walked from first to last and with secondary commands
--group[string]: the name of the spawned monstergroup
function createAttackIndex(index, location, route, group)
	defend_dwall_tmp.siege[index] = {};
	defend_dwall_tmp.siege[index]["location"] = location;
	--defend_dwall_tmp.siege[index]["route"] = {};
	defend_dwall_tmp.siege[index]["route"] = route;
	defend_dwall_tmp.siege[index]["group"] = group;
end;

-- creates a spawngroup and sends it to attack
function launchAttack(index)
	--group data
	local gd = defend_dwall_tmp.siege[index];
	local group = createMonsterGroup(gd["group"],gd["location"]);
	
	for j, monster in ipairs(group)do
		for i, location in ipairs(gd["route"])do
			addUnitCommand(monster,"walk",location,1,"secondary");
		end;
	end;
end;


function skel_catapult(impactPos)
	createProjectile("catapult_skeleton",impactPos);
	setDamageValue("phys_dmg", {20,35});
	setDamageValue("fire_dmg", {30,55});
	setDamageValue("power",3000);
	setDamageValue("attack",3000);
	
	--print('createObject("catapult_skel,{'..impactPos[1]..','..impactPos[2]..'}")');
	timedExecute('createObject("catapult_skel",{'..impactPos[1]..','..impactPos[2]..'})',1000);
end;
--sends down a rain of skeletons over a time of six seconds
--number: the amount of skeletons send down
--defense ring: 45/80 bis 86/105function catapult(impactPos)
function skeletonBarrage(number)
	for i = 1, number do
		local delay = math.random(200,8000);
		local x = math.random(45,80);
		local y = math.random(86,105);
		timedExecute('skel_catapult({'..x..','..y..'})',delay);
	end;
end;

function barrage(number)
	for i = 1, number do
		local delay = math.random(200,8000);
		local x = math.random(45,80);
		local y = math.random(86,105);
		timedExecute('catapult({'..x..','..y..'})',delay);
	end;
end;

--eventfunctions
function entryLich()
	defend_dwall_tmp.lich = createObject("lich_siege","locEntryUndead");
	setRefName(defend_dwall_tmp.lich,"Nar'Asoroth")
	addUnitCommand(defend_dwall_tmp.lich,"walk","locLich",0.5);
	addUnitCommand(defend_dwall_tmp.lich,"guard","locLich",20,"secondary");
	
	defend_dwall_tmp.attendend1=createObject("necromancer_siegeDefense","locEntryUndead");
	setRefName(defend_dwall_tmp.attendend1,"Skinny Necromancer")
	timedExecute('addUnitCommand('..defend_dwall_tmp.attendend1..',"walk","locAttendend1")',1500);
	defend_dwall_tmp.attendend2=createObject("necromancer_siegeDefense","locEntryUndead");
	setRefName(defend_dwall_tmp.attendend2,"Scarfaced Necromancer")
	timedExecute('addUnitCommand('..defend_dwall_tmp.attendend2..',"walk","locAttendend2")',1500);
end;

function blockStone(stoneLoc,angle)
	local stone = createObject("siege_block",stoneLoc,angle);
	setObjectAnimation(stone,"rise",3000);
end;
function blockWall()
	local nextPos;
	local angle = 1;
	for i=1,10 do
		local delay = math.random(100,500);
		nextPos="locFissure"..i;
		local ang = 90*angle;
		timedExecute('blockStone("'..nextPos..'",'..ang..')',delay);
		angle=angle*(-1);
	end;
end;

function addImportantSpeakers()
	addSpeaker(defend_dwall_tmp.lich,"lich");
	addSpeaker(defend_dwall_tmp.attendend1,"att1");
	addSpeaker(defend_dwall_tmp.attendend2,"att2");
	addSpeaker(defend_dwall_tmp.council, "darna");
	addSpeaker(defend_dwall_tmp.general, "greif");
end;

function callElementals()
	setGuard(18,-90);
	setGuard(19,-90);
end;


function phase()
	print(defend_dwall_tmp.phase);
end;
function setDebugToFalse()
	debug["dwall_siege"]["skipIntro"]=false;
	debug["dwall_siege"]["skipWave1"]=false;
	debug["dwall_siege"]["skipWave2"]=false;
	debug["dwall_siege"]["skipWave3"]=false;
	debug["dwall_siege"]["skipWave4"]=false;
	debug["dwall_siege"]["skipFirePrep"]=false;
	debug["dwall_siege"]["skipWave5"]=false;
	debug["dwall_siege"]["skipWave6"]=false;
	debug["dwall_siege"]["skip2ndBoss"]=false;
	debug["dwall_siege"]["skipWave7"]=false;
	debug["dwall_siege"]["skipWave8"]=false;
	debug["dwall_siege"]["skip3rdBoss"]=false
	debug["dwall_siege"]["skipWave9"]=false;
	debug["dwall_siege"]["skipWave10"]=false;
	debug["dwall_siege"]["skip4thBoss"]=false;
	debug["dwall_siege"]["skipIntroNeivan"]=false;
	debug["dwall_siege"]["skipWave11"]=false;
	debug["dwall_siege"]["skipWave12"]=false;
	debug["dwall_siege"]["skip5thBoss"]=false;
	debug["dwall_siege"]["skipWave13"]=false;
	debug["dwall_siege"]["skipWave14"]=false;
	debug["dwall_siege"]["skip6thBoss"]=false;
end;
