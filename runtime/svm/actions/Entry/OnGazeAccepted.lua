local core = require "core"
local log = require "log"
local diagram = require "diagram"

local extend = "VR"
local group = "Entry"
local template = "OnGazeAccepted"

local image = "Entry/Entry.png"

local prop_trigger = "trigger"

local label_trigger = {en_US="Gaze", zh_CN="视线"}
local txt_accepted = {en_US="Accepted", zh_CN="被触发时"}

log.info("register "..extend.."."..group.."."..template.." action")
diagram.registerBlock(extend, group, template)
diagram.blockUiUseImage(group, template, image)

diagram.blockRegisterProperty(group, template, prop_trigger, "")

diagram.blockUiAddText(group, template, core.translate(label_trigger))
diagram.blockUiBindProperty(group, template, prop_trigger)
diagram.blockUiAddText(group, template, core.translate(txt_accepted))

diagram.inspectorUiBindLineEdit(group, template, prop_trigger, core.translate(label_trigger))
