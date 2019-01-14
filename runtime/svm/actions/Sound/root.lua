local core = require "core"
local diagram = require "diagram"

local alias = {en_US="Sound", zh_CN="声音", tw="聲音", pt="Som", fr="Du son", it="Suono", sp="Sonar", jp="音", kr="소리", de="Klingen", ru="звук"}

diagram.setGroupAlias("Sound", core.translate(alias))
