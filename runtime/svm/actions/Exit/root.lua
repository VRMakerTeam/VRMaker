local core = require "core"
local diagram = require "diagram"

local alias = {en_US="Exit", zh_CN="出口", tw="出口", pt="Saída", fr="sortie", it="Uscita", sp="salida", jp="出口", kr="출구", de="Ausgang", ru="Выход"}

diagram.setGroupAlias("Exit", core.translate(alias))
