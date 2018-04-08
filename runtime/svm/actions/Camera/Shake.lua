local core = require "core"
local log = require "log"
local diagram = require "diagram"
local canvas = require "canvas"

local extend = "basic"
local group = "Camera"
local template = "Shake"

local image = "Camera/Camera.png"

local prop_duration = "duration"

local label_duration = {en_US="Duration", zh_CN="时间"}
local txt_shake = {en_US="Shake", zh_CN="抖动"}

log.info("register "..extend.."."..group.."."..template.." action")
diagram.registerBlock(extend, group, template)
diagram.blockUiUseImage(group, template, image)

diagram.blockRegisterProperty(group, template, prop_duration, 0.0)

--
-- Block UI
--
diagram.blockUiAddText(group, template, core.translate(txt_shake))

--
-- Inspector UI
-- 
diagram.inspectorUiBindNumberSpin(group, template, prop_duration, core.translate(label_duration), 0, 9999999)

