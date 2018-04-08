local core = require "core"
local diagram = require "diagram"

local alias = {en_US="Exit", zh_CN="出口"}

diagram.setGroupAlias("Exit", core.translate(alias))
