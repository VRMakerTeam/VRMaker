local core = require "core"
local diagram = require "diagram"

local alias = {en_US="Camera", zh_CN="摄像机"}

diagram.setGroupAlias("Camera", core.translate(alias))
