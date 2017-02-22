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
    SPELL_MONK_ROLL_TRIGGER                             = 107427
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

void AddSC_monk_spell_scripts_pl()
{
    new spell_monk_chi_wave_healing_bolt();
    new spell_monk_roll();
    new spell_monk_transcendence_transfer();
}
