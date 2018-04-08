local log = require "log"
local diagram = require "diagram"

function init_diagram()
    log.trace("init diagram")
    --register action
    diagram.registerActions()
    diagram.refreshActions()
end
