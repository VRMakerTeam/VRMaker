local core = require "core"
local diagram = require "diagram"

local alias = {en_US="UI", zh_CN="界面"}

diagram.setGroupAlias("Ui", core.translate(alias))
