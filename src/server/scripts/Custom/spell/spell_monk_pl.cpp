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
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Cell.h"
#include "CellImpl.h"

enum MonkSpells
{
    SPELL_MONK_CHI_WAVE_HEAL                            = 132463,
    SPELL_MONK_ROLL                                     = 109132,
    SPELL_MONK_ROLL_TRIGGER                             = 107427,
    SPELL_MONK_CREATE_CHI_SPHERE                        = 121283,
    SPELL_MONK_POWER_STRIKES_AURA                       = 129914,
    SPELL_MONK_RENEWING_MIST_HOT                        = 119611,
    SPELL_MONK_THUNDER_FOCUS_TEA                        = 116680

};

enum MonkCreatures
{
    TRANSCENDENCE_SPIRIT = 54569
};

// 173545 - Chi Wave (healing bolt)
// 7.x.x
class spell_monk_chi_wave_healing_bolt: public SpellScriptLoader
{
    public:
        spell_monk_chi_wave_healing_bolt() : SpellScriptLoader("spell_monk_chi_wave_healing_bolt") { }

        class spell_monk_chi_wave_healing_bolt_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_chi_wave_healing_bolt_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MONK_CHI_WAVE_HEAL))
                    return false;
                return true;
            }

            void HandleOnHit()
            {
                if (!GetOriginalCaster())
                    return;

                if (Player* player = GetOriginalCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (!target->IsValidAttackTarget(player))
                            player->CastSpell(target, SPELL_MONK_CHI_WAVE_HEAL, true);
                    }
                }
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_monk_chi_wave_healing_bolt_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_monk_chi_wave_healing_bolt_SpellScript();
        }
};

// 109132 - Roll
// 7.x.x
class spell_monk_roll : public SpellScriptLoader
{
    public:
        spell_monk_roll() : SpellScriptLoader("spell_monk_roll") { }
        class spell_monk_roll_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_roll_SpellScript);

            bool Validate(SpellInfo const* /*spell*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MONK_ROLL_TRIGGER))
                    return false;
                return true;
            }

            bool Load() override
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void HandleBeforeCast()
            {
                Aura* aura = GetCaster()->AddAura(SPELL_MONK_ROLL_TRIGGER, GetCaster());
                if (!aura)
                    return;
                AuraApplication* app = aura->GetApplicationOfTarget(GetCaster()->GetGUID());
                if (!app)
                    return;
                app->ClientUpdate();
            }

            void HandleAfterCast()
            {
                Unit* caster = GetCaster();
                caster->CastSpell(caster, SPELL_MONK_ROLL_TRIGGER, true);
            }

            void Register() override
            {
                BeforeCast += SpellCastFn(spell_monk_roll_SpellScript::HandleBeforeCast);
                AfterCast += SpellCastFn(spell_monk_roll_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_monk_roll_SpellScript();
        }
};

// Transcendence : Transfer - 119996
// 7.x.x
class spell_monk_transcendence_transfer: public SpellScriptLoader
{
    public:
        spell_monk_transcendence_transfer() : SpellScriptLoader("spell_monk_transcendence_transfer") { }

        class spell_monk_transcendence_transfer_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_transcendence_transfer_SpellScript);

            SpellCastResult CheckSpiritRange()
            {
                if (Unit* caster = GetCaster())
                {
                    for (Unit::ControlList::const_iterator itr = caster->m_Controlled.begin(); itr != caster->m_Controlled.end(); ++itr)
                    {
                        if ((*itr)->GetEntry() == TRANSCENDENCE_SPIRIT)
                        {
                            if ((*itr)->GetDistance(caster) > 40.0f)
                                return SPELL_FAILED_DONT_REPORT;
                        }
                    }
                }

                return SPELL_CAST_OK;
            }

            void HandleDummy()
            {
                if (Unit* caster = GetCaster())
                {
                    for (Unit::ControlList::const_iterator itr = caster->m_Controlled.begin(); itr != caster->m_Controlled.end(); ++itr)
                    {
                        if ((*itr)->GetEntry() == TRANSCENDENCE_SPIRIT)
                        {
                            Creature* clone = (*itr)->ToCreature();
                            if (clone && clone->AI())
                                clone->AI()->DoAction(1);
                        }
                    }
                }
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_monk_transcendence_transfer_SpellScript::CheckSpiritRange);
                OnHit += SpellHitFn(spell_monk_transcendence_transfer_SpellScript::HandleDummy);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_monk_transcendence_transfer_SpellScript();
        }
};

// Dampen Harm - 122278
// 7.x.x
class spell_monk_dampen_harm: public SpellScriptLoader
{
    public:
        spell_monk_dampen_harm() : SpellScriptLoader("spell_monk_dampen_harm") { }

        class spell_monk_dampen_harm_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_monk_dampen_harm_AuraScript);

        private:
            uint32 healPct;

            bool Load()
            {
                healPct = GetSpellInfo()->GetEffect(EFFECT_0)->CalcValue();
                return true;
            }

            void CalculateAmount(AuraEffect const* /*auraEff*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                amount = -1;
            }

            void Absorb(AuraEffect* auraEff, DamageInfo& dmgInfo, uint32& absorbAmount)
            {
                Unit* target = GetTarget();

                uint32 health = target->CountPctFromMaxHealth(healPct);

                if (dmgInfo.GetDamage() < health)
                    return;

                absorbAmount = dmgInfo.GetDamage() / 2;
                auraEff->GetBase()->DropCharge();
            }

            void Register() override
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_monk_dampen_harm_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_monk_dampen_harm_AuraScript::Absorb, EFFECT_0);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_monk_dampen_harm_AuraScript();
        }
};

// Zen Flight - 125883
// 7.x.x
class spell_monk_zen_flight_check: public SpellScriptLoader
{
    public:
        spell_monk_zen_flight_check() : SpellScriptLoader("spell_monk_zen_flight_check") { }

        class spell_monk_zen_flight_check_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_zen_flight_check_SpellScript);

            SpellCastResult CheckTarget()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (_player->GetMap()->IsBattlegroundOrArena())
                        return SPELL_FAILED_NOT_IN_BATTLEGROUND;

                    if (!_player->HasSpell(90267) && (_player->GetMapId() == 1 || _player->GetMapId() == 0))
                        return SPELL_FAILED_NOT_HERE;

                    if (!_player->HasSpell(115913) && (_player->GetMapId() == 870))
                        return SPELL_FAILED_NOT_HERE;
                }

                return SPELL_CAST_OK;
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_monk_zen_flight_check_SpellScript::CheckTarget);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_monk_zen_flight_check_SpellScript();
        }
};

// Power Strikes - 121817
// 7.x.x
class spell_monk_power_strikes: public SpellScriptLoader
{
    public:
        spell_monk_power_strikes() : SpellScriptLoader("spell_monk_power_strikes") { }

        class spell_monk_power_strikes_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_power_strikes_SpellScript);

            void HandleAfterCast()
            {
                Unit* caster = GetCaster();
                
                if (caster->HasAura(SPELL_MONK_POWER_STRIKES_AURA))
                {
                    if (caster->GetPower(POWER_CHI) >= caster->GetMaxPower(POWER_CHI))
                        caster->CastSpell(caster, SPELL_MONK_CREATE_CHI_SPHERE, true);
                    else
                        caster->ModifyPower(POWER_CHI, 1);

                    caster->RemoveAura(SPELL_MONK_POWER_STRIKES_AURA);
                }
            }

            void Register() override
            {
                AfterCast += SpellCastFn(spell_monk_power_strikes_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_monk_power_strikes_SpellScript();
        }
};

// Thunder Focus Tea - 116680
// Called 116670, 191837
// 7.x.x
class spell_monk_thunder_focus_tea: public SpellScriptLoader
{
    public:
        spell_monk_thunder_focus_tea() : SpellScriptLoader("spell_monk_thunder_focus_tea") { }

        class spell_monk_thunder_focus_tea_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_monk_thunder_focus_tea_SpellScript);

            void FilterTargets(std::list<WorldObject*>& unitList)
            {
                unitList.remove_if(Trinity::UnitAuraCheck(false, SPELL_MONK_RENEWING_MIST_HOT, GetCaster()->GetGUID()));
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        if (_player->HasAura(SPELL_MONK_THUNDER_FOCUS_TEA))
                        {
                            std::list<Unit*> groupList;

                            _player->GetPartyMembers(groupList);

                            for (auto itr : groupList)
                                if (Aura* renewingMistGroup = itr->GetAura(SPELL_MONK_RENEWING_MIST_HOT, _player->GetGUID()))
                                    renewingMistGroup->RefreshDuration();

                            _player->RemoveAura(SPELL_MONK_THUNDER_FOCUS_TEA);
                        }
                    }
                }
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_monk_thunder_focus_tea_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ALLY);
                OnHit += SpellHitFn(spell_monk_thunder_focus_tea_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_monk_thunder_focus_tea_SpellScript();
        }
};

void AddSC_monk_spell_scripts_pl()
{
    new spell_monk_chi_wave_healing_bolt();
    new spell_monk_roll();
    new spell_monk_transcendence_transfer();
    new spell_monk_dampen_harm();
    new spell_monk_zen_flight_check();
    new spell_monk_power_strikes();
    new spell_monk_thunder_focus_tea();
}
