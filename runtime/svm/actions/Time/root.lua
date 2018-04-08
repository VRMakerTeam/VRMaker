local core = require "core"
local diagram = require "diagram"

local alias = {en_US="Time", zh_CN="时间"}

diagram.setGroupAlias("Time", core.translate(alias))
