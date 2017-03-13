/*
 * Copyright (C) 2016-2017 ProjectLegion <https://bitbucket.org/projectlegion/projectlegion/>
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
#include "throne_of_the_tides.h"

static DoorData const doorData[] = 
{
    {GO_LADY_NAZJAR_DOOR,         DATA_LADY_NAZJAR,        DOOR_TYPE_ROOM},
    {GO_LADY_NAZJAR_DOOR,         DATA_COMMANDER_ULTHOK,   DOOR_TYPE_ROOM},
    {GO_COMMANDER_ULTHOK_DOOR,    DATA_COMMANDER_ULTHOK,   DOOR_TYPE_ROOM},
    {GO_ERUNAK_STONESPEAKER_DOOR, DATA_MINDBENDER_GHURSHA, DOOR_TYPE_ROOM},
    {GO_OZUMAT_DOOR,              DATA_OZUMAT,             DOOR_TYPE_ROOM},
    {0,                           0,                       DOOR_TYPE_ROOM}
};

class instance_throne_of_the_tides : public InstanceMapScript
{
public:
    instance_throne_of_the_tides() : InstanceMapScript(TotTScriptName, 643) { }

    struct instance_throne_of_the_tides_InstanceMapScript : public InstanceScript
    {
        instance_throne_of_the_tides_InstanceMapScript(Map* map) : InstanceScript(map)
        {
            SetBossNumber(EncounterCount);
            LoadDoorData(doorData);

            memset(m_uiEvents, 0, sizeof(m_uiEvents));
        }

        void OnPlayerEnter(Player* player) override
        {
            if (!TeamInInstance)
                TeamInInstance = player->GetTeam();

        }

        void OnCreatureCreate(Creature* creature) override
        {
            if (!TeamInInstance)
            {
                Map::PlayerList const &players = instance->GetPlayers();
                if (!players.isEmpty())
                    if (Player* player = players.begin()->GetSource())
                        TeamInInstance = player->GetTeam();
            }

            switch(creature->GetEntry())
            {
            case NPC_LADY_NAZJAR_EVENT:
                LadyNazjarEventGUID = creature->GetGUID();
                break;
            case NPC_LADY_NAZJAR:
                LadyNazjarGUID = creature->GetGUID();
                break;
            case NPC_COMMANDER_ULTHOK:
                CommanderUlthokGUID = creature->GetGUID();
                if (GetData(DATA_COMMANDER_ULTHOK_EVENT) == DONE)
                    creature->SetPhaseMask(PHASEMASK_NORMAL, true);
                else
                    creature->SetPhaseMask(2, true);
                break;
            case NPC_ERUNAK_STONESPEAKER:
                ErunakStonespeakerGUID = creature->GetGUID();
                break;
            case NPC_MINDBENDER_GHURSHA:
                MindbenderGhurshaGUID = creature->GetGUID();
                break;
            case NPC_OZUMAT:
                OzumatGUID = creature->GetGUID();
                break;
            case NPC_NEPTULON:
                NeptulonGUID = creature->GetGUID();
                break;
            case NPC_CAPTAIN_TAYLOR:
                if (TeamInInstance == HORDE)
                    creature->UpdateEntry(NPC_LEGIONNAIRE_NAZGRIM);
                break;
            case NPC_LEGIONNAIRE_NAZGRIM:
                if (TeamInInstance == ALLIANCE)
                    creature->UpdateEntry(NPC_CAPTAIN_TAYLOR);
                break;
            }
        }

        void OnGameObjectCreate(GameObject* go) override
        {
            switch(go->GetEntry())
            {
            case GO_CORALES:
                CoralesGUID = go->GetGUID();
                if (GetBossState(DATA_LADY_NAZJAR) == DONE)
                {
                    go->SetGoState(GO_STATE_ACTIVE_ALTERNATIVE);
                    go->SetPhaseMask(2, true);
                }
                break;
            case GO_LADY_NAZJAR_DOOR:
                LadyNazjarDoorGUID = go->GetGUID();
                AddDoor(go, true);
                break;
            case GO_COMMANDER_ULTHOK_DOOR:
                CommanderUlthokDoorGUID = go->GetGUID();
                AddDoor(go, true);
                break;
            case GO_ERUNAK_STONESPEAKER_DOOR:
                MindebenderGhurshaDoorGUID = go->GetGUID();
                AddDoor(go, true);
                break;
            case GO_OZUMAT_DOOR:
                OzumatDoorGUID = go->GetGUID();
                AddDoor(go, true);
                break;
            case GO_CONTROL_SYSTEM:
                ControlSystemGUID = go->GetGUID();
                break;
            case GO_TENTACLE_RIGHT:
                TentacleRightGUID = go->GetGUID();
                if (GetBossState(DATA_COMMANDER_ULTHOK) == DONE)
                    go->SetPhaseMask(2, true);
                break;
            case GO_TENTACLE_LEFT:
                TentacleLeftGUID = go->GetGUID();
                if (GetBossState(DATA_COMMANDER_ULTHOK) == DONE)
                    go->SetPhaseMask(2, true);
                break;
            case GO_INVISIBLE_DOOR_1:
                InvisibleDoor1GUID = go->GetGUID();
                if (GetBossState(DATA_COMMANDER_ULTHOK) == DONE)
                    go->SetPhaseMask(2, true);
                break;
            case GO_INVISIBLE_DOOR_2:
                InvisibleDoor2GUID = go->GetGUID();
                if (GetBossState(DATA_COMMANDER_ULTHOK) == DONE)
                    go->SetPhaseMask(2, true);
                break;
            case GO_NEPTULON_CACHE:
            case GO_NEPTULON_CACHE_H:
                NeptulonCache = go->GetGUID();
                break;
            }
        }

        void OnGameObjectRemove(GameObject* go) override
        {
            switch (go->GetEntry())
            {
            case GO_LADY_NAZJAR_DOOR:
            case GO_COMMANDER_ULTHOK_DOOR:
            case GO_ERUNAK_STONESPEAKER_DOOR:
            case GO_OZUMAT_DOOR:
                AddDoor(go, false);
                break;
            }
        }

        void SetData(uint32 type, uint32 data) override
        {
            switch (type)
            {
                case DATA_LADY_NAZJAR_EVENT:
                    m_uiEvents[0] = data;
                    break;
                case DATA_COMMANDER_ULTHOK_EVENT:
                    m_uiEvents[1] = data;
                    break;
                case DATA_NEPTULON_EVENT:
                    m_uiEvents[2] = data;
            }
            if (data == DONE)
                SaveToDB();
        }

        uint32 GetData(uint32 type) const override
        {
            switch (type)
            {
                case DATA_LADY_NAZJAR_EVENT:
                    return m_uiEvents[0];
                case DATA_COMMANDER_ULTHOK_EVENT:
                    return m_uiEvents[1];
                case DATA_NEPTULON_EVENT:
                    return m_uiEvents[2];
            }
            return 0;
        }

        ObjectGuid GetGuidData(uint32 type) const override
        {
            switch(type)
            {
            case DATA_LADY_NAZJAR:
                return LadyNazjarGUID;
            case DATA_COMMANDER_ULTHOK:
                return CommanderUlthokGUID;
            case DATA_ERUNAK_STONESPEAKER:
                return ErunakStonespeakerGUID;
            case DATA_MINDBENDER_GHURSHA:
                return MindbenderGhurshaGUID;
            case DATA_OZUMAT:
                return OzumatGUID;
            case DATA_NEPTULON:
                return NeptulonGUID;
            case DATA_LADY_NAZJAR_EVENT:
                return LadyNazjarEventGUID;
            case DATA_CORALES:
                return CoralesGUID;
            }

            return ObjectGuid::Empty;
        }

        bool SetBossState(uint32 type, EncounterState state)
        {
            if (!InstanceScript::SetBossState(type, state))
                return false;

            switch (type)
            {
            case DATA_LADY_NAZJAR:
                break;
            case DATA_COMMANDER_ULTHOK:
                if (state == DONE)
                {
                    if (GameObject* TentacleRight = instance->GetGameObject(TentacleRightGUID))
                        TentacleRight->SetPhaseMask(2, true);
                    if (GameObject* TentacleLeft = instance->GetGameObject(TentacleLeftGUID))
                        TentacleLeft->SetPhaseMask(2, true);
                    if (GameObject* InvisibleDoor1 = instance->GetGameObject(InvisibleDoor1GUID))
                        InvisibleDoor1->SetPhaseMask(2, true);
                    if (GameObject* InvisibleDoor2 = instance->GetGameObject(InvisibleDoor2GUID))
                        InvisibleDoor2->SetPhaseMask(2, true);
                }
                break;
            case DATA_MINDBENDER_GHURSHA:
                break;
            case DATA_OZUMAT:
                if (state == DONE)
                    DoRespawnGameObject(NeptulonCache, DAY);
                break;
             }
            return true;
        }

        std::string GetSaveData() override
        {
            OUT_SAVE_INST_DATA;

            std::string str_data;

            std::ostringstream saveStream;
            saveStream << "T o t T " << m_uiEvents[0] << " " << m_uiEvents[1] << " " << m_uiEvents[2] << " ";

            str_data = saveStream.str();

            OUT_SAVE_INST_DATA_COMPLETE;
            return str_data;
        }

        void Load(const char* in) override
        {
            if (!in)
            {
                OUT_LOAD_INST_DATA_FAIL;
                return;
            }

            OUT_LOAD_INST_DATA(in);

            char dataHead1, dataHead2, dataHead3, dataHead4;

            std::istringstream loadStream(in);
            loadStream >> dataHead1 >> dataHead2 >> dataHead3 >> dataHead4;

            if (dataHead1 == 'T' && dataHead2 == 'o' && dataHead3 == 't' && dataHead4 == 'T')
            {
                for (uint8 i = 0; i < EncounterCount; ++i)
                {
                    uint32 tmpState;
                    loadStream >> tmpState;
                    if (tmpState == IN_PROGRESS || tmpState > SPECIAL)
                        tmpState = NOT_STARTED;
                    SetBossState(i, EncounterState(tmpState));
                }
                for (uint8 i = 0; i < 3; ++i)
                {
                    uint32 tmpEvent;
                    loadStream >> tmpEvent;
                    if (tmpEvent == IN_PROGRESS || tmpEvent > SPECIAL)
                        tmpEvent = NOT_STARTED;
                    m_uiEvents[i] = tmpEvent;
                }

            } else OUT_LOAD_INST_DATA_FAIL;

            OUT_LOAD_INST_DATA_COMPLETE;
        }

        private:
            ObjectGuid LadyNazjarGUID;
            ObjectGuid CommanderUlthokGUID;
            ObjectGuid ErunakStonespeakerGUID;
            ObjectGuid MindbenderGhurshaGUID;
            ObjectGuid OzumatGUID;
            ObjectGuid NeptulonGUID;
            ObjectGuid LadyNazjarEventGUID;
            ObjectGuid CoralesGUID;
            ObjectGuid LadyNazjarDoorGUID;
            ObjectGuid CommanderUlthokDoorGUID;
            ObjectGuid MindebenderGhurshaDoorGUID;
            ObjectGuid OzumatDoorGUID;
            ObjectGuid ControlSystemGUID;
            ObjectGuid TentacleRightGUID;
            ObjectGuid TentacleLeftGUID;
            ObjectGuid InvisibleDoor1GUID;
            ObjectGuid InvisibleDoor2GUID;
            ObjectGuid NeptulonCache;

            uint32 TeamInInstance;
            uint32 m_uiEvents[3];
    };

    InstanceScript* GetInstanceScript(InstanceMap* map) const override
    {
        return new instance_throne_of_the_tides_InstanceMapScript(map);
    }
};

void AddSC_instance_throne_of_the_tides()
{
    new instance_throne_of_the_tides();
}
