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

#include "Player.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "SpellHistory.h"
#include "Containers.h"

enum DeathKnightSpells
{
    SPELL_DK_SPELL_DESECRATED_GROUND            = 118009,
    SPELL_DK_SPELL_DESECRATED_GROUND_IMMUNE     = 115018
};

// 118009 - Desecrated ground
// 7.x.x
class spell_dk_desecrated_ground : public SpellScriptLoader
{
    public:
        spell_dk_desecrated_ground() : SpellScriptLoader("spell_dk_desecrated_ground") { }
        class spell_dk_desecrated_ground_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_desecrated_ground_AuraScript);
            void OnTick(AuraEffect const* /*aurEff*/)
            {
                if (GetCaster())
                    if (DynamicObject* dynObj = GetCaster()->GetDynObject(SPELL_DK_SPELL_DESECRATED_GROUND))
                        if (GetCaster()->GetDistance(dynObj) <= 8.0f)
                            GetCaster()->CastSpell(GetCaster(), SPELL_DK_SPELL_DESECRATED_GROUND_IMMUNE, true);
            }
            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dk_desecrated_ground_AuraScript::OnTick, EFFECT_1, SPELL_AURA_PERIODIC_DUMMY);
            }
        };
        AuraScript* GetAuraScript() const override
        {
            return new spell_dk_desecrated_ground_AuraScript();
        }
};
// 49184 - Howling Blast
// 7.x.x
class spell_dk_howling_blast : public SpellScriptLoader
{
    public:
        spell_dk_howling_blast() : SpellScriptLoader("spell_dk_howling_blast") { }
        class spell_dk_howling_blast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_howling_blast_SpellScript);
            void HandleBeforeCast()
            {
                Unit* target = GetExplTargetUnit();
                Unit* caster = GetCaster();
                if (!caster || !target)
                    return;
            }
            void HandleOnHit()
            {
                Unit* target = GetHitUnit();
                Unit* caster = GetCaster();
                if (!caster || !target)
                    return;
                if (target->HasAuraState(AURA_STATE_FROZEN))
                    SetHitDamage(GetHitDamage()*5); // *8 //7.1.5?
                else
                    SetHitDamage(GetHitDamage());
                caster->CastSpell(target, SPELL_DK_FROST_FEVER, true);
            }
            void Register() override
            {
                BeforeCast += SpellCastFn(spell_dk_howling_blast_SpellScript::HandleBeforeCast);
                OnHit += SpellHitFn(spell_dk_howling_blast_SpellScript::HandleOnHit);
            }
        };
        SpellScript* GetSpellScript() const override
        {
            return new spell_dk_howling_blast_SpellScript();
        }
};

// 108199 - Gorefiend's Grasp
// 7.x.x
class spell_dk_gorefiends_grasp : public SpellScriptLoader
{
    public:
        spell_dk_gorefiends_grasp() : SpellScriptLoader("spell_dk_gorefiends_grasp") { }
        class spell_dk_gorefiends_grasp_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dk_gorefiends_grasp_SpellScript);
            void HandleScript(SpellEffIndex effIndex)
            {
                if (Unit* _unit = GetCaster()->ToUnit())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        std::list<Unit*> TargetList;
                        Trinity::AnyUnfriendlyUnitInObjectRangeCheck checker(GetCaster(), GetCaster(), 20.0f);
                        Trinity::UnitListSearcher<Trinity::AnyUnfriendlyUnitInObjectRangeCheck> searcher(GetCaster(), TargetList, checker);
                        GetCaster()->VisitNearbyObject(20.0f, searcher);
                        for (std::list<Unit*>::iterator itr = TargetList.begin(); itr != TargetList.end(); ++itr)
                        {
                             Unit* target = *itr;
                             if (!target || !target->IsAlive())
                                 continue;
                    
                             _unit->CastSpell(target, 114869, true);
                             target->CastSpell(_unit, 146599, true);
                        }
                    }
                }
            }
            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_dk_gorefiends_grasp_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };
        SpellScript* GetSpellScript() const override
        {
            return new spell_dk_gorefiends_grasp_SpellScript();
        }
};

// 119975 - Conversion
// 7.x.x
class spell_dk_conversion : public SpellScriptLoader
{
    public:
        spell_dk_conversion() : SpellScriptLoader("spell_dk_conversion") { }
        class spell_dk_conversion_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_conversion_AuraScript);
            void OnTick(AuraEffect const* aurEff)
            {
                if (GetCaster())
                {
                    // Drain 5 runic power to regen 2% of max health per second
                    int32 runicPower = GetCaster()->GetPower(POWER_RUNIC_POWER);
                    if (runicPower > 50)
                        GetCaster()->SetPower(POWER_RUNIC_POWER, GetCaster()->GetPower(POWER_RUNIC_POWER) - 50);
                    else if (runicPower > 0)
                    {
                        GetCaster()->SetPower(POWER_RUNIC_POWER, 0);
                        GetCaster()->RemoveAura(119980);
                    }
                    else if (runicPower == 0)
                        GetCaster()->RemoveAura(119980);
                 }
            }
            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_dk_conversion_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };
        AuraScript* GetAuraScript() const override
        {
            return new spell_dk_conversion_AuraScript();
        }
};

// 116888 - Purgatory
// 7.x.x
class spell_dk_purgatory : public SpellScriptLoader
{
    public:
        spell_dk_purgatory() : SpellScriptLoader("spell_dk_purgatory") { }
        class spell_dk_purgatory_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_purgatory_AuraScript);
            void OnRemove(AuraEffect const* /*eff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* _unit = GetTarget()->ToUnit())
                {
                    AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
                    if (removeMode == AURA_REMOVE_BY_EXPIRE)
                        _unit->CastSpell(_unit, 123982, true);
                }
            }
            void Register() override
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_dk_purgatory_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_SCHOOL_HEAL_ABSORB, AURA_EFFECT_HANDLE_REAL);
            }
        };
        AuraScript* GetAuraScript() const override
        {
            return new spell_dk_purgatory_AuraScript();
        }
};

// 114556 - Purgatory
// 7.x.x
class spell_dk_purgatory_absorb : public SpellScriptLoader
{
    public:
        spell_dk_purgatory_absorb() : SpellScriptLoader("spell_dk_purgatory_absorb") { }
        class spell_dk_purgatory_absorb_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_purgatory_absorb_AuraScript);
            void CalculateAmount(AuraEffect const* /*aurEff*/, int32& amount, bool& /*canBeRecalculated*/)
            {
                amount = -1;
            }
            void Absorb(AuraEffect* /*aurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
            {
                Unit* target = GetTarget();
                if (dmgInfo.GetDamage() < target->GetHealth())
                    return;
                // No damage received under Shroud of Purgatory
                if (target->ToUnit()->HasAura(116888))
                {
                    absorbAmount = dmgInfo.GetDamage();
                    return;
                }
                if (target->ToUnit()->HasAura(123981))
                    return;
                int32 bp = dmgInfo.GetDamage();
                target->CastCustomSpell(target, 116888, &bp, NULL, NULL, true);
                target->CastSpell(target, 123981, true);
                target->SetHealth(1);
                absorbAmount = dmgInfo.GetDamage();
            }
            void Register() override
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dk_purgatory_absorb_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_dk_purgatory_absorb_AuraScript::Absorb, EFFECT_0);
            }
        };
        AuraScript* GetAuraScript() const override
        {
            return new spell_dk_purgatory_absorb_AuraScript();
        }
};

void AddSC_deathknight_spell_scripts_pl()
{
    new spell_dk_desecrated_ground();
    new spell_dk_howling_blast();
    new spell_dk_gorefiends_grasp();
    new spell_dk_conversion();
    new spell_dk_purgatory();
    new spell_dk_purgatory_absorb();
}
