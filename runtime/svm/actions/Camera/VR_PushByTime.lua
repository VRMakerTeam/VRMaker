local core = require "core"
local log = require "log"
local diagram = require "diagram"
local canvas = require "canvas"

local extend = "VR"
local group = "Camera"
local template = "VR_PushByTime"

local image = "Camera/Camera.png"

local prop_duration = "duration"

local label_duration = {en_US="Duration", zh_CN="时间"}

local txt_push = {en_US="Push(Time)", zh_CN="推近（时间）"}

log.info("register "..extend.."."..group.."."..template.." action")
diagram.registerBlock(extend, group, template)
diagram.blockUiUseImage(group, template, image)

diagram.blockRegisterProperty(group, template, prop_duration, 1.0)

--
-- Block UI
--
diagram.blockUiAddText(group, template, core.translate(txt_push))

--
-- Inspector UI
-- 
diagram.inspectorBindCallback(group, template, "onBuildFinish", onInspectorBuildFinish)

diagram.inspectorUiBindNumberSpin(group, template, prop_duration, core.translate(label_duration), 0, 999999999.0)

