local core = require "core"
local log = require "log"
local diagram = require "diagram"

local extend = "basic"
local group = "Time"
local template = "Sleep"

local image = "Time/Time.png"

local prop_time = "duration"
local label_time = {en_US="time", zh_CN="时间"}
local txt_sleep = {en_US="Wait", zh_CN="等待"}

log.info("register "..extend.."."..group.."."..template.." action")
diagram.registerBlock(extend, group, template)
diagram.blockUiUseImage(group, template, image)

diagram.blockRegisterProperty(group, template, prop_time,0) 

diagram.blockUiAddText(group, template, core.translate(txt_sleep))

diagram.inspectorUiBindNumberSpin(group, template, prop_time, core.translate(label_time), 0, 999999.0)
