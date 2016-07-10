/*
 * Copyright (C) 2008-2016 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "firelands.h"

DoorData const doorData[] =
{
    { GO_RAGNAROS_DOOR,         DATA_RAGNAROS,              DOOR_TYPE_ROOM },
    { 0,                        0,                          DOOR_TYPE_ROOM } //END
};

class instance_firelands : public InstanceMapScript
{
    public:
        instance_firelands() : InstanceMapScript(FirelandsScriptName, 720) { }

        struct instance_firelands_InstanceScript : public InstanceScript
        {
            instance_firelands_InstanceScript(InstanceMap* map) : InstanceScript(map)
            {
                SetHeaders(DataHeader);
                SetBossNumber(EncounterCount);
                LoadDoorData(doorData);

                RagnarosEmerged = false;
            }

            void OnCreatureCreate(Creature* creature) override
            {
                switch (creature->GetEntry())
                {
                    case NPC_SMOULDERING_HATCHLING:
                        // Cannot directly start attacking here as the creature is not yet on map
                        creature->m_Events.AddEvent(new DelayedAttackStartEvent(creature), creature->m_Events.CalculateTime(500));
                        break;
                    case NPC_RAGNAROS:
                        RagnarosGUID = creature->GetGUID();
                        //if (!RagnarosEmerged && !creature->IsInPhase(PHASE_DUNGEON_ALTERNATE))
                        //    creature->SetInPhase(PHASE_DUNGEON_ALTERNATE, true, true);
                        break;
                    case NPC_SULFURAS:
                        SulfurasGUID = creature->GetGUID();
                        break;
                    case NPC_CENARIUS:
                        CenariusGUID = creature->GetGUID();
                        break;
                    case NPC_HAMUUL_RUNETOTEM:
                        HamuulGUID = creature->GetGUID();
                        break;
                    case NPC_MALFURION_STORMRAGE:
                        MalfurionGUID = creature->GetGUID();
                        break;
                    case NPC_DREADFLAME:
                        DreadflameGUID = creature->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* go) override
            {
                /*if ((go->GetAreaId() == AREA_SULFURON_KEEP_COURTYARD || go->GetAreaId() == AREA_SULFURON_KEEP) && !CourtyardDefeated() && go->GetEntry() != GO_RAGNAROS_PLATFORM)
                {
                    go->SetInPhase(PHASE_DUNGEON_ALTERNATE, true, true);
                }*/

                switch(go->GetEntry())
                {
                    case GO_RAGNAROS_DOOR:
                        AddDoor(go, true);
                        break;
                    case GO_RAGNAROS_PLATFORM:
                        RagnarosPlatformGUID = go->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            void OnGameObjectRemove(GameObject* go) override
            {
                switch (go->GetEntry())
                {
                    case GO_RAGNAROS_DOOR:
                        AddDoor(go, false);
                        break;
                    default:
                        break;
                }

                InstanceScript::OnGameObjectRemove(go);
            }
            

            void SetData(uint32 type, uint32 data) override
            {
                switch (type)
                {
                    case DATA_RAGNAROS:
                    {
                        if (data != DONE || RagnarosEmerged)
                            break;

                        Creature* ragnaros = instance->GetCreature(RagnarosGUID);
                        if (!ragnaros || !ragnaros->IsInPhase(PHASE_DUNGEON_ALTERNATE) || !ragnaros->IsAIEnabled)
                            break;

                        ragnaros->SetInPhase(PHASE_DUNGEON_ALTERNATE, true, false);
                        ragnaros->AI()->DoAction(ACTION_RAGNAROS_INTRO);

                        RagnarosEmerged = true;
                        SaveToDB();
                        break;
                    }
                    default:
                        break;
                }

                SaveToDB();
            }

            uint32 GetData(uint32 type) const override
            {
                switch (type)
                {
                    case DATA_RAGNAROS:
                        if (RagnarosEmerged)
                            return DONE;

                        return NOT_STARTED;
                    default:
                        break;
                }

                return 0;
            }

			ObjectGuid GetGuidData(uint32 type) const override
            {
                switch (type)
                {
                    case NPC_RAGNAROS:
                        return RagnarosGUID;
                    case NPC_SULFURAS:
                        return SulfurasGUID;
                    case NPC_CENARIUS:
                        return CenariusGUID;
                    case NPC_HAMUUL_RUNETOTEM:
                        return HamuulGUID;
                    case NPC_MALFURION_STORMRAGE:
                        return MalfurionGUID;
                    case NPC_DREADFLAME:
                        return DreadflameGUID;

                    case GO_RAGNAROS_PLATFORM:
                        return RagnarosPlatformGUID;
                }

                return ObjectGuid::Empty;
            }

            void WriteSaveDataMore(std::ostringstream& data) override
            {
                data << RagnarosEmerged;
            }

            void ReadSaveDataMore(std::istringstream& data) override
            {
                data >> RagnarosEmerged;
            }

            private:
                bool RagnarosEmerged;

            protected:
                ObjectGuid RagnarosGUID;
                ObjectGuid SulfurasGUID;
                ObjectGuid CenariusGUID;
                ObjectGuid HamuulGUID;
                ObjectGuid MalfurionGUID;
                ObjectGuid DreadflameGUID;

                ObjectGuid RagnarosPlatformGUID;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const override
        {
            return new instance_firelands_InstanceScript(map);
        }
};

void AddSC_instance_firelands()
{
    new instance_firelands();
}
