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
#include "GridNotifiers.h"

enum PriestSpells
{
    SPELL_PRIEST_SURGE_OF_LIGHT                     = 114255,
    SPELL_PRIEST_SURGE_OF_LIGHT_VISUAL              = 128654,
    SPELL_PRIEST_BODY_AND_SOUL_AURA                 = 64129,
    SPELL_PRIEST_BODY_AND_SOUL_INCREASE_SPEED       = 65081
};

// Shadowfiend - 34433
// Mindbender - 123040
class spell_pri_shadowfiend: public SpellScriptLoader
{
    public:
        spell_pri_shadowfiend() : SpellScriptLoader("spell_pri_shadowfiend") { }

        class spell_pri_shadowfiend_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_shadowfiend_SpellScript);

            void HandleAfterHit()
            {
                if (Player* pl_player = GetCaster()->ToPlayer())
                {
                    if (Unit* pl_target = GetExplTargetUnit())
                    {
                        if (Guardian* pl_pet = pl_player->GetGuardianPet())
                        {
                            pl_pet->InitCharmInfo();
                            pl_pet->SetReactState(REACT_AGGRESSIVE);

                            if (pl_pet->IsValidAttackTarget(pl_target))
                                pl_pet->ToCreature()->AI()->AttackStart(pl_target);
                            else
                            {
                                Unit* pl_victim = pl_player->GetSelectedUnit();
                                if (pl_victim && pl_pet->IsValidAttackTarget(pl_target))
                                    pl_pet->ToCreature()->AI()->AttackStart(pl_target);
                            }
                        }
                    }
                }
            }

            void Register() override
            {
                AfterHit += SpellHitFn(spell_pri_shadowfiend_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_pri_shadowfiend_SpellScript();
        }
};

// Surge of Light - 114255
class spell_pri_surge_of_light : public SpellScriptLoader
{
    public:
        spell_pri_surge_of_light() : SpellScriptLoader("spell_pri_surge_of_light") { }

        class spell_pri_surge_of_light_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_surge_of_light_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_PRIEST_SURGE_OF_LIGHT))
                    return false;
                return true;
            }

            int32 m_duration = 0;

            void HandleOnPrepare()
            {
                if (Unit* pl_caster = GetCaster())
                if (Aura* pl_SurgeOfLight = pl_caster->GetAura(SPELL_PRIEST_SURGE_OF_LIGHT))
                    m_duration = pl_SurgeOfLight->GetDuration();
            }

            void HandleOnCast()
            {
                if (Unit* pl_caster = GetCaster())
                if (Aura* pl_SurgeOfLight = pl_caster->GetAura(SPELL_PRIEST_SURGE_OF_LIGHT))
                {
                    pl_SurgeOfLight->SetDuration(m_duration);
                    if (pl_SurgeOfLight->GetStackAmount() > 1)
                        pl_SurgeOfLight->SetStackAmount(1);
                }
            }

            void Register() override
            {
                OnPrepare += SpellOnPrepareFn(spell_pri_surge_of_light_SpellScript::HandleOnPrepare);
                OnCast += SpellCastFn(spell_pri_surge_of_light_SpellScript::HandleOnCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_pri_surge_of_light_SpellScript();
        }

        class spell_pri_surge_of_light_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pri_surge_of_light_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_PRIEST_SURGE_OF_LIGHT_VISUAL))
                    return false;
                return true;
            }

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* pl_caster = GetCaster();
                if (!pl_caster->HasAura(SPELL_PRIEST_SURGE_OF_LIGHT_VISUAL))
                    pl_caster->CastSpell(pl_caster, SPELL_PRIEST_SURGE_OF_LIGHT_VISUAL, true);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* pl_caster = GetCaster();
                if (pl_caster->HasAura(SPELL_PRIEST_SURGE_OF_LIGHT_VISUAL))
                    pl_caster->RemoveAura(SPELL_PRIEST_SURGE_OF_LIGHT_VISUAL);
            }

            void Register() override
            {
                OnEffectApply += AuraEffectRemoveFn(spell_pri_surge_of_light_AuraScript::OnApply, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_pri_surge_of_light_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_ADD_PCT_MODIFIER, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_pri_surge_of_light_AuraScript();
        }
};

// Surge of Light (aura)
class spell_pri_surge_of_light_aura : public SpellScriptLoader
{
    public:
        spell_pri_surge_of_light_aura() : SpellScriptLoader("spell_pri_surge_of_light_aura") { }

        class spell_pri_surge_of_light_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_pri_surge_of_light_aura_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_PRIEST_SURGE_OF_LIGHT))
                    return false;
                return true;
            }

            void OnProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();

                if (!eventInfo.GetHealInfo() || !eventInfo.GetHealInfo()->GetHeal() || !eventInfo.GetActor())
                    return;

                if (Player* pl_player = eventInfo.GetActor()->ToPlayer())
                {
                    if (pl_player->GetUInt32Value(PLAYER_FIELD_CURRENT_SPEC_ID) == TALENT_SPEC_PRIEST_SHADOW)
                        return;

                    if (roll_chance_i(GetSpellInfo()->GetEffect(EFFECT_0)->CalcValue()))
                    {
                        if (Aura* pl_SurgeOfLight = pl_player->GetAura(SPELL_PRIEST_SURGE_OF_LIGHT))
                        {
                            if (pl_SurgeOfLight->GetStackAmount() == 2)
                                pl_SurgeOfLight->SetDuration(pl_SurgeOfLight->GetMaxDuration());
                            else
                                pl_player->CastSpell(pl_player, SPELL_PRIEST_SURGE_OF_LIGHT, true);
                        }
                        else
                            pl_player->CastSpell(pl_player, SPELL_PRIEST_SURGE_OF_LIGHT, true);
                    }
                }
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_pri_surge_of_light_aura_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_pri_surge_of_light_aura_AuraScript();
        }
};

// Called by Leap of Faith - 73325 and Power Word : Shield - 17
// Body and Soul - 64129
// 7.x.x
class spell_pri_body_and_soul: public SpellScriptLoader
{
    public:
        spell_pri_body_and_soul() : SpellScriptLoader("spell_pri_body_and_soul") { }

        class spell_pri_body_and_soul_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_pri_body_and_soul_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_PRIEST_BODY_AND_SOUL_AURA) ||
                    !sSpellMgr->GetSpellInfo(SPELL_PRIEST_BODY_AND_SOUL_INCREASE_SPEED))
                    return false;
                return true;
            }

            void HandleOnHit()
            {
                if (Player* _player = GetCaster()->ToPlayer())
                    if (Unit* target = GetHitUnit())
                        if (_player->HasAura(SPELL_PRIEST_BODY_AND_SOUL_AURA))
                            _player->CastSpell(target, SPELL_PRIEST_BODY_AND_SOUL_INCREASE_SPEED, true);
            }

            void Register() override
            {
                OnHit += SpellHitFn(spell_pri_body_and_soul_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_pri_body_and_soul_SpellScript();
        }
};

void AddSC_priest_spell_scripts_pl()
{
    new spell_pri_shadowfiend();
    new spell_pri_surge_of_light();
    new spell_pri_surge_of_light_aura();
    new spell_pri_body_and_soul();
}
