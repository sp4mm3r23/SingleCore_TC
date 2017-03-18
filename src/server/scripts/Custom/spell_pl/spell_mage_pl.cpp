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
#include "SpellHistory.h"
#include "SpellAuraEffects.h"
#include "Pet.h"
#include "GridNotifiers.h"

enum MageSpells_pl
{
    SPELL_MAGE_MIRROR_IMAGE_LEFT                 = 58834,
    SPELL_MAGE_MIRROR_IMAGE_RIGHT                = 58833,
    SPELL_MAGE_MIRROR_IMAGE_FRONT                = 58831
};

// Mirror Image - 55342
class spell_mage_mirror_image_summon: public SpellScriptLoader
{
    public:
        spell_mage_mirror_image_summon() : SpellScriptLoader("spell_mage_mirror_image_summon") { }

        class spell_mage_mirror_image_summon_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_mage_mirror_image_summon_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_MAGE_MIRROR_IMAGE_LEFT) ||
                    !sSpellMgr->GetSpellInfo(SPELL_MAGE_MIRROR_IMAGE_FRONT) ||
                    !sSpellMgr->GetSpellInfo(SPELL_MAGE_MIRROR_IMAGE_RIGHT))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    caster->CastSpell(caster, SPELL_MAGE_MIRROR_IMAGE_LEFT, true);
                    caster->CastSpell(caster, SPELL_MAGE_MIRROR_IMAGE_FRONT, true);
                    caster->CastSpell(caster, SPELL_MAGE_MIRROR_IMAGE_RIGHT, true);
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_mage_mirror_image_summon_SpellScript::HandleDummy, EFFECT_1, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_mage_mirror_image_summon_SpellScript();
        }
};

void AddSC_mage_spell_scripts_pl()
{
    new spell_mage_mirror_image_summon();
}
