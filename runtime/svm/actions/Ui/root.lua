local core = require "core"
local diagram = require "diagram"

local alias = {en_US="UI", zh_CN="界面", tw="介面", pt="Interface", fr="L'interface", it="interfaccia", sp="Interfaz", jp="インターフェース", kr="인터페이스", de="Schnittstelle", ru="интерфейс"}

diagram.setGroupAlias("Ui", core.translate(alias))
