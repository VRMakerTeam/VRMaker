local core = require "core"
local diagram = require "diagram"

local alias = {en_US="Time", zh_CN="时间", tw="時間", pt="Tempo", fr="Temps", it="Tempo", sp="Hora", jp="時間", kr="시각", de="Zeit", ru="Время"}

diagram.setGroupAlias("Time", core.translate(alias))
