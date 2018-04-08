local core = require "core"
local diagram = require "diagram"

local alias = {en_US="Entry", zh_CN="入口"}

diagram.setGroupAlias("Entry", core.translate(alias))
