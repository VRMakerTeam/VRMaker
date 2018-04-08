local core = require "core"
local diagram = require "diagram"

local alias = {en_US="Sound", zh_CN="声音"}

diagram.setGroupAlias("Sound", core.translate(alias))
