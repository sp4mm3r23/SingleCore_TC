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
#include "ScriptedCreature.h"
#include "throne_of_the_tides.h"

enum Yells
{
    SAY_START_EVENT = 0,
    SAY_EVADE       = 1
};

enum Events
{
    EVENT_SUMMON_DRUDGE = 1
};

enum Actions
{
    ACTION_LADY_NAZJAR_START_EVENT      = 1
};

enum Points
{
    POINT_EVADE = 1
};

enum DataOther
{
    DATA_ATTACKER_GUID  = 1
};

enum Creatures
{
    NPC_THRONE_OF_THE_TIDES_TELEPORTER_1    = 51391,
    NPC_THRONE_OF_THE_TIDES_TELEPORTER_2    = 51395,
    NPC_NAZJAR_INVADER                      = 39616,
    NPC_NAZJAR_SPIRITMENDER                 = 41139,
    NPC_DEEP_MURLOC_DRUDGE                  = 39960,
    NPC_TEMPEST_WITCH                       = 40634
};

const Position eventPos[7] = 
{
    { -121.93f, 807.15f, 797.19f, 3.13f  },
    { -122.00f, 799.22f, 797.13f, 3.13f  },
    { -97.83f, 798.27f, 797.04f, 3.13f   },
    { -98.13f, 806.96f, 797.04f, 3.13f   },
    { -65.19f, 808.50f, 796.96f, 3.13f   },
    { -66.57f, 798.02f, 796.87f, 3.13f   },
    { 13.67f, 802.34f, 806.43f, 0.12f    }
};


class at_tott_lady_nazjar_event : public AreaTriggerScript
{
public:
    at_tott_lady_nazjar_event() : AreaTriggerScript("at_tott_lady_nazjar_event") { }

   bool OnTrigger(Player* player, AreaTriggerEntry const* /*areaTrigger*/, bool /*entered*/) override
    {
        if (InstanceScript* instance = player->GetInstanceScript())
        {
            if (instance->GetData(DATA_LADY_NAZJAR_EVENT) != DONE
                && instance->GetBossState(DATA_COMMANDER_ULTHOK) != DONE)
            {
                instance->SetData(DATA_LADY_NAZJAR_EVENT, DONE);
                if (Creature* LadyNazjarEvent = ObjectAccessor::GetCreature(*player, instance->GetGuidData(DATA_LADY_NAZJAR_EVENT)))
                    LadyNazjarEvent->AI()->DoAction(ACTION_LADY_NAZJAR_START_EVENT);
            }
        }
        return true;
    }
};

const Position teleporterPos[2] = 
{
    { -560.25f, 819.19f, 245.28f, 5.20f }, // entrance;
    { -14.72f, 796.57f, 808.12f, 1.99f  }, // upper
};

class npc_throne_of_the_tides_teleporter : public CreatureScript
{
    public:
        npc_throne_of_the_tides_teleporter() : CreatureScript("npc_throne_of_the_tides_teleporter"){ }

        bool OnGossipHello(Player* player, Creature* creature) override
        {
            if (player->IsInCombat())
                return false;
            switch (creature->GetEntry())
            {
            case NPC_THRONE_OF_THE_TIDES_TELEPORTER_1:
                    player->NearTeleportTo(teleporterPos[1].GetPositionX(),
                        teleporterPos[1].GetPositionY(),
                        teleporterPos[1].GetPositionZ(),
                        teleporterPos[1].GetOrientation());
                    return false;
                break;
            case NPC_THRONE_OF_THE_TIDES_TELEPORTER_2:
                player->NearTeleportTo(teleporterPos[0].GetPositionX(),
                    teleporterPos[0].GetPositionY(),
                    teleporterPos[0].GetPositionZ(),
                    teleporterPos[0].GetOrientation());
                break;
            }
            return false;
        }
};

class npc_lady_nazjar_event_39959 : public CreatureScript
{
    public:
        npc_lady_nazjar_event_39959() : CreatureScript("npc_lady_nazjar_event_39959") { }

        struct npc_lady_nazjar_event_39959AI : public ScriptedAI
        {
            npc_lady_nazjar_event_39959AI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
            }

            void Reset() override
            {
                if (!instance)
                    return;

                if(instance->GetData(DATA_LADY_NAZJAR_EVENT) == DONE)
                    me->DespawnOrUnsummon();

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->SetReactState(REACT_PASSIVE);
                me->setActive(true);
                bEvade = false;
                events.Reset();
            }

            void MovementInform(uint32 type, uint32 id) override
            {
                if (type == POINT_MOTION_TYPE)
                {
                    if (id == POINT_EVADE)
                        me->DespawnOrUnsummon();
                }
            }

            void DoAction(int32 action) override
            {
                if (action == ACTION_LADY_NAZJAR_START_EVENT)
                {
                    Talk(SAY_START_EVENT);
                    me->SummonCreature(NPC_NAZJAR_INVADER, eventPos[0]);
                    me->SummonCreature(NPC_NAZJAR_INVADER, eventPos[1]);
                    me->SummonCreature(NPC_NAZJAR_INVADER, eventPos[2]);
                    me->SummonCreature(NPC_TEMPEST_WITCH, eventPos[3]);
                    me->SummonCreature(NPC_NAZJAR_SPIRITMENDER, eventPos[4]);
                    me->SummonCreature(NPC_NAZJAR_SPIRITMENDER, eventPos[5]);
                }
            }

            void UpdateAI(uint32 /*diff*/) override
            {
                if(!instance)
                    return;

                if (me->SelectNearestTarget(50.0f) && !bEvade)
                {
                    bEvade = true;
                    Talk(SAY_EVADE);
                    me->GetMotionMaster()->MovePoint(POINT_EVADE, eventPos[6]);
                }
            }

        private:
            InstanceScript* instance;
            EventMap events;
            bool bEvade;
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_lady_nazjar_event_39959AI(creature);
        }
};

class go_totd_defense_system : public GameObjectScript
{
public:
    go_totd_defense_system() : GameObjectScript("go_totd_defense_system") { }

    bool OnGossipHello(Player* /*player*/, GameObject* go) override
    {
        if(go->GetInstanceScript())
        {
            Map::PlayerList const &PlayerList = go->GetMap()->GetPlayers();

            if (!PlayerList.isEmpty())
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    i->GetSource()->SendCinematicStart(169);
        }
        return false;
    }
};

void AddSC_throne_of_the_tides()
{
    new npc_lady_nazjar_event_39959();
    new go_totd_defense_system();
    new npc_throne_of_the_tides_teleporter();
    //new at_tott_lady_nazjar_event();
}
