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

// 35660 
class npc_rampaging_worgen_phase_1 : public CreatureScript
{
public:
    npc_rampaging_worgen_phase_1() : CreatureScript("npc_rampaging_worgen_phase_1") { }

    struct npc_rampaging_worgen_phase_1AI : public ScriptedAI
    {
        npc_rampaging_worgen_phase_1AI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override
        {           
            enrage = false;
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
        }

        void JustDied(Unit* /*killer*/) override
        {
            me->DespawnOrUnsummon(1000);
        }

        void MovementInform(uint32 type, uint32 id) override
        {
            if (type == POINT_MOTION_TYPE)
                switch (id)
            {
                case MOVE_TO_START_POSITION:
                {
                    events.ScheduleEvent(EVENT_MOVE_TO_LIAM, 500);
                    break;
                }
                case MOVE_TO_PRINCE_LIAM:
                {
                    events.ScheduleEvent(EVENT_ATTACK_LIAM, 100);
                    break;
                }
                case MOVE_TO_DOOR:
                {
                    events.ScheduleEvent(EVENT_FOLLOW_CITIZEN1, 3000);
                    break;
                }
            }
        }

        void DamageTaken(Unit* who, uint32 &damage) override
        {
            if (!enrage && me->GetHealthPct() < 90.0f)
            {
                me->CastSpell(me, SPELL_ENRAGE);
                enrage = true;
                events.ScheduleEvent(EVENT_ENRAGE_COOLDOWN, urand(121000, 150000));
            }
        }

        void AttackStart(Unit* who) override
        {
            if (who->GetEntry() != NPC_PRINCE_LIAM)
                return;
            ScriptedAI::AttackStart(who);
        }

        void SetGUID(ObjectGuid guid, int32 id = 0) override
        { 
            switch (id)
            {
                case NPC_PRINCE_LIAM:
                {
                    liamGUID = guid;
                    break;
                }
                case NPC_FRIGHTENED_CITIZEN_WORGEN:
                {
                    citizenGUID = guid;
                    break;
                }
                case GO_MERCHANT_DOOR:
                {
                    doorGUID = guid;
                    break;
                }
            }
        }

        void DoAction(int32 param) override
        {
            switch (param)
            {
                case ACTION_START_ANIM_MERCANT:
                {
                    events.ScheduleEvent(EVENT_MOVE_TO_DOOR, 1000);
                    break;
                }
                case ACTION_START_ANIM_LIAM:
                {
                    me->SetSpeed(MOVE_RUN, 1.8f);
                    me->GetMotionMaster()->MovePoint(MOVE_TO_START_POSITION, -1482.9f, 1394.6f, 35.55f);
                    break;
                }              
            }
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_ENRAGE_COOLDOWN:
                    {
                        enrage = false;
                        break;
                    }
                    case EVENT_MOVE_TO_LIAM:
                    {
                        if (Creature* liam = ObjectAccessor::GetCreature(*me, liamGUID))
                        {
                            Position pos = liam->GetPosition();
                            float angle = liam->GetAngle(pos.GetPositionX(), pos.GetPositionY());
                            Position pos2 = liam->GetNearPosition(2.0f, angle);
                            me->GetMotionMaster()->MovePoint(MOVE_TO_PRINCE_LIAM, pos2, true);
                        }
                        break;
                    }
                    case EVENT_ATTACK_LIAM:
                    {
                        if (Creature* liam = ObjectAccessor::GetCreature(*me, liamGUID))
                        {
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->Attack(liam, true);
                            liam->Attack(me, true);
                        }
                        break;
                    }
                    case EVENT_MOVE_TO_DOOR:
                    {
                        if (GameObject* go = ObjectAccessor::GetGameObject(*me, doorGUID))
                            me->GetMotionMaster()->MovePoint(MOVE_TO_DOOR, go->GetPosition());
                        break;
                    }
                    case EVENT_FOLLOW_CITIZEN1:
                    {
                        if (Creature* citizen = ObjectAccessor::GetCreature(*me, citizenGUID))
                            me->GetMotionMaster()->MoveFollow(citizen, 1.0f, 0.0f);
                        events.ScheduleEvent(EVENT_FOLLOW_CITIZEN2, 5000);
                        break;
                    }
                    case EVENT_FOLLOW_CITIZEN2:
                    {
                        me->DespawnOrUnsummon();
                        break;
                    }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }
    private:
        EventMap events;
        bool enrage;
        ObjectGuid liamGUID;
        ObjectGuid citizenGUID;
        ObjectGuid doorGUID;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_rampaging_worgen_phase_1AI(creature);
    }
};

// 34913
class npc_prince_liam_greymane_phase_1 : public CreatureScript
{
public:
    npc_prince_liam_greymane_phase_1() : CreatureScript("npc_prince_liam_greymane_phase_1") { }

    struct npc_prince_liam_greymane_phase_1AI : public ScriptedAI
    {
        npc_prince_liam_greymane_phase_1AI(Creature* creature) : ScriptedAI(creature) { Initialize(); }

        void Initialize()
        {
            events.ScheduleEvent(EVENT_START_NEXT_SHOWFIGHT, 60000);
        }

        void Reset() override
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_REMOVE_CLIENT_CONTROL);
        }

        void DamageTaken(Unit* who, uint32 &damage) override
        {
            damage /= 3;
        }

        void JustSummoned(Creature* summon) override
        { 
            if (summon->GetEntry() == NPC_RAMPAGING_WORGEN)
            {
                worgenGUID = summon->GetGUID();
                summon->GetAI()->SetGUID(me->GetGUID(), me->GetEntry());   
                summon->GetAI()->DoAction(ACTION_START_ATTACK_LIAM);
            }
        }

        void SummonedCreatureDies(Creature* summon, Unit* /*killer*/) override
        { 
            if (summon->GetEntry() == NPC_RAMPAGING_WORGEN)
                events.ScheduleEvent(EVENT_START_NEXT_SHOWFIGHT, 20000);
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_START_NEXT_SHOWFIGHT:
                    {                        
                        me->SummonCreature(NPC_RAMPAGING_WORGEN, -1491.9f, 1413.1f, 35.56f, 5.5f, TEMPSUMMON_TIMED_DESPAWN, 120000);
                        break;
                    }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }
    private:
        EventMap events;
        ObjectGuid worgenGUID;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_prince_liam_greymane_phase_1AI(creature);
    }
};

// 34916
class npc_gilneas_city_guard_phase_1 : public CreatureScript
{
public:
    npc_gilneas_city_guard_phase_1() : CreatureScript("npc_gilneas_city_guard_phase_1") { }

    struct npc_gilneas_city_guard_phase_1AI : public ScriptedAI
    {
        npc_gilneas_city_guard_phase_1AI(Creature* creature) : ScriptedAI(creature) { Initialize(); }

        void Initialize()
        {
            worgenList.clear();
            worgenList.push_back(NPC_RAMPAGING_WORGEN_34884);
            worgenList.push_back(NPC_BLOODFANG_WORGEN_35118);
            worgenList.push_back(NPC_RAMPAGING_WORGEN_35660);
            minHealthPct = frand(30.0f, 85.0f);
        }

        void Reset() override
        {
            events.Reset();
            events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, urand(900, 1200));
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
        }

        void DamageTaken(Unit* who, uint32 &Damage) override
        {
            if (who->ToCreature())
                if (me->GetHealthPct() < minHealthPct)
                    Damage = 0;
                else
                    Damage = 1;
        }

        void DamageDealt(Unit* victim, uint32& damage, DamageEffectType damageType) override
        {
            if (victim->ToCreature())
                if (victim->GetHealthPct() < minHealthPct)
                    damage = 0;
                else
                    damage = 1;
        }

        void MovementInform(uint32 type, uint32 pointId) override
        {
            if (type == POINT_MOTION_TYPE && pointId == MOVE_TO_HOMEPOSITION)
                events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 25);
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_CHECK_SHOWFIGHT:
                    {
                        if (me->isDead() || me->IsInCombat())
                        {
                            events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 2500);
                            return;
                        }

                        if (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 10.0f)
                        {
                            me->GetMotionMaster()->MovePoint(MOVE_TO_HOMEPOSITION, me->GetHomePosition());
                            return;
                        }

                        /*if (Creature* worgen = me->FindNearestCreature(worgenList, 5.0f))
                            if (!worgen->IsInCombat())
                            {
                                me->Attack(worgen, true);
                                worgen->Attack(me, true);
                                events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 2500);
                                return;
                            }*/

                        events.ScheduleEvent(EVENT_CHECK_SHOWFIGHT, 2500);
                        break;
                    }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }
    private:
        EventMap events;
        float    minHealthPct;
        std::list<uint32> worgenList;
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_gilneas_city_guard_phase_1AI(creature);
    }
};

// 34884
class npc_rampaging_worgen_phase : public CreatureScript
{
public:
   npc_rampaging_worgen_phase() : CreatureScript("npc_rampaging_worgen_phase") {}

    struct npc_rampaging_worgen_phaseAI : public ScriptedAI
    {
        npc_rampaging_worgen_phaseAI(Creature* creature) : ScriptedAI(creature) {}

        void Reset() override
        {
            enrage = false;
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
        }

        void DamageTaken(Unit* who, uint32 &damage) override
        {
            if (!enrage && me->GetHealthPct() < 90.0f)
            {
                me->CastSpell(me, SPELL_ENRAGE_WORGEN);
                enrage = true;
                events.ScheduleEvent(EVENT_ENRAGE_COOLDOWN, urand(121000, 150000));
            }
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_ENRAGE_COOLDOWN:
                    {
                        enrage = false;
                        break;
                    }
                }
            }

            if (!UpdateVictim())
                return;
            else
                DoMeleeAttackIfReady();
        }
    private:
        EventMap events;
        bool enrage;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_rampaging_worgen_phaseAI(creature);
    }
};

// 34981
class npc_frightened_citizen : public CreatureScript
{
public:
    npc_frightened_citizen() : CreatureScript("npc_frightened_citizen") {} 

    struct npc_frightened_citizenAI : public ScriptedAI
    {
        npc_frightened_citizenAI(Creature* creature) : ScriptedAI(creature) {}

        void Reset() override
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
        }

        void MovementInform(uint32 type, uint32 point) override
        {
            if (type == POINT_MOTION_TYPE)
                switch (point)
                {
                    case MOVE_TO_DOOR:
                    {
                        events.ScheduleEvent(EVENT_MOVE_TO_PLAYER, 25);
                        break;
                    }
                    case MOVE_TO_PLAYER:
                    {
                        Talk(1);
                        events.ScheduleEvent(EVENT_MOVE_TO_MARKER, 4000);
                        break;
                    }
                    case MOVE_TO_MARKER:
                    {
                        events.ScheduleEvent(EVENT_MOVE_TO_END, 25);
                        break;
                    }
                    case MOVE_TO_END:
                    {
                        me->DespawnOrUnsummon();
                        break;
                    }
                }
        }

        void SetGUID(ObjectGuid guid, int32 id = 0) override
        {
            switch (id)
            {
                case PLAYER_GUID:
                {
                    playerGUID = guid;
                    break;
                }
                case GO_MERCHANT_DOOR:
                {
                    doorGUID = guid;
                    break;
                }
            }
        }

        void DoAction(int32 param) override
        {
            switch (param)
            {
                case ACTION_START_ANIM_CITIZEN:
                {
                    events.ScheduleEvent(EVENT_MOVE_TO_DOOR, 2000);
                    break;
                }
            }
        }
        
        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_MOVE_TO_DOOR:
                    {
                        if (GameObject* go = ObjectAccessor::GetGameObject(*me, doorGUID))
                            me->GetMotionMaster()->MovePoint(MOVE_TO_DOOR, go->GetPosition());
                        break;
                    }
                    case EVENT_MOVE_TO_PLAYER:
                    {
                        if (Player* player = ObjectAccessor::GetPlayer(*me, playerGUID))
                            me->GetMotionMaster()->MovePoint(MOVE_TO_PLAYER, player->GetNearPosition(2.0, player->GetOrientation()));
                        break;
                    }
                    case EVENT_MOVE_TO_MARKER:
                    {
                        if (Creature* marker = me->FindNearestCreature(NPC_GILNEAS_EVACUATION_FACING_MARKER_35830, 100.0f))
                            me->GetMotionMaster()->MovePoint(MOVE_TO_MARKER, marker->GetPosition());
                        break;
                    }
                    case EVENT_MOVE_TO_END:
                    {
                        me->GetMotionMaster()->MovePoint(MOVE_TO_END, me->GetNearPosition(25.0f, 0.0f));
                        break;
                    }
                }
            }
        }
    private:
        EventMap events;
        ObjectGuid playerGUID;
        ObjectGuid doorGUID;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_frightened_citizenAI(creature);
    }
};

// 35836
class npc_frightened_citizen_1 : public CreatureScript
{
public:
    npc_frightened_citizen_1() : CreatureScript("npc_frightened_citizen_1") {} 

    struct npc_frightened_citizen_1AI : public ScriptedAI
    {
        npc_frightened_citizen_1AI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() override
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
        }

        void MovementInform(uint32 type, uint32 point) override
        {
            if (type == POINT_MOTION_TYPE)
                switch (point)
                {
                    case MOVE_TO_DOOR:
                    {
                        events.ScheduleEvent(EVENT_MOVE_TO_PLAYER, 25);
                        break;
                    }
                    case MOVE_TO_PLAYER:
                    {
                        Talk(0);
                        events.ScheduleEvent(EVENT_MOVE_TO_MARKER, 2000);
                        break;
                    }
                    case MOVE_TO_MARKER:
                    {
                        events.ScheduleEvent(EVENT_MOVE_TO_END, 25);
                        break;
                    }
                    case MOVE_TO_END:
                    {
                        me->DespawnOrUnsummon();
                        break;
                    }
                }
        }

        void SetGUID(ObjectGuid guid, int32 id = 0) override
        {
            switch (id)
            {
                case PLAYER_GUID:
                {
                    playerGUID = guid;
                    break;
                }
                case GO_MERCHANT_DOOR:
                {
                    doorGUID = guid;
                    break;
                }
            }
        }

        void DoAction(int32 param) override
        {
            switch (param)
            {
                case ACTION_START_ANIM_CITIZEN:
                {
                    events.ScheduleEvent(EVENT_MOVE_TO_DOOR, 1000);
                    break;
                }
            }
        }

        void UpdateAI(uint32 diff) override
        {
            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_MOVE_TO_DOOR:
                    {
                        if (GameObject* go = ObjectAccessor::GetGameObject(*me, doorGUID))
                            me->GetMotionMaster()->MovePoint(MOVE_TO_DOOR, go->GetPosition());
                        break;
                    }
                    case EVENT_MOVE_TO_PLAYER:
                    {
                        if (Player* player = ObjectAccessor::GetPlayer(*me, playerGUID))
                        {
                            me->GetMotionMaster()->MovePoint(MOVE_TO_PLAYER, player->GetNearPosition(2.0, player->GetOrientation()));
                        }
                        break;
                    }
                    case EVENT_MOVE_TO_MARKER:
                    {
                        if (Creature* marker = me->FindNearestCreature(NPC_GILNEAS_EVACUATION_FACING_MARKER_35830, 100.0f))
                            me->GetMotionMaster()->MovePoint(MOVE_TO_MARKER, marker->GetPosition());
                        break;
                    }
                    case EVENT_MOVE_TO_END:
                    {
                        me->GetMotionMaster()->MovePoint(MOVE_TO_END, me->GetNearPosition(10.0f, 0.0f));
                        break;
                    }
                }
            }
        }
    private:
        EventMap events;
        ObjectGuid playerGUID;
        ObjectGuid doorGUID;
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_frightened_citizen_1AI(creature);
    }
};

// 195327
class go_merchant_square_door : public GameObjectScript
{
public:
    go_merchant_square_door() : GameObjectScript("go_merchant_square_door") { }

    bool OnGossipHello(Player* player, GameObject* go) override
    {
        if (!player || !go)
            return false;

        switch (urand(0, 1))
        {
            case 0:
            {
                SummonOnlyCitizen(player, go);
                break;
            }
            case 1:
            {
                SummonCitizenAndWorgen(player, go);
                break;
            }
        }

        return false;
    }

private:
    void SummonOnlyCitizen(Player* player, GameObject* go)
    {
        Position posC = GetCitizenPosition(go);
        if (Creature* citizen = player->SummonCreature(NPC_FRIGHTENED_CITIZEN_ALONE, posC, TEMPSUMMON_TIMED_DESPAWN, 30000))
        {
            citizen->GetAI()->SetGUID(go->GetGUID(), go->GetEntry());
            citizen->GetAI()->SetGUID(player->GetGUID(), PLAYER_GUID);
            citizen->GetAI()->DoAction(ACTION_START_ANIM_CITIZEN);
        }       
    }

    void SummonCitizenAndWorgen(Player* player, GameObject* go)
    {
        Position posC = GetCitizenPosition(go);
        Position posW = GetWorgenPosition(go);
        if (Creature* citizen = player->SummonCreature(NPC_FRIGHTENED_CITIZEN_WORGEN, posC, TEMPSUMMON_TIMED_DESPAWN, 30000))
            if (Creature* worgen = player->SummonCreature(NPC_RAMPAGING_WORGEN, posW, TEMPSUMMON_TIMED_DESPAWN, 30000))
            {
                citizen->GetAI()->SetGUID(go->GetGUID(), go->GetEntry());
                citizen->GetAI()->SetGUID(player->GetGUID(), PLAYER_GUID);
                citizen->GetAI()->SetGUID(worgen->GetGUID(), worgen->GetEntry());
                citizen->GetAI()->DoAction(ACTION_START_ANIM_CITIZEN);
                worgen->GetAI()->SetGUID(go->GetGUID(), go->GetEntry());
                worgen->GetAI()->SetGUID(player->GetGUID(), PLAYER_GUID);
                worgen->GetAI()->SetGUID(citizen->GetGUID(), citizen->GetEntry());
                worgen->GetAI()->DoAction(ACTION_START_ANIM_MERCANT);
            }
    }

    Position GetCitizenPosition(GameObject* go)
    {
        float x, y;
        go->GetNearPoint2D(x, y, 1.0f, go->GetOrientation() + M_PI);
        return Position(x, y, go->GetPositionZ());
    }

    Position GetWorgenPosition(GameObject* go)
    {
        float x, y;
        go->GetNearPoint2D(x, y, 3.0f, go->GetOrientation() + M_PI * 0.75f);
        return Position(x, y, go->GetPositionZ());
    }

    ObjectGuid playerGUID;
    ObjectGuid doorGUID;
    ObjectGuid citizenGUID;
    ObjectGuid worgenGUID;
};

void AddSC_gilneas_pl()
{
    new npc_gilneas_city_guard_gate();
    new npc_prince_liam_greymane();
    new npc_lieutenant_walden();
    new npc_rampaging_worgen_phase_1();
    new npc_gilneas_city_guard_phase_1();
    new npc_prince_liam_greymane_phase_1();
    new go_merchant_square_door();
    new npc_rampaging_worgen_phase();
    new npc_frightened_citizen();
    new npc_frightened_citizen_1();
}
