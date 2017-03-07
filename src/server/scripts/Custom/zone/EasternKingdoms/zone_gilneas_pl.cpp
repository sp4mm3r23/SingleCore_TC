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

#include "ScriptPCH.h"
#include "Cell.h"
#include "CellImpl.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "GameObjectAI.h"
#include "GameObject.h"
#include "GridNotifiers.h"
#include "PassiveAI.h"
#include "Pet.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "ScriptedFollowerAI.h"
#include "Unit.h"
#include "Vehicle.h"
#include "zone_gilneas_pl.h"

//34864
class npc_gilneas_city_guard_gate : public CreatureScript
{
public:
    npc_gilneas_city_guard_gate() : CreatureScript("npc_gilneas_city_guard_gate") { }

    struct npc_gilneas_city_guard_gateAI : public ScriptedAI
    {
        npc_gilneas_city_guard_gateAI(Creature* creature) : ScriptedAI(creature)  { }

        void Reset() override
        {
            if (me->GetDistance2d(-1430.47f, 1345.55f) < 10.0f)
                events.ScheduleEvent(EVENT_START_TALK_WITH_CITIZEN, urand(10000, 30000));
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_START_TALK_WITH_CITIZEN:
                    {
                        citicenGUID = GetRandomCitizen();
                        emote = RAND(EMOTE_STATE_COWER, EMOTE_STATE_TALK, EMOTE_ONESHOT_CRY, EMOTE_STATE_SPELL_PRECAST, EMOTE_STATE_EXCLAIM);
                        say = urand(0, 2);
                        if (Creature* npc = ObjectAccessor::GetCreature(*me, citicenGUID))
                            npc->HandleEmoteCommand(emote);
                        events.ScheduleEvent(EVENT_TALK_WITH_CITIZEN_1, urand(1200, 2000));
                        break;
                    }
                    case EVENT_TALK_WITH_CITIZEN_1:
                    {
                        if (Creature* npc = ObjectAccessor::GetCreature(*me, citicenGUID))
                            npc->AI()->Talk(say);
                        events.ScheduleEvent(EVENT_TALK_WITH_CITIZEN_2, 5000);
                        break;
                    }
                    case EVENT_TALK_WITH_CITIZEN_2:
                    {
                        Talk(say);
                        events.ScheduleEvent(EVENT_TALK_WITH_CITIZEN_3, 5000);
                        break;
                    }
                    case EVENT_TALK_WITH_CITIZEN_3:
                    {
                        if (Creature* npc = ObjectAccessor::GetCreature(*me, citicenGUID))
                            npc->HandleEmoteCommand(EMOTE_STATE_NONE);
                        events.ScheduleEvent(EVENT_START_TALK_WITH_CITIZEN, urand(5000, 30000));
                        break;
                    }
                }
            }

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }

        void FillCitizenList()
        {
            listOfCitizenGUID.clear();
            std::list<Creature*> listOfCitizen;
            me->GetCreatureListWithEntryInGrid(listOfCitizen, NPC_PANICKED_CITIZEN_GATE, 35.0f);
            for (std::list<Creature*>::iterator itr = listOfCitizen.begin(); itr != listOfCitizen.end(); ++itr)
                listOfCitizenGUID.push_back((*itr)->GetGUID());
        }

        ObjectGuid GetRandomCitizen()
        {
            if (listOfCitizenGUID.empty())
                FillCitizenList();
            uint8 rol = urand(0, listOfCitizenGUID.size() - 1);
            std::list<ObjectGuid>::iterator itr = listOfCitizenGUID.begin();
            std::advance(itr, rol);
            return (*itr);
        }

    private:
        std::list<ObjectGuid> listOfCitizenGUID;
        EventMap events;
        uint8 say;
        uint8 emote;
        ObjectGuid citicenGUID;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_gilneas_city_guard_gateAI(creature);
    }
};

// 34850
class npc_prince_liam_greymane : public CreatureScript
{
public:
    npc_prince_liam_greymane() : CreatureScript("npc_prince_liam_greymane") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest) override
    {
        if (quest->GetQuestId() == QUEST_LOCKDOWN)
            if (Creature* citizen = creature->FindNearestCreature(NPC_PANICKED_CITIZEN, 20.0f))
                citizen->AI()->Talk(0);

        return true;
    }

    struct npc_prince_liam_greymaneAI : public ScriptedAI
    {
        npc_prince_liam_greymaneAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override
        {            
            events.RescheduleEvent(EVENT_COUNT_COOLDOWN, 1000);
        }

        void MoveInLineOfSight(Unit* who) override
        {
            if (Player* player = who->ToPlayer())
                if (player->GetDistance2d(me) < 15.0f)
                    if (cdList.find(player->GetGUID()) == cdList.end())
                        if (player->GetQuestStatus(QUEST_LOCKDOWN) != QUEST_STATUS_REWARDED)
                            cdList.insert(std::make_pair(player->GetGUID(), 70));
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_COUNT_COOLDOWN:
                    {
                        for (std::map<ObjectGuid, int32>::iterator itr = cdList.begin(); itr != cdList.end(); ++itr)
                            if ((itr)->second > 0)
                                --(itr)->second;

                        if (!playerGUID)
                            for (std::map<ObjectGuid, int32>::iterator itr = cdList.begin(); itr != cdList.end(); ++itr)
                                if ((itr)->second == 0)
                                    if (Player* player = ObjectAccessor::GetPlayer(*me, (itr)->first))
                                        if (player->GetDistance2d(me) < 15.0f)
                                        {
                                            --(itr)->second;
                                            playerGUID = player->GetGUID();
                                            events.ScheduleEvent(EVENT_MASTER_RESET, 120000);
                                            events.ScheduleEvent(EVENT_START_TALK_TO_GUARD, 1000);
                                        }

                        for (std::map<ObjectGuid, int32>::iterator itr = cdList.begin(); itr != cdList.end();)
                            if (ToBeDeleted(itr))
                                cdList.erase(itr++);
                            else
                                itr++;

                        events.ScheduleEvent(EVENT_COUNT_COOLDOWN, 1000);
                        break;
                    }
                    case EVENT_START_TALK_TO_GUARD:
                    {
                        if (Player* player = ObjectAccessor::GetPlayer(*me, playerGUID))
                            Talk(0, player);
                        events.ScheduleEvent(EVENT_TALK_TO_GUARD_1, 15000);
                        break;
                    }
                    case EVENT_TALK_TO_GUARD_1:
                    {
                        if (Player* player = ObjectAccessor::GetPlayer(*me, playerGUID))
                            Talk(1, player);
                        events.ScheduleEvent(EVENT_TALK_TO_GUARD_2, 18000);
                        break;
                    }
                    case EVENT_TALK_TO_GUARD_2:
                    {
                        if (Player* player = ObjectAccessor::GetPlayer(*me, playerGUID))
                            Talk(2, player);
                        events.ScheduleEvent(EVENT_TALK_TO_GUARD_3, 25000);
                        break;
                    }
                    case EVENT_TALK_TO_GUARD_3:
                    {
                        events.ScheduleEvent(EVENT_MASTER_RESET, 1000);
                        break;
                    }
                    case EVENT_MASTER_RESET:
                    {
                        Reset();
                        break;
                    }
                }
            }

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }

        bool ToBeDeleted(std::map<ObjectGuid, int32>::iterator itr)
        {
            if (itr->second < 0)
                if (Player* player = ObjectAccessor::GetPlayer(*me, (itr)->first))
                    if (player->GetQuestStatus(QUEST_LOCKDOWN) == QUEST_STATUS_REWARDED)
                        return true;
            return false;
        }

    private:
        EventMap events;
        std::map<ObjectGuid, int32> cdList;
        ObjectGuid playerGUID;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_prince_liam_greymaneAI(creature);
    }
};

// update player phase 1 to phase 2
class npc_lieutenant_walden : public CreatureScript
{
    public:
        npc_lieutenant_walden() : CreatureScript("npc_lieutenant_walden") { }

    private:
        bool OnQuestReward(Player* player, Creature* creature, Quest const* quest, uint32 /*opt*/) override
        {
            if (quest->GetQuestId() == QUEST_LOCKDOWN)
            {
                player->CastSpell(player, SPELL_PHASE_QUEST_ZONE_SPECIFIC_01, false);
                player->CastSpell(player, SPELL_UPDATE_PHASE_SHIFT, false);
                player->SaveToDB();
            }
            return false;

        }
};

void AddSC_gilneas_pl()
{
    new npc_gilneas_city_guard_gate();
    new npc_prince_liam_greymane();
    new npc_lieutenant_walden();
}
