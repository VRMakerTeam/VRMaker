local core = require "core"
local diagram = require "diagram"

local alias = {en_US="Entry", zh_CN="入口", tw="入口", pt="Entrada", fr="Entrée", it="Iscrizione", sp="Entrada", jp="エントリ", kr="기입", de="Einstiegspunkt", ru="точка входа"}

diagram.setGroupAlias("Entry", core.translate(alias))
